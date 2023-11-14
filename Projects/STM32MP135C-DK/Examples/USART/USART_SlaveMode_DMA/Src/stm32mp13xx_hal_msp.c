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
  * Copyright (c) 2022 STMicroelectronics.
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

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

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

/**
* @brief USART MSP Initialization
* This function configures the hardware resources used in this example
* @param husart: USART handle pointer
* @retval None
*/
void HAL_USART_MspInit(USART_HandleTypeDef* husart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(husart->Instance==USART1)
  {
    /* USER CODE BEGIN USART1_MspInit 0 */
    /* USER CODE END USART1_MspInit 0 */

    /* At reset, peripherals are secure by default */
#if !defined AF_SECURE_USART_TEST
    /* Unsecure USART1 so that it can be used with DMA2 (which is always unsecure). */
    LL_ETZPC_Set_USART1_PeriphProtection(ETZPC, ETZPC_PERIPH_PROTECTION_READ_WRITE_NONSECURE);
#endif

    /**##-1- Configure the USART clock source #*/
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    /**
    USART1 GPIO Configuration

    We need USART1_TX, USART1_RX and USART1_CK for this example.

    Possible USART1 pins among all chip pins:
    USART1_TX: PA9 AF7, PB6  AF4, PB14 AF4, PC0 AF7
    USART1_RX: PB0 AF4, PD13 AF7, PD14 AF7, PG6 AF4
    USART1_CK: PA6 AF7, PA8  AF7, PB0  AF7

    Possible USART1 pins among exposed GPIO pins on board:
    USART1_TX: PC0 AF7
    USART1_RX: PB0 AF4, PD13 AF7
    USART1_CK: PA8 AF7, PB0  AF7

    Chosen pins:
    PC0  - AF7 - EXP_GPIO14 ------> USART1_TX
    PD13 - AF7 - EXP_GPIO6  ------> USART1_RX
    PB0  - AF7 - EXP_GPIO15 ------> USART1_CK
    */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

#if !defined AF_SECURE_USART_TEST
    /* Unsecure USART1 GPIO */
    GPIOC->SECCFGR &= ~GPIO_PIN_0;
    GPIOD->SECCFGR &= ~GPIO_PIN_13;
    GPIOB->SECCFGR &= ~GPIO_PIN_0;
#endif

    /* USART1_TX: PC0  - AF7 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART1_RX: PD13 - AF7 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART1_CK: PB0 - AF7 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    IRQ_SetPriority(USART1_IRQn, 0);
    IRQ_Enable(USART1_IRQn);

    /* USART1 DMA Init */

    /* USART1 RX Init */
#if defined AF_SECURE_USART_TEST
    /*
     * For secure access:
     * - DMA3 must be used because the other DMAs cannot be secure
     * - DMA_REQUEST_SEC_USART1_RX must be used (and not DMA_REQUEST_USART1_RX)
     */
    hdma_usart1_rx.Instance = DMA3_Stream0;
    hdma_usart1_rx.Init.Request = DMA_REQUEST_SEC_USART1_RX;
#else
    /*
     * For unsecure access:
     * - DMA1 or DMA2 can be used but not DMA3 because DMA3 is always secure. Here we use DMA2.
     * - DMA_REQUEST_USART1_RX must be used (and not DMA_REQUEST_SEC_USART1_RX)
     */
    hdma_usart1_rx.Instance = DMA2_Stream0;
    hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
#endif
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(husart,hdmarx,hdma_usart1_rx);

    /* USART1 TX Init */
#if defined AF_SECURE_USART_TEST
    /*
     * For secure access:
     * - DMA3 must be used because the other DMAs cannot be secure
     * - DMA_REQUEST_SEC_USART1_TX must be used (and not DMA_REQUEST_USART1_TX)
     */
    hdma_usart1_tx.Instance = DMA3_Stream1;
    hdma_usart1_tx.Init.Request = DMA_REQUEST_SEC_USART1_TX;
#else
    /*
     * For unsecure access:
     * - DMA1 or DMA2 can be used but not DMA3 because DMA3 is always secure. Here we use DMA2.
     * - DMA_REQUEST_USART1_TX must be used (and not DMA_REQUEST_SEC_USART1_TX)
     */
    hdma_usart1_tx.Instance = DMA2_Stream1;
    hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
#endif
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(husart,hdmatx,hdma_usart1_tx);

  /* USER CODE BEGIN USART1_MspInit 1 */
  /* USER CODE END USART1_MspInit 1 */
  }
}

/**
* @brief USART MSP De-Initialization
* This function de-initializes the hardware resources used in this example
* @param husart: USART handle pointer
* @retval None
*/
void HAL_USART_MspDeInit(USART_HandleTypeDef* husart)
{
  if(husart->Instance==USART1)
  {
    /* USER CODE BEGIN USART1_MspDeInit 0 */
    /* USER CODE END USART1_MspDeInit 0 */

    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /*
    USART1 GPIO Configuration:
    PC0  - AF7 - EXP_GPIO14 ------> USART1_TX
    PD13 - AF7 - EXP_GPIO6  ------> USART1_RX
    PB0  - AF7 - EXP_GPIO15 ------> USART1_CK
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_13);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(husart->hdmarx);
    HAL_DMA_DeInit(husart->hdmatx);

    /* USART1 interrupt DeInit */
    IRQ_Disable(USART1_IRQn);

    /* USER CODE BEGIN USART1_MspDeInit 1 */
    /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
