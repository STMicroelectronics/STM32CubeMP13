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

extern MDMA_HandleTypeDef hmdma_hash_in;

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

void HAL_HASH_MspInit(HASH_HandleTypeDef* hhash)
{

  /* USER CODE BEGIN HASH_MspInit 0 */

  /* USER CODE END HASH_MspInit 0 */
  /* Peripheral clock enable */
  __HAL_RCC_HASH1_CLK_ENABLE();

  /* Peripheral MDMA init*/
  hmdma_hash_in.Instance                  = MDMA_Channel15;
  hmdma_hash_in.Init.Request              = MDMA_REQUEST_HASH1_IN;
  hmdma_hash_in.Init.TransferTriggerMode  = MDMA_BLOCK_TRANSFER;
  hmdma_hash_in.Init.Priority             = MDMA_PRIORITY_HIGH;
  hmdma_hash_in.Init.SecureMode           = MDMA_SECURE_MODE_ENABLE;
  hmdma_hash_in.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdma_hash_in.Init.SourceInc            = MDMA_SRC_INC_WORD;
  hmdma_hash_in.Init.DestinationInc       = MDMA_DEST_INC_DISABLE;
  hmdma_hash_in.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  hmdma_hash_in.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;
  hmdma_hash_in.Init.DataAlignment        = MDMA_DATAALIGN_RIGHT;
  hmdma_hash_in.Init.BufferTransferLength = 16;
  hmdma_hash_in.Init.SourceBurst          = MDMA_SOURCE_BURST_SINGLE;
  hmdma_hash_in.Init.DestBurst            = MDMA_DEST_BURST_SINGLE;

  if (HAL_MDMA_DeInit(&hmdma_hash_in) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_MDMA_Init(&hmdma_hash_in) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_LINKDMA(hhash,hmdmain,hmdma_hash_in);

  /* USER CODE BEGIN HASH_MspInit 1 */

  /* USER CODE END HASH_MspInit 1 */

}

void HAL_HASH_MspDeInit(HASH_HandleTypeDef* hhash)
{

  /* USER CODE BEGIN HASH_MspDeInit 0 */

  /* USER CODE END HASH_MspDeInit 0 */

    /* Reset HASH peripheral */
    __HAL_RCC_HASH1_FORCE_RESET();
    __HAL_RCC_HASH1_RELEASE_RESET();

    /* Peripheral clock disable */
    __HAL_RCC_HASH1_CLK_DISABLE();

    /* Peripheral MDMA DeInit*/
    HAL_MDMA_DeInit(hhash->hmdmain);

  /* USER CODE BEGIN HASH_MspDeInit 1 */

  /* USER CODE END HASH_MspDeInit 1 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

