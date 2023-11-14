/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         DDR/DDR_Init/Src/stm32mp13xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */
#define STPMIC1_DEFAULT_START_UP_DELAY_MS  1

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

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/* USER CODE BEGIN 1 */

/**
  * @brief  This function handles System Power configuration.
  * @param  DDR type
  * @retval
  *  0: Test passed
  *  Value different from 0: Test failed
  */
int HAL_DDR_MspInit(ddr_type type)
{
  uint16_t buck2_mv = 1350;

/* For DK Board : DDR type is STM32MP_DDR3 */
  if (type ==  STM32MP_DDR3)
  {
    STPMU1_Regulator_Voltage_Set(STPMU1_BUCK2, buck2_mv);
    STPMU1_Regulator_Enable(STPMU1_BUCK2);
    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);
    STPMU1_Regulator_Enable(STPMU1_VREFDDR);
    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);
  }
  return 0;
}

/* USER CODE END 1 */
