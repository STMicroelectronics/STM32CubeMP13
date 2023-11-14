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
extern DMA_HandleTypeDef hdma_memtomem_dma3_stream0;

/******************************************************************************/
/* STM32MP13xx Peripheral Interrupt Handlers                                  */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32mp135c_ca7.c).              */
/******************************************************************************/

/**
* @brief This function handles DMA3 stream0 global interrupt.
*/
void DMA3_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA3_Stream0_IRQn 0 */

  /* USER CODE END DMA3_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_memtomem_dma3_stream0);
  /* USER CODE BEGIN DMA3_Stream0_IRQn 1 */

  /* USER CODE END DMA3_Stream0_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
