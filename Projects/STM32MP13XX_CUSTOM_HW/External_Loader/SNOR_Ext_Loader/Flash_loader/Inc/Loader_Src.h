/**
  ******************************************************************************
  * @file    Loader_Src.h
  * @author  MCD Application Team
  * @brief   Header file of Loader_Src.c
  *           
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
#ifndef __LOADER_SRC_H
#define __LOADER_SRC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_hal.h"
#include "stm32mp13xx_valid_xspi.h"


#define TIMEOUT 5000U
#define KeepInCompilation __root

/* Private function prototypes -----------------------------------------------*/
//int Init ();
//KeepInCompilation int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
//KeepInCompilation int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress);
//KeepInCompilation uint64_t Verify (uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement);
//KeepInCompilation HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
//KeepInCompilation int MassErase (uint32_t Parallelism );
//void SystemClock_Config(void);
//void HAL_MspInit(void);


#endif /* __LOADER_SRC_H */

