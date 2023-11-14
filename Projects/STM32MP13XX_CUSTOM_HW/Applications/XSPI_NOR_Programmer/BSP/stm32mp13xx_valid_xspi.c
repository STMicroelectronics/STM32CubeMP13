/**
  ******************************************************************************
  * @file    stm32mp13xx_valid_xspi.c
  * @author  MCD Application Team
  * @brief   This file includes a standard driver for the MX25L512 XSPI
  *          memory mounted on STM32MP13XX_CUSTOM_HW board.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


  /* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_valid_xspi.h"
#include "mx25l512.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX-VALID
  * @{
  */

/** @defgroup STM32MP13XX-VALID_XSPI STM32MP13XX-VALID_XSPI
  * @{
  */

/** @defgroup STM32MP13XX-VALID_XSPI_Private_Types_Definitions STM32MP13XX-VALID_XSPI Types Definitions
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32MP13XX-VALID_XSPI_Private_Defines STM32MP13XX-VALID_XSPI Types Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32MP13XX-VALID_XSPI_Private_Macros STM32MP13XX-VALID_XSPI Private Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32MP13XX-VALID_XSPI_Imported_Variables STM32MP13XX-VALID_XSPI Imported Variables
  * @{
  */
  /**
    * @}
    */

/** @defgroup STM32MP13XX-VALID_XSPI_Private_Variables STM32MP13XX-VALID_XSPI Private Variables
  * @{
  */
XSPI_HandleTypeDef hxspi;


/** @defgroup STM32MP13XX-VALID_XSPI_Private_Function_Prototypes STM32MP13XX-VALID_XSPI Private Function Prototypes
  * @{
  */

static uint8_t XSPI_WriteEnable(void);
static uint8_t XSPI_AutoPollingMemReady(void);
static uint8_t XSPI_Configuration(void);
static uint8_t XSPI_ResetChip(void);

/**
  * @}
  */

/** @defgroup STM32MP13XX-VALID_XSPI_Exported_Functions STM32MP13XX-VALID_XSPI Exported Functions
  * @{
  */

/* XSPI init function */
void MX_XSPI_Init(void)
{
    hxspi.Instance = QUADSPI;
    hxspi.Init.ClockPrescaler = 2;
    hxspi.Init.FifoThresholdByte = 4;
    hxspi.Init.SampleShifting = XSPI_SAMPLE_SHIFTING_NONE;
    hxspi.Init.MemorySize = 25;
    hxspi.Init.ChipSelectHighTimeCycle = 1;
    hxspi.Init.ClockMode = XSPI_CLOCK_MODE_0;
    hxspi.Init.MemoryMode = HAL_XSPI_SINGLE_MEM;
    if (HAL_XSPI_Init(&hxspi) != HAL_OK) {
        BSP_Error_Handler();
    }
}

void HAL_XSPI_MspInit(XSPI_HandleTypeDef* xspiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if (xspiHandle->Instance == QUADSPI) {
    /* USER CODE BEGIN XSPI_MspInit 0 */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInit.QspiClockSelection = RCC_QSPICLKSOURCE_ACLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      BSP_Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();

    /* XSPI GPIO Configuration
    PF10    ------> XSPI_CLK
    PB2     ------> XSPI_BK1_NCS
    PF8     ------> XSPI_BK1_IO0
    PF9     ------> XSPI_BK2_IO1
    PF6     ------> XSPI_BK1_IO2
    PG8     ------> XSPI_BK1_IO3
    PE4     ------> XSPI_BK2_NCS
    PH2     ------> XSPI_BK2_IO0
    PG10    ------> XSPI_BK2_IO1
    PD13    ------> XSPI_BK2_IO2
    PH7     ------> XSPI_BK2_IO3
     */

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_QUADSPI;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_QUADSPI;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* XSPI interrupt Init */
    IRQ_SetPriority(QUADSPI_IRQn, 0x0F);
    IRQ_Enable(QUADSPI_IRQn);

    /* Reset the XSPI memory interface */
    __HAL_RCC_QSPI_FORCE_RESET();
    __HAL_RCC_QSPI_RELEASE_RESET();
  }
}

void HAL_XSPI_MspDeInit(XSPI_HandleTypeDef* xspiHandle)
{
  if (xspiHandle->Instance == QUADSPI) {
    /* USER CODE BEGIN XSPI_MspDeInit 0 */
    /* Reset the XSPI memory interface */
    __HAL_RCC_QSPI_FORCE_RESET();
    __HAL_RCC_QSPI_RELEASE_RESET();

    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /* XSPI GPIO Configuration
    PF10    ------> XSPI_CLK
    PB2     ------> XSPI_BK1_NCS
    PF8     ------> XSPI_BK1_IO0
    PF9     ------> XSPI_BK2_IO1
    PF6     ------> XSPI_BK1_IO2
    PG8     ------> XSPI_BK1_IO3
    PE4     ------> XSPI_BK2_NCS
    PH2     ------> XSPI_BK2_IO0
    PG10    ------> XSPI_BK2_IO1
    PD13    ------> XSPI_BK2_IO2
    PH7     ------> XSPI_BK2_IO3
     */

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_8|GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_2|GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_13);

    /* XSPI interrupt DeInit */
    IRQ_Disable(QUADSPI_IRQn);
  }
}
/* USER CODE BEGIN 1 */

/* XSPI init function */
uint8_t BSP_XSPI_Init(void)
{
  /* prepare XSPI peripheral for ST-Link Utility operations */
  if (HAL_XSPI_DeInit(&hxspi) != HAL_OK) {
      return HAL_ERROR;
  }
  MX_XSPI_Init();
  if (XSPI_ResetChip() != HAL_OK) {
      return HAL_ERROR;
  }
  HAL_Delay(1);
  if (XSPI_AutoPollingMemReady() != HAL_OK) {
      return HAL_ERROR;
  }
  if (XSPI_WriteEnable() != HAL_OK) {
      return HAL_ERROR;
  }
  if (XSPI_Configuration() != HAL_OK) {
      return HAL_ERROR;
  }
  return HAL_OK;
}

uint8_t BSP_XSPI_Erase_Chip(void)
{
  XSPI_RegularCmdTypeDef sCommand;

  if (XSPI_WriteEnable() != HAL_OK) {
      return HAL_ERROR;
  }

  /* Erasing Sequence --------------------------------- */
  sCommand.Instruction = CHIP_ERASE_CMD;
  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.AddressWidth = XSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_NONE;
  sCommand.Address = 0;
  sCommand.DataMode = XSPI_DATA_NONE;
  sCommand.DummyCycles = 0;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  if (HAL_XSPI_Command(&hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }
  if (XSPI_AutoPollingMemReady() != HAL_OK) {
      return HAL_ERROR;
  }
  return HAL_OK;
}

uint8_t XSPI_AutoPollingMemReady(void)
{

  XSPI_RegularCmdTypeDef sCommand;
  XSPI_AutoPollingTypeDef sConfig;

  /* Configure automatic polling mode to wait for memory ready ------ */
  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction = READ_STATUS_REG_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DataMode = XSPI_DATA_1_LINE;
  sCommand.DummyCycles = 0;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  /* For auto polling mode status bytes size is same as DataLength */
  sCommand.DataLength = 1;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  sConfig.MatchValue = 0x00;
  sConfig.MatchMask = 0x01;
  sConfig.MatchMode = XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime = 0x10;
  sConfig.AutomaticStop = XSPI_AUTOMATIC_STOP_ENABLE;
  ConfigCmd = sCommand;
  if (HAL_XSPI_AutoPolling(&hxspi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }
  return HAL_OK;
}

static uint8_t XSPI_WriteEnable(void)
{
  XSPI_RegularCmdTypeDef sCommand;
  XSPI_AutoPollingTypeDef sConfig;

  /* Enable write operations ------------------------------------------ */
  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction = WRITE_ENABLE_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DataMode = XSPI_DATA_NONE;
  sCommand.DummyCycles = 0;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  /* For auto polling mode status bytes size is same as DataLength */
  sCommand.DataLength = 1;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  if (HAL_XSPI_Command(&hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling ---- */
  sConfig.MatchValue = 0x02;
  sConfig.MatchMask = 0x02;
  sConfig.MatchMode = XSPI_MATCH_MODE_AND;
  sConfig.IntervalTime = 0x10;
  sConfig.AutomaticStop = XSPI_AUTOMATIC_STOP_ENABLE;

  sCommand.Instruction = READ_STATUS_REG_CMD;
  sCommand.DataMode = XSPI_DATA_1_LINE;
  ConfigCmd = sCommand;
  if (HAL_XSPI_AutoPolling(&hxspi, &sConfig, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }

  return HAL_OK;
}

/* Enable quad mode and set dummy cycles count */
uint8_t XSPI_Configuration(void)
{

  XSPI_RegularCmdTypeDef sCommand;
  uint8_t test_buffer[4] = { 0 };
  /*read status register*/
  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction = READ_STATUS_REG_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DataMode = XSPI_DATA_1_LINE;
  sCommand.DummyCycles = 0;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  sCommand.DataLength = 1;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  if (HAL_XSPI_Command(&hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }
  if (HAL_XSPI_Receive(&hxspi, test_buffer, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }
  /*read configuration register*/
  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction = READ_CFG_REG_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DataMode = XSPI_DATA_1_LINE;
  sCommand.DummyCycles = 0;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  sCommand.DataLength = 1;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  if (HAL_XSPI_Command(&hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }
  if (HAL_XSPI_Receive(&hxspi, &(test_buffer[1]), HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }

  test_buffer[0] |= 0x40;         /* modify buffer to enable quad mode */
  test_buffer[1] |= 0xC0;         /* set dummy cycles */

  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.AddressWidth = XSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction = WRITE_STATUS_CFG_REG_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_NONE;
  sCommand.DataMode = XSPI_DATA_1_LINE;
  sCommand.DummyCycles = 0;
  sCommand.DataLength = 2;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  if (HAL_XSPI_Command(&hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }
  if (HAL_XSPI_Transmit(&hxspi, test_buffer, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
      return HAL_ERROR;
  }
  return HAL_OK;
}

uint8_t BSP_XSPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{

  XSPI_RegularCmdTypeDef sCommand;

  EraseStartAddress = EraseStartAddress
                      - EraseStartAddress % MX25L512_SECTOR_SIZE;

  /* Erasing Sequence -------------------------------------------------- */
  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.AddressWidth = XSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction = SECTOR_ERASE_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_1_LINE;

  sCommand.DataMode = XSPI_DATA_NONE;
  sCommand.DummyCycles = 0;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  while (EraseEndAddress >= EraseStartAddress) {
      sCommand.Address = (EraseStartAddress & 0x0FFFFFFF);
      if (XSPI_WriteEnable() != HAL_OK) {
          return HAL_ERROR;
      }
      if (HAL_XSPI_Command(&hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
          return HAL_ERROR;
      }
      EraseStartAddress += MX25L512_SECTOR_SIZE;
      if (XSPI_AutoPollingMemReady() != HAL_OK) {
          return HAL_ERROR;
      }
  }

  return HAL_OK;
}

uint8_t BSP_XSPI_WriteMemory(uint8_t* buffer, uint32_t address, uint32_t buffer_size)
{

  XSPI_RegularCmdTypeDef sCommand;
  uint32_t end_addr, current_size, current_addr;

  /* Calculation of the size between the write address and the end of the page */
  current_addr = 0;

  while (current_addr <= address) {
      current_addr += MX25L512_PAGE_SIZE;
  }
  current_size = current_addr - address;

  /* Check if the size of the data is less than the remaining place in the page */
  if (current_size > buffer_size) {
      current_size = buffer_size;
  }

  /* Initialize the address variables */
  current_addr = address;
  end_addr = address + buffer_size;

  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.AddressWidth = XSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction = QUAD_IN_FAST_PROG_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_4_LINES;
  sCommand.DataMode = XSPI_DATA_4_LINES;
  sCommand.DataLength = buffer_size;
  sCommand.Address = address;
  sCommand.DummyCycles = 0;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  /* Perform the write page by page */
  do {
      sCommand.Address = current_addr;
      sCommand.DataLength = current_size;

      if (current_size == 0) {
          return HAL_OK;
      }

      /* Enable write operations */
      if (XSPI_WriteEnable() != HAL_OK) {
          return HAL_ERROR;
      }

      /* Configure the command */
      if (HAL_XSPI_Command(&hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
          return HAL_ERROR;
      }

      /* Transmission of the data */
      if (HAL_XSPI_Transmit(&hxspi, buffer, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
          return HAL_ERROR;
      }

      /* Configure automatic polling mode to wait for end of program */
      if (XSPI_AutoPollingMemReady() != HAL_OK) {
          return HAL_ERROR;
      }

      /* Update the address and size variables for next page programming */
      current_addr += current_size;
      buffer += current_size;
      current_size = ((current_addr + MX25L512_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : MX25L512_PAGE_SIZE;
  } while (current_addr <= end_addr);

  return HAL_OK;
}

/**
  * @brief  Reads an amount of data from the XSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval XSPI memory status
  */
uint8_t BSP_XSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  XSPI_RegularCmdTypeDef s_command;

  /* Configure automatic polling mode to wait for end of program */
  if (XSPI_AutoPollingMemReady() != HAL_OK) {
      return HAL_ERROR;
  }

  /* Initialize the read command */
  s_command.InstructionMode   = XSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = READ_CMD;
  s_command.AddressMode       = XSPI_ADDRESS_1_LINE;
  s_command.AddressWidth       = XSPI_ADDRESS_24_BITS;
  s_command.Address           = ReadAddr;
  s_command.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  s_command.AlternateBytesWidth= 0;
  s_command.DataMode          = XSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.DataLength            = Size;
  s_command.DTRMode           = XSPI_DTR_MODE_DISABLE;
  s_command.DelayHoldHalfCycle  = XSPI_DHHC_ANALOG_DELAY;
  s_command.SIOOMode          = XSPI_SIOO_INST_EVERY_CMD;
  s_command.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  /* Configure the command */
  if (HAL_XSPI_Command(&hxspi, &s_command, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_XSPI_Receive(&hxspi, pData, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

uint8_t BSP_XSPI_EnableMemoryMappedMode(void) {
  XSPI_RegularCmdTypeDef sCommand;
  XSPI_MemoryMappedTypeDef sMemMappedCfg;

  /* Enable Memory-Mapped mode-------------------------------------------------- */

  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.AddressWidth = XSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_1_LINE;
  sCommand.DataMode = XSPI_DATA_4_LINES;
  sCommand.DataLength = 0;
  sCommand.Address = 0;
  sCommand.Instruction = QUAD_OUT_FAST_READ_CMD;
  sCommand.DummyCycles = MX25L512_DUMMY_CYCLES_READ_QUAD_IO;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  sMemMappedCfg.TimeOutActivation = XSPI_TIMEOUT_COUNTER_DISABLE;
  ConfigCmd = sCommand;
  if (HAL_XSPI_MemoryMapped(&hxspi, &sMemMappedCfg) != HAL_OK) {
      return HAL_ERROR;
  }
  return HAL_OK;
}

uint8_t XSPI_ResetChip(void)
{
  XSPI_RegularCmdTypeDef sCommand;
  uint32_t temp = 0;
  /* Erasing Sequence -------------------------------------------------- */
  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.AddressWidth = XSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction = RESET_ENABLE_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_NONE;
  sCommand.Address = 0;
  sCommand.DataMode = XSPI_DATA_NONE;
  sCommand.DummyCycles = 0;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  if (HAL_XSPI_Command(&hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)
      != HAL_OK) {
      return HAL_ERROR;
  }
  for (temp = 0; temp < 0x2f; temp++) {
      __NOP();
  }

  sCommand.InstructionMode = XSPI_INSTRUCTION_1_LINE;
  sCommand.AddressWidth = XSPI_ADDRESS_24_BITS;
  sCommand.AlternateByteMode = XSPI_ALT_BYTES_NONE;
  sCommand.DTRMode = XSPI_DTR_MODE_DISABLE;
  sCommand.DelayHoldHalfCycle = XSPI_DHHC_ANALOG_DELAY;
  sCommand.SIOOMode = XSPI_SIOO_INST_EVERY_CMD;
  sCommand.Instruction = RESET_MEMORY_CMD;
  sCommand.AddressMode = XSPI_ADDRESS_NONE;
  sCommand.Address = 0;
  sCommand.DataMode = XSPI_DATA_NONE;
  sCommand.DummyCycles = 0;
  sCommand.IOSelect = HAL_XSPI_SELECT_IO_3_0;

  if (HAL_XSPI_Command(&hxspi, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE)
      != HAL_OK) {
      return HAL_ERROR;
  }
  return HAL_OK;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
