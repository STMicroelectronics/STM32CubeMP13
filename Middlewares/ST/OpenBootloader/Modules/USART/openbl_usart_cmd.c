/**
  ******************************************************************************
  * @file    openbl_usart_cmd.c
  * @author  MCD Application Team
  * @brief   Contains USART protocol commands
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "openbl_usart_cmd.h"
#include "openbootloader_conf.h"
#include "app_openbootloader.h"
#include "usart_interface.h"
#include "external_memory_interface.h"
#include "openbl_util.h"

/* External variables --------------------------------------------------------*/
extern OPENBL_Flashlayout_TypeDef FlashlayoutStruct;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_USART_COMMANDS_NB          8U       /* Number of supported commands */

#define USART_RAM_BUFFER_SIZE             1024U    /* Size of USART buffer used to store received data from the host */

#define OPENBL_USART_PACKET_SIZE          256      /* Size of USART Packet send by the host */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer used to store received data from the host */
static uint8_t USART_RAM_Buf[USART_RAM_BUFFER_SIZE];
#if (defined SNOR_FLASH) || (defined SDCARD_FLASH)
static uint8_t phase = PHASE_FLASHLAYOUT;
/* phase_mirror is used to retain value of current phase in OPENBL_USART_GetPhase() till
   phase/partition : 0x4 and 0x5 is downloaded in OPENBL_USART_Download(). As only 256 bytes
   of USART packet is sent by CubeProgrammer and minimum SD block size to be written is 512 bytes so
   2 USART packets are clubbed and then written to SDCard */
static uint8_t phase_mirror = PHASE_FLASHLAYOUT;
#else
static uint8_t phase = PHASE_FLASHLAYOUT;
#endif
static uint32_t destination = RAM_WRITE_ADDRESS;
static uint32_t cur_sector = 0;
static uint32_t last_sector = 0;
static uint8_t operation;
static uint32_t packet_number = 0;
static bool is_fl = true;
static uint32_t cur_part = 1;

/* Private function prototypes -----------------------------------------------*/
static void OPENBL_USART_GetCommand(void);
static void OPENBL_USART_GetVersion(void);
static void OPENBL_USART_GetID(void);
static void OPENBL_USART_GetPhase(void);
static void OPENBL_USART_ReadMemory(void);
static void OPENBL_USART_Download(void);
static void OPENBL_USART_ReadPartition(void);
static void OPENBL_USART_Start(void);
static uint8_t OPENBL_USART_GetAddress(uint32_t *Address);
void Write_Flash_layout_Ext_Mem(void);

/* Exported variables --------------------------------------------------------*/
OPENBL_CommandsTypeDef OPENBL_USART_Commands =
{
  OPENBL_USART_GetCommand,
  OPENBL_USART_GetVersion,
  OPENBL_USART_GetID,
  OPENBL_USART_GetPhase,
  OPENBL_USART_ReadMemory,
  OPENBL_USART_Download,
  OPENBL_USART_ReadPartition,
  OPENBL_USART_Start
};

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  This function is used to get a pointer to the structure that contains the available USART commands.
  * @return Returns a pointer to the OPENBL_USART_Commands struct.
  */
OPENBL_CommandsTypeDef *OPENBL_USART_GetCommandsList(void)
{
  return (&OPENBL_USART_Commands);
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to get the list of the available USART commands
  * @retval None.
  */
static void OPENBL_USART_GetCommand(void)
{
  uint32_t counter;

  const uint8_t a_OPENBL_USART_CommandsList[OPENBL_USART_COMMANDS_NB] =
  {
    CMD_GET_COMMAND,
    CMD_GET_VERSION,
    CMD_GET_ID,
    CMD_GET_PHASE,
    CMD_READ_MEMORY,
    CMD_READ_PARTITION,
    CMD_START,
    CMD_DOWNLOAD
  };

  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Send the number of commands supported by the USART protocol */
  OPENBL_USART_SendByte(OPENBL_USART_COMMANDS_NB);

  /* Send USART protocol version */
  OPENBL_USART_SendByte(OPENBL_USART_VERSION);

  /* Send the list of supported commands */
  for (counter = 0U; counter < OPENBL_USART_COMMANDS_NB; counter++)
  {
    OPENBL_USART_SendByte(a_OPENBL_USART_CommandsList[counter]);
  }

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the USART protocol version.
  * @retval None.
  */
static void OPENBL_USART_GetVersion(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Send USART protocol version */
  OPENBL_USART_SendByte(OPENBL_USART_VERSION);

  /* Send the Option bytes */
  OPENBL_USART_SendByte(0x00);
  OPENBL_USART_SendByte(0x00);

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the device ID.
  * @retval None.
  */
static void OPENBL_USART_GetID(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Send the number of following bytes - 1 */
  OPENBL_USART_SendByte(0x01);

  /* Send the device ID starting by the MSB byte then the LSB byte */
  OPENBL_USART_SendByte(DEVICE_ID_MSB);
  OPENBL_USART_SendByte(DEVICE_ID_LSB);

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the phase ID.
  * @retval None.
  */
static void OPENBL_USART_GetPhase(void)
{
  uint8_t length = 1;
  phase_mirror = phase;

  /* First phase is reserved for flashlayout phase */
  if (phase == PHASE_FLASHLAYOUT)
  {
    destination = FLASHLAYOUT_RAM_ADDRESS;
    phase = PHASE_0x3;
  }
#if (defined SNOR_FLASH) || (defined SDCARD_FLASH)
  else if (phase == PHASE_0x3)
  {
    destination = RAM_WRITE_ADDRESS;
    phase = PHASE_0x4;
  }
  /* Flash in External memory phase */
  else if ((phase == PHASE_0x4) || (phase == PHASE_0x5))
    {
	    uint8_t i = 0;
		for (i = 0; i < FlashlayoutStruct.partsize; i++)
		{
			if (FlashlayoutStruct.id[i] == phase)
			{
				break;
			}
		}
		if (i == FlashlayoutStruct.partsize)
		{
			destination = UNDEF_ADDRESS;
		}
		else
		{
			destination = EXT_MEMORY_START_ADDRESS + FlashlayoutStruct.offset[i];
		}
		if (phase == PHASE_0x4)
		{
			/* Init external memory */
			if (1 != OPENBL_MEM_Init(destination))
			{
				OPENBL_USART_SendByte(NACK_BYTE);
				return;
			}
			phase = PHASE_0x5;
		}
		else
		{
			phase = PHASE_END;
		}
    }
  else if (phase == PHASE_END)
  {

  }
#endif /* (defined SNOR_FLASH) || (defined SDCARD_FLASH) */
  else /* Other phase and after flashlayout parsing */
  {
    /* Check if there is available partition */
    if (cur_part < FlashlayoutStruct.partsize)
    {
      /* Get the current partition phase ID */
      phase = FlashlayoutStruct.id[cur_part];

      /* Get the destination address based on current partion ip */
      if (!strcmp(FlashlayoutStruct.ip[cur_part], "none"))
      {
        /* If partition ip is "none" destination is RAM address */
        destination = RAM_WRITE_ADDRESS;
      }
      else if ((!strcmp(FlashlayoutStruct.ip[cur_part], "nor")) ||
              (!strcmp(FlashlayoutStruct.ip[cur_part], "mmc")))
      {
        /* If partiton ip is "nor" destination is external memory address (qspi nor) */
        destination = FlashlayoutStruct.offset[cur_part];
      }
      else /* Only none, nor are supported */
      {
    	  OPENBL_USART_SendByte(NACK_BYTE);
      }

      /* Go to the next partition */
      cur_part++;

      /* Init the external memories */
	  if (1 != OPENBL_MEM_Init(destination))
	  {
		OPENBL_USART_SendByte(NACK_BYTE);
		return;
	  }
    }
    else
    {
      /* End operation after this phase */
      phase = PHASE_END;
    }
  }

  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  OPENBL_USART_SendByte(length + 5);               /* Total length */

  OPENBL_USART_SendByte(phase_mirror & 0xFF);             /* partition ID */

  OPENBL_USART_SendByte(destination);              /* byte 1 of address */
  OPENBL_USART_SendByte(destination >> 8);         /* byte 2 of address */
  OPENBL_USART_SendByte(destination >> 16);        /* byte 3 of address */
  OPENBL_USART_SendByte(destination >> 24);        /* byte 4 of address */

  OPENBL_USART_SendByte(length);                   /* Information length */

  OPENBL_USART_SendByte(0);                        /* Send information */

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
 * @brief  This function is used to read memory from the device.
 * @retval None.
 */
static void OPENBL_USART_ReadMemory(void)
{
  uint32_t address;
  uint32_t counter;
  uint32_t memory_index;
  uint8_t data;
  uint8_t tmpXOR;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get the memory address */
  if (OPENBL_USART_GetAddress(&address) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Get the number of bytes to be received */
    data = OPENBL_USART_ReadByte();
    tmpXOR  = ~data;

    /* Check data integrity */
    if (OPENBL_USART_ReadByte() != tmpXOR)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }
    else
    {
      OPENBL_USART_SendByte(ACK_BYTE);

      /* Get the memory index to know from which memory we will read */
      memory_index = OPENBL_MEM_GetMemoryIndex(address);

      /* Read the data (data + 1) from the memory and send them to the host */
      for (counter = ((uint32_t)data + 1U); counter != 0U; counter--)
      {
        OPENBL_USART_SendByte(OPENBL_MEM_Read(address, memory_index));
        address++;
      }
    }
  }
}

void Write_Flash_layout_Ext_Mem(void)
{
	uint32_t i = 0;
	uint8_t tmpBuffer[512];
	OPENBL_Flashlayout_Storage_TypeDef (*FlashlayoutStorageStruct)[] = (OPENBL_Flashlayout_Storage_TypeDef (*)[])&tmpBuffer;

	for (i = 0; i < FlashlayoutStruct.partsize; i++)
	{
		strncpy((*FlashlayoutStorageStruct)[i].name, FlashlayoutStruct.name[i], sizeof((*FlashlayoutStorageStruct)[i].name));
		(*FlashlayoutStorageStruct)[i].id = FlashlayoutStruct.id[i];
		(*FlashlayoutStorageStruct)[i].offset = FlashlayoutStruct.offset[i];
	}
#if defined(SDCARD_FLASH)
	OPENBL_MEM_Write(0x00, (uint8_t *)tmpBuffer, 1);
#elif defined(SNOR_FLASH)
	OPENBL_MEM_SectorErase((EXT_MEMORY_START_ADDRESS + GPT_OFFSET), (EXT_MEMORY_START_ADDRESS + GPT_OFFSET), ((EXT_MEMORY_START_ADDRESS + GPT_OFFSET) + 512));
	OPENBL_MEM_Write((EXT_MEMORY_START_ADDRESS + GPT_OFFSET), (uint8_t *)tmpBuffer, 512);
#endif /* SDCARD_FLASH */
}

/**
 * @brief  This function is used to write in to device memory.
 * @retval None.
 */
static void OPENBL_USART_Download(void)
{
  uint32_t address;
  uint32_t tmpXOR;
  uint32_t counter;
  uint32_t codesize;
  uint8_t *ramaddress;
  uint8_t data;
  uint32_t res;
#if defined SDCARD_FLASH
  /* prev_code_size is used to cache code_size and thus provide appropriate offset for clubbing 2 USART packets and then writing data to SDCard */
  static uint16_t prev_code_size = 0;
  static uint32_t codesize_verify = 0;
#endif

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get the memory address */
  if (OPENBL_USART_GetAddress(&address) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Read number of bytes to be written and data */
    ramaddress = (uint8_t *)USART_RAM_Buf;

    /* Read the number of bytes to be written: Max number of data = data + 1 = 256 */
    data = OPENBL_USART_ReadByte();

    /* Number of data to be written = data + 1 */
    codesize = (uint32_t)data + 1U;

    /* Checksum Initialization */
    tmpXOR = data;

#if defined SDCARD_FLASH
    if((phase_mirror == PHASE_0x4) || (phase_mirror == PHASE_0x5))
    {
        /* ramaddress should be incremented with prev_code_size so that the incoming USART packet is written at
           appropriate offset required for clubbing 2 USART packets for SD write */
        if (packet_number  % 2 != 0)
        {
            ramaddress += prev_code_size;
        }
        else
        {
            prev_code_size = codesize;
            codesize_verify = 0;
        }
        codesize_verify += prev_code_size;
    }
#endif

    /* UART receive data and send to RAM Buffer */
    for (counter = codesize; counter != 0U ; counter--)
    {
      data    = OPENBL_USART_ReadByte();
      tmpXOR ^= data;

      *(__IO uint8_t *)(ramaddress) = data;

      ramaddress++;
    }

    /* Send NACK if Checksum is incorrect */
    if (OPENBL_USART_ReadByte() != tmpXOR)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }
    else
    {
        /* If External memory download, erase the sector */
        /* address typecasted to int32_t to avoid warning in IAR */
    	  if ((int32_t)address >= EXT_MEMORY_START_ADDRESS && address <= EXT_MEMORY_END_ADDRESS)
        {
#if defined SDCARD_FLASH
          cur_sector = address - EXT_MEMORY_START_ADDRESS;
#elif defined SNOR_FLASH
          cur_sector = ((address - EXT_MEMORY_START_ADDRESS) / SECTOR_SIZE) + 1;
#endif
          if (cur_sector > last_sector)
          {
#if defined SDCARD_FLASH
            /* Erase sector */
            OPENBL_MEM_SectorErase(address, address, (address + 1));
#elif defined SNOR_FLASH
            /* Erase sector */
            OPENBL_MEM_SectorErase(address, address, (address + codesize));
#endif
            last_sector = cur_sector;
          }
        }

        if (0 == packet_number)
        {
			if (PHASE_0x4 == phase_mirror)
			{
#if defined SDCARD_FLASH
				/* Sector Erase is being to erase first 2MB so that GPT table gets erased in case SD Card
				 * is previously programmed with Linux Image */
				OPENBL_MEM_SectorErase(EXT_MEMORY_START_ADDRESS, EXT_MEMORY_START_ADDRESS, (2 * 1024 * 1024));
#endif /* SDCARD_FLASH */
				uint32_t command = CONFIGURE_ENABLE_BOOT1_PARTITION;
				OPENBL_MEM_Configure(address, (void *)&command);
			}
			else if (PHASE_0x5 == phase_mirror)
			{
				uint32_t command = CONFIGURE_DISABLE_BOOT1_PARTITION;
				OPENBL_MEM_Configure(address, (void *)&command);
				Write_Flash_layout_Ext_Mem();
			}
        }
#if defined SDCARD_FLASH
      	if((phase_mirror == PHASE_0x4) || (phase_mirror == PHASE_0x5))
        {
        	/*As only 256 bytes of USART packet is sent by CubeProgrammer and minimum SD block size to be written is 512 bytes so
        	2 USART packets are clubbed and then written to SDCard */
        	if ((codesize > 0) && (codesize <= 512))
        	{
        		OPENBL_MEM_Write(address, (uint8_t *)USART_RAM_Buf, 1);
        	}
        }
        else
        {
            /* Write data to memory */
            OPENBL_MEM_Write(address, (uint8_t *)USART_RAM_Buf, codesize);
        }

#else
        /* Write data to memory */
        OPENBL_MEM_Write(address, (uint8_t *)USART_RAM_Buf, codesize);
#endif

        /* First write memory operation is reserved for the flashlayout */
        if (is_fl)
        {
          /* First packet number of flashlayout download is reserved for ST binary signature */
          if (packet_number == 0)
          {
            /* Skip ST binary signature */
          }
          else
          {
            /* Parse the flashlayout */
            if (parse_flash_layout(address, codesize) == PARSE_ERROR)
            {
              OPENBL_USART_SendByte(NACK_BYTE);
            }
            is_fl = false; /* Leave the flashlayout parsing */
          }
        }

        /* If External memory download, verify data write to memory */
        /* address typecasted to int32_t to avoid warning in IAR */
        if ((int32_t)address >= EXT_MEMORY_START_ADDRESS && address <= EXT_MEMORY_END_ADDRESS)
        {
#if defined SNOR_FLASH
          /* Verify data write to memory */
          res = OPENBL_MEM_Verify(address, (uint32_t)USART_RAM_Buf, codesize, 0);
          if ((res != 0) && (res < (address + codesize)))
#endif
#if defined SDCARD_FLASH
          /* Verify data write to SDCard memory */
          res = OPENBL_MEM_Verify((address * BLOCKSIZE_SD), (uint32_t)USART_RAM_Buf, codesize_verify, 0);
          if (res != 0)
          {
            OPENBL_USART_SendByte(NACK_BYTE);
          }
#endif
#if defined SNOR_FLASH
          if ((res != 0) && (res < (address + codesize)))
          {
            OPENBL_USART_SendByte(NACK_BYTE);
          }
#endif
        }

      /* Send last Acknowledge synchronization byte */
      OPENBL_USART_SendByte(ACK_BYTE);
    }
  }
}

/**
 * @brief  This function is used to read data from NVM, Flash.
 * @retval None.
 */
static void OPENBL_USART_ReadPartition(void)
{
  uint32_t offset;
  uint32_t tmpXOR;
  uint32_t codesize;
  uint8_t data;
  uint8_t tmpOffset[4] = {0, 0, 0, 0};
  uint8_t partId;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get partition ID byte */
  partId = OPENBL_USART_ReadByte();

  /* Get offset address bytes */
  tmpOffset[3] = OPENBL_USART_ReadByte();
  tmpOffset[2] = OPENBL_USART_ReadByte();
  tmpOffset[1] = OPENBL_USART_ReadByte();
  tmpOffset[0] = OPENBL_USART_ReadByte();

  tmpXOR = tmpOffset[3] ^ tmpOffset[2] ^ tmpOffset[1] ^ tmpOffset[0] ^ partId;

  /* Check the integrity of received data */
  if (OPENBL_USART_ReadByte() != tmpXOR)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
	  /* Get offset value */
    offset = (((uint32_t)tmpOffset[3] << 24) | ((uint32_t)tmpOffset[2] << 16) | ((uint32_t)tmpOffset[1] << 8) | (uint32_t)tmpOffset[0]);

    /* Save the operation type */
    operation = partId;

    /* Save the packet number */
    packet_number = offset / OPENBL_USART_PACKET_SIZE;

    OPENBL_USART_SendByte(ACK_BYTE);

    /* NbrOfData to read = data + 1 */
    data = OPENBL_USART_ReadByte();

    /* Number of data to be written = data + 1 */
    codesize = (uint32_t)data + 1U;

    /* Size for Word writing */
    codesize /= 4;

    /* Checksum Initialization */
    tmpXOR = data;

    /* Get the data */
    data = OPENBL_USART_ReadByte();

    /* Check the integrity of received data */
    if ((data ^ tmpXOR) != 0xFF)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }

    OPENBL_USART_SendByte(ACK_BYTE);
  }
}

/**
  * @brief  This function is used to jump to the user application.
  * @retval None.
  */
static void OPENBL_USART_Start(void)
{
  uint32_t address;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get memory address */
  if (OPENBL_USART_GetAddress(&address) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
   /* If the jump address is valid then send ACK */
    OPENBL_USART_SendByte(ACK_BYTE);
  }
}

/**
 * @brief  This function is used to get a valid address.
 * @retval Returns NACK status in case of error else returns ACK status.
 */
static uint8_t OPENBL_USART_GetAddress(uint32_t *Address)
{
  uint8_t tmpAddress[4] = {0, 0, 0, 0};
  uint8_t status;
  uint8_t tmpXOR;

  tmpAddress[3] = OPENBL_USART_ReadByte();
  tmpAddress[2] = OPENBL_USART_ReadByte();
  tmpAddress[1] = OPENBL_USART_ReadByte();
  tmpAddress[0] = OPENBL_USART_ReadByte();

  tmpXOR = tmpAddress[3] ^ tmpAddress[2] ^ tmpAddress[1] ^ tmpAddress[0];

  /* Check the integrity of received data */
  if (OPENBL_USART_ReadByte() != tmpXOR)
  {
    status = NACK_BYTE;
  }
  else
  {
	  /* Get the start address */
    *Address = (((uint32_t)tmpAddress[3] << 24) | ((uint32_t)tmpAddress[2] << 16) | ((uint32_t)tmpAddress[1] << 8) | (uint32_t)tmpAddress[0]);

    /* Get the operation type */
    operation = (uint8_t)(*Address >> 24);

    /* Get the packet number */
    packet_number = (uint32_t)((*Address << 8) >> 8);

    /* Check if the operation is supported */
    switch (operation)
    {
    case PHASE_FLASHLAYOUT:
      break;
    default:
      status = NACK_BYTE;
    }

    /* Check if the address is supported */
    if (*Address == 0xFFFFFFFF)
    {
      status = ACK_BYTE;
    }
    else
    {

#if defined SDCARD_FLASH
    if((phase_mirror == PHASE_0x4) || (phase_mirror == PHASE_0x5))
    {
        /* Build SDCard block address */
        *Address = destination + ((packet_number * OPENBL_USART_PACKET_SIZE)/BLOCKSIZE_SD);
    }
    else
    {
        /* Build the real memory address */
        *Address = destination + (packet_number * OPENBL_USART_PACKET_SIZE);
    }
#else
      /* Build the real memory address */
      *Address = destination + (packet_number * OPENBL_USART_PACKET_SIZE);
#endif

      /* Check if the memory address is valid */
      if (OPENBL_MEM_GetAddressArea(*Address) == AREA_ERROR)
      {
        status = NACK_BYTE;
      }
      else
      {
        status = ACK_BYTE;
      }
    }
  }

  return status;
}
