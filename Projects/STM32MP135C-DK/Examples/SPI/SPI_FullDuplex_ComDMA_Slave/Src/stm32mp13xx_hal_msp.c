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

extern DMA_HandleTypeDef hdma_spi5_rx;
extern DMA_HandleTypeDef hdma_spi5_tx;

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
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct = {0};
  if(hspi->Instance==SPI5)
  {
    /* USER CODE BEGIN SPI5_MspInit 0 */
    /* USER CODE END SPI5_MspInit 0 */

    /* At reset, peripherals are secure by default */
#if !defined AF_SECURE_SPI5_TEST
    /* Unsecure SPI5 */
    LL_ETZPC_Set_SPI5_PeriphProtection(ETZPC, LL_ETZPC_PERIPH_PROTECTION_READ_WRITE_NONSECURE);
#endif

    /*##-1- Configure the SPI clock source #*/
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI5;
    RCC_PeriphCLKInitStruct.Spi5ClockSelection = RCC_SPI5CLKSOURCE_HSI;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_SPI5_CLK_ENABLE();

    /* GPIO Ports Clock Enable for used SPI5 pins */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

#if !defined AF_SECURE_SPI5_TEST
    /* Unsecure SPI5 GPIO */
    GPIOH->SECCFGR &= ~(GPIO_PIN_3|GPIO_PIN_7);
    GPIOA->SECCFGR &= ~GPIO_PIN_8;
#endif

    /*
    SPI5 GPIO Configuration
    PA8     ------> SPI5_MISO
    PH7     ------> SPI5_SCK
    PH3     ------> SPI5_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI5;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* SPI5 interrupt Init */
    IRQ_SetPriority(SPI5_IRQn, 0);
    IRQ_Enable(SPI5_IRQn);

    /* SPI5 DMA Init */

    /* SPI5 RX Init */
#if defined AF_SECURE_SPI5_TEST
    /*
     * For secure access:
     * - DMA3 must be used because the other DMAs cannot be secure
     * - DMA_REQUEST_SEC_SPI5_RX must be used (and not DMA_REQUEST_SPI5_RX)
     */
    hdma_spi5_rx.Instance = DMA3_Stream0;
    hdma_spi5_rx.Init.Request = DMA_REQUEST_SEC_SPI5_RX;
#else
    /*
     * For unsecure access:
     * - DMA1 or DMA2 can be used but not DMA3 because DMA3 is always secure. Here we use DMA2.
     * - DMA_REQUEST_SPI5_RX must be used (and not DMA_REQUEST_SEC_SPI5_RX)
     */
    hdma_spi5_rx.Instance = DMA2_Stream0;
    hdma_spi5_rx.Init.Request = DMA_REQUEST_SPI5_RX;
#endif
    hdma_spi5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi5_rx.Init.Mode = DMA_NORMAL;
    hdma_spi5_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_spi5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi5_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hspi,hdmarx,hdma_spi5_rx);

    /* SPI5 TX Init */
#if defined AF_SECURE_SPI5_TEST
    /*
     * For secure access:
     * - DMA3 must be used because the other DMAs cannot be secure
     * - DMA_REQUEST_SEC_SPI5_TX must be used (and not DMA_REQUEST_SPI5_TX)
     */
    hdma_spi5_tx.Instance = DMA3_Stream1;
    hdma_spi5_tx.Init.Request = DMA_REQUEST_SEC_SPI5_TX;
#else
    /*
     * For unsecure access:
     * - DMA1 or DMA2 can be used but not DMA3 because DMA3 is always secure. Here we use DMA2.
     * - DMA_REQUEST_SPI5_TX must be used (and not DMA_REQUEST_SEC_SPI5_TX)
     */
    hdma_spi5_tx.Instance = DMA2_Stream1;
    hdma_spi5_tx.Init.Request = DMA_REQUEST_SPI5_TX;
#endif
    hdma_spi5_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi5_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi5_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi5_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi5_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi5_tx.Init.Mode = DMA_NORMAL;
    hdma_spi5_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi5_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi5_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hspi,hdmatx,hdma_spi5_tx);

    /* USER CODE BEGIN SPI5_MspInit 1 */
    /* USER CODE END SPI5_MspInit 1 */
  }
}

/**
* @brief SPI MSP De-Initialization
* This function de-initializes the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI5)
  {
    /* USER CODE BEGIN SPI5_MspDeInit 0 */
    /* USER CODE END SPI5_MspDeInit 0 */

    /* Peripheral clock disable */
    __HAL_RCC_SPI5_CLK_DISABLE();

    /*
    SPI5 GPIO Configuration:
    PA8     ------> SPI5_MISO
    PH7     ------> SPI5_SCK
    PH3     ------> SPI5_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_3 | GPIO_PIN_7);

    /* SPI5 DMA DeInit */
    HAL_DMA_DeInit(hspi->hdmarx);
    HAL_DMA_DeInit(hspi->hdmatx);

    /* SPI5 interrupt DeInit */
    IRQ_Disable(SPI5_IRQn);

    /* USER CODE BEGIN SPI5_MspDeInit 1 */
    /* USER CODE END SPI5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
