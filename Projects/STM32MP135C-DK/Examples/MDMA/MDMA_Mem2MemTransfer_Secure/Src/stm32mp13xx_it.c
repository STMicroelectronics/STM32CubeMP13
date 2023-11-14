/**
  ******************************************************************************
  * @file    stm32mp13xx_it.c
  * @author  MCD Application Team
  * @brief   Interrupt Service Routines.
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
#include "main.h"
#include "stm32mp13xx_it.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern MDMA_HandleTypeDef   hmdma_memtomem;

/******************************************************************************/
/* STM32MP13xx Peripheral Interrupt Handlers                                  */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32mp135c_ca7.c).              */
/******************************************************************************/

/**
* @brief This function handles MDMA global interrupt in SECURE mode.
*/

void MDMA_SEC_IT_IRQHandler(void)
{
  /* Check the interrupt and clear flag */
  HAL_MDMA_IRQHandler(&hmdma_memtomem);
}
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
