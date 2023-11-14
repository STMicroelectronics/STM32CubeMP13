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

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==UART8)
  {
  /* USER CODE BEGIN UART8_MspInit 0 */
    RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;

    /*##-1- Configure the UART clock source #*/
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART78;
  	RCC_PeriphCLKInitStruct.Uart78ClockSelection = RCC_UART78CLKSOURCE_HSI;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  /* USER CODE END UART8_MspInit 0 */

    /* Peripheral clock enable */
    __HAL_RCC_UART8_CLK_ENABLE();


    /**UART8 GPIO Configuration
    PF9     ------> UART8_RX
    PE1     ------> UART8_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART8;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART8;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);


    /* UART8 interrupt Init */
    IRQ_SetPriority(UART8_IRQn, 0);
    IRQ_Enable(UART8_IRQn);
  /* USER CODE BEGIN UART8_MspInit 1 */

  /* USER CODE END UART8_MspInit 1 */
  }

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==UART8)
  {
  /* USER CODE BEGIN UART8_MspDeInit 0 */

  /* USER CODE END UART8_MspDeInit 0 */

  /* Peripheral clock disable */
  __HAL_RCC_UART8_CLK_DISABLE();


    /**UART8 GPIO Configuration
    PF9     ------> UART8_RX
    PE1     ------> UART8_TX
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_1);
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_9);


    /* UART8 interrupt DeInit */
    IRQ_Disable(UART8_IRQn);
  /* USER CODE BEGIN UART8_MspDeInit 1 */

  /* USER CODE END UART8_MspDeInit 1 */
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

