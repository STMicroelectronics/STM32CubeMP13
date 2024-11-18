/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stm32mp13xx_valid.h"
#include "stm32mp13xx_valid_xspi.h"
/* USER CODE END Includes */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SystemClock_Config(void);
void MX_GPIO_Init(void);
int Init(void);
void configuration(void *ptr);
int Write(uint32_t Address, uint32_t Size, uint8_t* buffer);
int SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress);
int MassErase(void);
uint32_t CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal);
uint64_t Verify(uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement);
/* USER CODE END EFP */


/* Private defines -----------------------------------------------------------*/
#define DEFAULT_IRQ_PRIO      1U

#ifdef __cplusplus
 extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

