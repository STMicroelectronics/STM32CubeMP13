/**
  ******************************************************************************
  * @file    stm32mp13xx_valid_xspi.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32mp13xx_valid_xspi.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32MP13xx_VALID_XSPI_H
#define __STM32MP13xx_VALID_XSPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_valid.h"
#include "stm32mp13xx_hal.h"
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

 extern XSPI_HandleTypeDef hxspi;

   /**
     * @}
     */

 /** @defgroup STM32MP13XX-VALID_XSPI_Private_Variables STM32MP13XX-VALID_XSPI Private Variables
   * @{
   */

 /** @defgroup STM32MP13XX-VALID_XSPI_Private_Function_Prototypes STM32MP13XX-VALID_XSPI Private Function Prototypes
   * @{
   */
 /**
   * @}
   */

 /** @defgroup STM32MP13XX-VALID_XSPI_Exported_Functions STM32MP13XX-VALID_XSPI Exported Functions
   * @{
   */

 uint8_t BSP_XSPI_Init(void);
 uint8_t BSP_XSPI_EraseSector(uint32_t EraseStartAddress, uint32_t EraseEndAddress);
 uint8_t BSP_XSPI_WriteMemory(uint8_t* buffer, uint32_t address, uint32_t buffer_size);
 uint8_t BSP_XSPI_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
 uint8_t BSP_XSPI_EnableMemoryMappedMode(void);
 uint8_t BSP_XSPI_Erase_Chip (void);
 void MX_XSPI_Init(void);

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

#ifdef __cplusplus
}
#endif

#endif /* __STM32MP13xx_VALID_XSPI_H */
