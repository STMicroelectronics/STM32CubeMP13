/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    common.h
  * @author  MCD Application Team
  * @brief   Common header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#ifndef COMMON_H
#define COMMON_H

#include "stm32mp13xx.h"

/* Use RAM1 (16 KB) for the memory regions. */
#define READONLY_REGION            (SRAM_BASE)
#define READWRITE_REGION           (SRAM_BASE + 0x1000)
#define SHARED_MEM_SIZE            0x1000 /* Each region is 4 KB (MMU page size). */

typedef enum {
  PROCESSING_NOT_STARTED    = 99,
  WRITING_TO_READWRITE      = 88,
  WRITING_TO_READONLY       = 77,
  READING_FROM_READWRITE    = 66,
  READING_FROM_READONLY     = 55,
  PROCESSING_FINISHED       = 44
} ProgressState;

#endif
