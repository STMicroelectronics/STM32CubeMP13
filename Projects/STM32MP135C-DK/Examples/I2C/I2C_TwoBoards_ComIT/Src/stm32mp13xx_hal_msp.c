/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32mp13xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   This file provides code for the MSP Initialization
  *                      and de-Initialization codes.
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

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C5)
  {
  /* USER CODE BEGIN I2C5_MspInit 0 */
    RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;

    /*##-1- Configure the I2C clock source #*/
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C5;
    RCC_PeriphCLKInitStruct.I2c5ClockSelection = RCC_I2C5CLKSOURCE_HSI;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  /* USER CODE END I2C5_MspInit 0 */

    /**I2C5 GPIO Configuration
    PD1     ------> I2C5_SCL
    PH6     ------> I2C5_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C5;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C5_CLK_ENABLE();
    /* I2C5 interrupt Init */
    IRQ_SetPriority(I2C5_EV_IRQn, 0);
    IRQ_Enable(I2C5_EV_IRQn);
    IRQ_SetPriority(I2C5_ER_IRQn, 0);
    IRQ_Enable(I2C5_ER_IRQn);
  /* USER CODE BEGIN I2C5_MspInit 1 */

  /* USER CODE END I2C5_MspInit 1 */
  }

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{

  if(hi2c->Instance==I2C5)
  {
  /* USER CODE BEGIN I2C5_MspDeInit 0 */

  /* USER CODE END I2C5_MspDeInit 0 */

  /* Peripheral clock disable */
  __HAL_RCC_I2C5_CLK_DISABLE();


    /**I2C5 GPIO Configuration
    PD1     ------> I2C5_SCL
    PH6     ------> I2C5_SDA
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_1);
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_6);

    /* I2C5 interrupt DeInit */
    IRQ_Disable(I2C5_EV_IRQn);
    IRQ_Disable(I2C5_ER_IRQn);
  /* USER CODE BEGIN I2C5_MspDeInit 1 */

  /* USER CODE END I2C5_MspDeInit 1 */
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

