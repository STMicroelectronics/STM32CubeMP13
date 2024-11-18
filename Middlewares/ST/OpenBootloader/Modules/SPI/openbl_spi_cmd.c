/**
  ******************************************************************************
  * @file    openbl_spi_cmd.c
  * @author  MCD Application Team
  * @brief   Contains SPI protocol commands
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "openbl_mem.h"
#include "openbl_spi_cmd.h"

#include "openbootloader_conf.h"
#include "spi_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_SPI_COMMANDS_NB            11U  /* Number of supported commands */
#define SPI_RAM_BUFFER_SIZE             1156U  /* Size of SPI buffer used to store received data from the host */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer used to store received data from the host */
static uint8_t SPI_RAM_Buf[SPI_RAM_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void OPENBL_SPI_GetCommand(void);
static void OPENBL_SPI_GetVersion(void);
static void OPENBL_SPI_GetID(void);
static void OPENBL_SPI_ReadMemory(void);
static void OPENBL_SPI_WriteMemory(void);
static void OPENBL_SPI_Go(void);
static void OPENBL_SPI_ReadoutProtect(void);
static void OPENBL_SPI_ReadoutUnprotect(void);
static void OPENBL_SPI_EraseMemory(void);
static void OPENBL_SPI_WriteProtect(void);
static void OPENBL_SPI_WriteUnprotect(void);

static uint8_t OPENBL_SPI_GetAddress(uint32_t *Address);

/* Exported variables --------------------------------------------------------*/
OPENBL_CommandsTypeDef OPENBL_SPI_Commands =
{
  OPENBL_SPI_GetCommand,
  OPENBL_SPI_GetVersion,
  OPENBL_SPI_GetID,
  OPENBL_SPI_ReadMemory,
  OPENBL_SPI_WriteMemory,
  OPENBL_SPI_Go,
  OPENBL_SPI_ReadoutProtect,
  OPENBL_SPI_ReadoutUnprotect,
  OPENBL_SPI_EraseMemory,
  OPENBL_SPI_WriteProtect,
  OPENBL_SPI_WriteUnprotect,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  This function is used to get a pointer to the structure that contains the available SPI commands.
  * @retval Returns a pointer to the OPENBL_SPI_Commands struct.
  */
OPENBL_CommandsTypeDef *OPENBL_SPI_GetCommandsList(void)
{
  return (&OPENBL_SPI_Commands);
}

/**
  * @brief  This function is used to get the list of the available SPI commands
  * @retval None.
  */
static void OPENBL_SPI_GetCommand(void)
{
  uint32_t counter;
  const uint8_t a_OPENBL_SPI_CommandsList[OPENBL_SPI_COMMANDS_NB] =
  {
    CMD_GET_COMMAND,
    CMD_GET_VERSION,
    CMD_GET_ID,
    CMD_READ_MEMORY,
    CMD_GO,
    CMD_WRITE_MEMORY,
    CMD_EXT_ERASE_MEMORY,
    CMD_WRITE_PROTECT,
    CMD_WRITE_UNPROTECT,
    CMD_READ_PROTECT,
    CMD_READ_UNPROTECT
  };

  OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

  /* Send the number of commands supported by the SPI protocol */
  OPENBL_SPI_SendByte(OPENBL_SPI_COMMANDS_NB);

  /* Send SPI protocol version */
  OPENBL_SPI_SendByte(OPENBL_SPI_VERSION);

  /* Send the list of supported commands */
  for (counter = 0U; counter < OPENBL_SPI_COMMANDS_NB; counter++)
  {
    OPENBL_SPI_SendByte(a_OPENBL_SPI_CommandsList[counter]);
  }

  /* Send last Acknowledge synchronization byte */
  OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the SPI protocol version.
  * @retval None.
  */
static void OPENBL_SPI_GetVersion(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

  /* Send SPI protocol version */
  OPENBL_SPI_SendByte(OPENBL_SPI_VERSION);

  /* Send last Acknowledge synchronization byte */
  OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the device ID.
  * @retval None.
  */
static void OPENBL_SPI_GetID(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

  OPENBL_SPI_SendByte(0x01);

  /* Send the device ID starting by the MSB byte then the LSB byte */
  OPENBL_SPI_SendByte(DEVICE_ID_MSB);
  OPENBL_SPI_SendByte(DEVICE_ID_LSB);

  /* Send last Acknowledge synchronization byte */
  OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);
}

/**
 * @brief  This function is used to read memory from the device.
 * @retval None.
 */
static void OPENBL_SPI_ReadMemory(void)
{
  uint32_t address;
  uint32_t counter;
  uint32_t memory_index;
  uint8_t data;
  uint8_t xor;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_SPI_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
    }
    else
    {
      OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

      /* Get the number of bytes to be received */
      data = OPENBL_SPI_ReadByte();
      xor  = ~data;

      /* Check data integrity */
      if (OPENBL_SPI_ReadByte() != xor)
      {
        OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
      }
      else
      {
        OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

        /* Get the memory index to know from which memory we will read */
        memory_index = OPENBL_MEM_GetMemoryIndex(address);

        /* Read the data (data + 1) from the memory and send them to the host */
        for (counter = ((uint32_t)data + 1U); counter != 0U; counter--)
        {
          OPENBL_SPI_SendByte(OPENBL_MEM_Read(address, memory_index));
          address++;
        }
      }
    }
  }
}

/**
 * @brief  This function is used to write in to device memory.
 * @retval None.
 */
static void OPENBL_SPI_WriteMemory(void)
{
  uint32_t address;
  uint32_t xor;
  uint32_t counter;
  uint32_t codesize;
  uint32_t mem_area;
  uint8_t *ramaddress;
  uint8_t data;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_SPI_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
    }
    else
    {
      OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

      /* Read number of bytes to be written and data */
      ramaddress = (uint8_t *)SPI_RAM_Buf;

      /* Read the number of bytes to be written: Max number of data = data + 1 = 256 */
      data = OPENBL_SPI_ReadByte();

      /* Number of data to be written = data + 1 */
      codesize = (uint32_t)data + 1U;

      /* Checksum Initialization */
      xor = data;

      /* SPI receive data and send to RAM Buffer */
      for (counter = codesize; counter != 0U ; counter--)
      {
        data  = OPENBL_SPI_ReadByte();
        xor  ^= data;

        *(__IO uint8_t *)(ramaddress) = data;

        ramaddress++;
      }

      /* Send NACk if Checksum is incorrect */
      if (OPENBL_SPI_ReadByte() != xor)
      {
        OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
      }
      else
      {
        /* Write data to memory */
        OPENBL_MEM_Write(address, (uint8_t *)SPI_RAM_Buf, codesize);

        /* Send last Acknowledge synchronization byte */
        OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

        /* Check if the received address is an option byte address */
        mem_area = OPENBL_MEM_GetAddressArea(address);

        if (mem_area == OB_AREA)
        {
          /* Launch Option Bytes reload */
          OPENBL_MEM_OptionBytesLaunch();
        }
      }
    }
  }
}

/**
  * @brief  This function is used to jump to the user application.
  * @retval None.
  */
static void OPENBL_SPI_Go(void)
{
  uint32_t address;
  uint8_t status;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

    /* Get memory address */
    if (OPENBL_SPI_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
    }
    else
    {
      /* Check if received address is valid or not */
      status = OPENBL_MEM_CheckJumpAddress(address);

      if (status == 0U)
      {
        OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
      }
      else
      {
        /* If the jump address is valid then send ACK */
        OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

        OPENBL_MEM_JumpToAddress(address);
      }
    }
  }
}

/**
 * @brief  This function is used to enable readout protection.
 * @retval None.
 */
static void OPENBL_SPI_ReadoutProtect(void)
{
  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

    /* Enable the read protection */
    OPENBL_MEM_SetReadOutProtection(ENABLE);

    OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

    /* Launch Option Bytes reload */
    OPENBL_MEM_OptionBytesLaunch();
  }
}

/**
 * @brief  This function is used to disable readout protection.
 * @note   Going from RDP level 1 to RDP level 0 erases all the flash,
 *         so the send of the second acknowledge after disabling the read protection
 *         is not possible, this is why the two ACKs are sent successively
 * @retval None.
 */
static void OPENBL_SPI_ReadoutUnprotect(void)
{
  OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

  OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

  /* Disable the read protection */
  OPENBL_MEM_SetReadOutProtection(DISABLE);

  /* Launch Option Bytes reload and reset system */
  OPENBL_MEM_OptionBytesLaunch();
}

/**
 * @brief  This function is used to erase a memory.
 * @retval None.
 */
static void OPENBL_SPI_EraseMemory(void)
{
  uint32_t xor;
  uint32_t counter;
  uint32_t numpage;
  uint16_t data;
  ErrorStatus error_value;
  uint8_t status = ACK_BYTE;
  uint8_t *ramaddress;

  ramaddress = (uint8_t *) SPI_RAM_Buf;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

    /* Read number of pages to be erased */
    data = OPENBL_SPI_ReadByte();
    data = (uint16_t)(data << 8) | OPENBL_SPI_ReadByte();

    /* Checksum initialization */
    xor  = ((uint32_t)data & 0xFF00U) >> 8;
    xor ^= (uint32_t)data & 0x00FFU;

    /* All commands in range 0xFFFZ are reserved for special erase features */
    if ((data & 0xFFF0U) == 0xFFF0U)
    {
      /* Check data integrity */
      if (OPENBL_SPI_ReadByte() != (uint8_t) xor)
      {
        status = NACK_BYTE;
      }
      else
      {
        if ((data == 0xFFFFU) || (data == 0xFFFEU) || (data == 0xFFFDU))
        {
          ramaddress[0] = (uint8_t)(data & 0x00FFU);
          ramaddress[1] = (uint8_t)((data & 0xFF00U) >> 8);

          /* Enable busy state */
          OPENBL_SPI_EnableBusyState();

          error_value = OPENBL_MEM_MassErase(FLASH_START_ADDRESS, (uint8_t *) SPI_RAM_Buf, SPI_RAM_BUFFER_SIZE);

          /* Disable busy state */
          OPENBL_SPI_DisableBusyState();

          if (error_value == SUCCESS)
          {
            status = ACK_BYTE;
          }
          else
          {
            status = NACK_BYTE;
          }
        }
        else
        {
          /* This sub-command is not supported */
          status = NACK_BYTE;
        }
      }
    }
    else
    {
      /* Check data integrity */
      if (OPENBL_SPI_ReadByte() != (uint8_t) xor)
      {
        status = NACK_BYTE;
      }
      else
      {
        OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

        ramaddress = (uint8_t *) SPI_RAM_Buf;

        /* Number of pages to be erased (data + 1) */
        numpage = (uint32_t)data + 1U;

        *ramaddress = (uint8_t)(numpage & 0x00FFU);
        ramaddress++;

        *ramaddress = (uint8_t)((numpage & 0xFF00U) >> 8);
        ramaddress++;

        /* Checksum Initialization */
        xor = 0U;

        /* Get the pages to be erased */
        for (counter = numpage; counter != 0U ; counter--)
        {
          /* Receive the MSB byte */
          data  = OPENBL_SPI_ReadByte();
          xor  ^= data;
          data  = (uint16_t)((data & 0x00FFU) << 8);

          /* Receive the LSB byte */
          data |= (uint8_t)(OPENBL_SPI_ReadByte() & 0x00FFU);
          xor  ^= ((uint32_t)data & 0x00FFU);

          /* Only store data that fit in the buffer length */
          if (counter < (SPI_RAM_BUFFER_SIZE / 2U))
          {
            *ramaddress = (uint8_t)(data & 0x00FFU);
            ramaddress++;

            *ramaddress = (uint8_t)((data & 0xFF00U) >> 8);
            ramaddress++;
          }
        }

        /* Check data integrity */
        if (OPENBL_SPI_ReadByte() != (uint8_t) xor)
        {
          status = NACK_BYTE;
        }
        else
        {
          /* Enable busy state */
          OPENBL_SPI_EnableBusyState();

          error_value = OPENBL_MEM_Erase(FLASH_START_ADDRESS, (uint8_t *) SPI_RAM_Buf, SPI_RAM_BUFFER_SIZE);

          /* Disable busy state */
          OPENBL_SPI_DisableBusyState();

          /* Errors from memory erase are not managed, always return ACK */
          if (error_value == SUCCESS)
          {
            status = ACK_BYTE;
          }
        }
      }
    }

    OPENBL_SPI_SendAcknowledgeByte(status);
  }
}

/**
 * @brief  This function is used to enable write protect.
 * @retval None.
 */
static void OPENBL_SPI_WriteProtect(void)
{
  uint8_t counter;
  uint8_t length;
  uint8_t data;
  uint8_t xor;
  ErrorStatus error_value;
  uint8_t *ramaddress;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

    /* Get the data length */
    data = OPENBL_SPI_ReadByte();
    xor  = ~data;

    /* Send NACk if Checksum is incorrect */
    if (OPENBL_SPI_ReadByte() != xor)
    {
      OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
    }
    else
    {
      OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

      ramaddress = (uint8_t *) SPI_RAM_Buf;
      length     = data + 1U;

      /* Checksum Initialization */
      xor = 0U;

      /* Receive data and write to RAM Buffer */
      for (counter = (length); counter != 0U ; counter--)
      {
        data  = OPENBL_SPI_ReadByte();
        xor  ^= data;

        *(__IO uint8_t *)(ramaddress) = (uint8_t) data;

        ramaddress++;
      }

      /* Check data integrity and send NACK if Checksum is incorrect */
      if (OPENBL_SPI_ReadByte() != (uint8_t) xor)
      {
        OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
      }
      else
      {
        ramaddress = (uint8_t *) SPI_RAM_Buf;

        /* Enable the write protection */
        error_value = OPENBL_MEM_SetWriteProtection(ENABLE, FLASH_START_ADDRESS, ramaddress, length);

        OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

        if (error_value == SUCCESS)
        {
          OPENBL_MEM_OptionBytesLaunch();
        }
      }
    }
  }
}

/**
 * @brief  This function is used to disable write protect.
 * @retval None.
 */
static void OPENBL_SPI_WriteUnprotect(void)
{
  ErrorStatus error_value;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_SPI_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

    /* Disable write protection */
    error_value = OPENBL_MEM_SetWriteProtection(DISABLE, FLASH_START_ADDRESS, NULL, 0);

    OPENBL_SPI_SendAcknowledgeByte(ACK_BYTE);

    if (error_value == SUCCESS)
    {
      OPENBL_MEM_OptionBytesLaunch();
    }
  }
}

/**
 * @brief  This function is used to get a valid address.
 * @retval Returns NACK status in case of error else returns ACK status.
 */
static uint8_t OPENBL_SPI_GetAddress(uint32_t *Address)
{
  uint8_t data[4] = {0, 0, 0, 0};
  uint8_t status;
  uint8_t xor;

  data[3] = OPENBL_SPI_ReadByte();
  data[2] = OPENBL_SPI_ReadByte();
  data[1] = OPENBL_SPI_ReadByte();
  data[0] = OPENBL_SPI_ReadByte();

  xor = data[3] ^ data[2] ^ data[1] ^ data[0];

  /* Check the integrity of received data */
  if (OPENBL_SPI_ReadByte() != xor)
  {
    status = NACK_BYTE;
  }
  else
  {
    *Address = ((uint32_t)data[3] << 24) | ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];

    /* Check if received address is valid or not */
    if (OPENBL_MEM_GetAddressArea(*Address) == AREA_ERROR)
    {
      status = NACK_BYTE;
    }
    else
    {
      status = ACK_BYTE;
    }
  }

  return status;
}
