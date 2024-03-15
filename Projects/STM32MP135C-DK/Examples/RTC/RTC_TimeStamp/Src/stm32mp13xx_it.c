/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32mp13xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32mp13xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern RTC_HandleTypeDef RtcHandle;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/* STM32MP13xx Peripheral Interrupt Handlers                                  */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32mp135c_ca7.c).              */
/******************************************************************************/

/* USER CODE BEGIN 1 */
/**
  * @brief  This function handles Tamper interrupt request.
  * @param  None
  * @retval None
  */
#if defined(CORTEX_IN_SECURE_STATE)
void TAMP_SIRQHandler(void)
{
  /* Catch immediately the timestamp event */
  if (HAL_OK != HAL_RTCEx_PollForTimeStampEvent(&RtcHandle, HAL_MAX_DELAY))
  {
    Error_Handler();
  }
  HAL_RTCEx_TamperIRQHandler(&RtcHandle);
}
#else /* CORTEX_IN_SECURE_STATE */
void TAMP_IRQHandler(void)
{
  /* Catch immediately the timestamp event */
  if (HAL_OK != HAL_RTCEx_PollForTimeStampEvent(&RtcHandle, HAL_MAX_DELAY))
  {
    Error_Handler();
  }
  HAL_RTCEx_TamperIRQHandler(&RtcHandle);
}
#endif /* CORTEX_IN_SECURE_STATE */

/* USER CODE END 1 */
