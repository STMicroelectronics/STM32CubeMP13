/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         mmu_stm32mp13xx_azurertos.h
  * @brief        MMU support for ThreadX Module Manager
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
#ifndef MMU_STM32MP13XX_AZURETOS_H
#define MMU_STM32MP13XX_AZURETOS_H

#include "stm32mp13xx.h"
#include <stdint.h>

/*
 * Supported values for the "attributes" parameter of _txm_module_manager_external_memory_enable().
 */
#define mmu_readOnly 0 /* The memory can only be read by the module. */
#define mmu_readWrite 1 /* The memory can be read and written by the module. */
#define mmu_peripheral 2 /* The memory can be accessed as a peripheral (strongly ordered) by the module. */

/* Initialize the MMU for use with ThreadX. It must be called before any ThreadX function. */
int mmu_initialize();

#endif
