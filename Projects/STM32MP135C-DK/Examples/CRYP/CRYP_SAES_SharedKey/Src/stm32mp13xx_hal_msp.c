/* USER CODE BEGIN Header */
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


extern void Error_Handler(void);
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

void HAL_CRYP_MspInit(CRYP_HandleTypeDef* hcryp)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hcryp->Instance==SAES)
  {
    /* USER CODE BEGIN CRYP_MspDeInit 0 */

    /* USER CODE END CRYP_MspDeInit 0 */
    /** Initializes the peripherals clock
    */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAES;
    PeriphClkInit.SaesClockSelection = RCC_SAESCLKSOURCE_ACLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_SAES_CLK_ENABLE();
    /* USER CODE BEGIN CRYP_MspDeInit 1 */
    /* Enable  CRYP1 clock */
    __HAL_RCC_CRYP1_CLK_ENABLE();

    HAL_Delay(10);
    /* USER CODE END CRYP_MspDeInit 1 */
  }
}

void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef* hcryp)
{
  /* USER CODE BEGIN CRYP_MspDeInit 0 */

  /* USER CODE END CRYP_MspDeInit 0 */
  /* Peripheral clock disable */
  __HAL_RCC_SAES_CLK_DISABLE();
  /* USER CODE BEGIN CRYP_MspDeInit 1 */

  /* USER CODE END CRYP_MspDeInit 1 */
}

/**
* @brief RNG MSP Initialization
* This function configures the hardware resources used in this example
* @param hrng: RNG handle pointer
* @retval None
*/
void HAL_RNG_MspInit(RNG_HandleTypeDef* hrng)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hrng->Instance==RNG1)
  {
    /* USER CODE BEGIN RNG_MspInit 0 */

    /* USER CODE END RNG_MspInit 0 */

    /** Initializes the peripherals clock
    */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RNG1;
    PeriphClkInit.Rng1ClockSelection = RCC_RNG1CLKSOURCE_LSI;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_RNG1_CLK_ENABLE();
    /* USER CODE BEGIN RNG_MspInit 1 */

    /* USER CODE END RNG_MspInit 1 */
   }
}

/**
* @brief RNG MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hrng: RNG handle pointer
* @retval None
*/
void HAL_RNG_MspDeInit(RNG_HandleTypeDef* hrng)
{
  if(hrng->Instance==RNG1)
  {
  /* USER CODE BEGIN RNG_MspDeInit 0 */

  /* USER CODE END RNG_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RNG1_CLK_DISABLE();
  /* USER CODE BEGIN RNG_MspDeInit 1 */

  /* USER CODE END RNG_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

