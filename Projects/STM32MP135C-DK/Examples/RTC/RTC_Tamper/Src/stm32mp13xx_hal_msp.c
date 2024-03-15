/**
  ******************************************************************************
  * @file    stm32mp13xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
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

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
static uint32_t RtcClockSource
#ifdef RTC_CLOCK_SOURCE_LSE
  = RCC_RTCCLKSOURCE_LSE;
#elif defined (RTC_CLOCK_SOURCE_LSI)
  = RCC_RTCCLKSOURCE_LSI;
#elif defined (RTC_CLOCK_SOURCE_HSE)
  = RCC_RTCCLKSOURCE_HSE_DIV;
#endif

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions ------------------------------------------------------- */
extern void Error_Handler(void);

/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

  /*##-1- Enables access to the backup domain ################################*/
  /* To enable access on RTC registers */
  HAL_PWR_EnableBkUpAccess();

  /*##-2- Get RTC clock configuration ########################################*/
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;

  /*##-3- Configure LSE/LSI/HSE as RTC clock source ##############################*/
#ifdef RTC_CLOCK_SOURCE_LSE
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);

#elif defined (RTC_CLOCK_SOURCE_LSI)
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSI, RCC_MCODIV_1);

#elif defined (RTC_CLOCK_SOURCE_HSE)
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  /*Division of RCC_RTCCLKSOURCE_HSE_DIV by 32*/
  __HAL_RCC_RTC_HSEDIV(32);
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV;
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
#endif

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.RTCClockSelection = RtcClockSource;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

#if defined (CORTEX_IN_SECURE_STATE)
  RTC_SecureStateTypeDef  secureState = {0};

  secureState.tampSecureFull = TAMP_SECURE_FULL_YES;
  (void)HAL_RTCEx_SecureModeSet(hrtc, &secureState);
#endif /* CORTEX_IN_SECURE_STATE */

  /*##-4- Configure the interrupt for RTC Tamper ##################################*/
#if defined (CORTEX_IN_SECURE_STATE)
  IRQ_SetPriority(TAMP_S_IRQn, 0x4);
  IRQ_Enable(TAMP_S_IRQn);
#else /* CORTEX_IN_SECURE_STATE */
  IRQ_SetPriority(TAMP_IRQn, 0x4);
  IRQ_Enable(TAMP_IRQn);
#endif /* CORTEX_IN_SECURE_STATE */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

