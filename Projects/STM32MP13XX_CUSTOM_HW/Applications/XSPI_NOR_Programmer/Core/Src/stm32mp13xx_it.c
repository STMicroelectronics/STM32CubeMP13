/**
  ******************************************************************************
  * @file    stm32mp13xx_it.c 
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  *
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32mp13xx_it.h"

/** @addtogroup STM32MP13xx_HAL_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern XSPI_HandleTypeDef hxspi;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/* STM32MP13xx Peripheral Interrupt Handlers                                  */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please 					  */
/******************************************************************************/

/**
* @brief This function handles XSPI global interrupt.
*/
void XSPI_IRQHandler(void)
{
  /* USER CODE BEGIN XSPI_IRQn 0 */

  /* USER CODE END XSPI_IRQn 0 */
  HAL_XSPI_IRQHandler(&hxspi);
  /* USER CODE BEGIN XSPI_IRQn 1 */

  /* USER CODE END XSPI_IRQn 1 */
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */ 


