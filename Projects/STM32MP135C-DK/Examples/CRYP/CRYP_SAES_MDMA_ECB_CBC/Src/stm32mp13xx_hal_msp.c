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

/*MDMA initialisation */
MDMA_HandleTypeDef hmdmaIn;
MDMA_HandleTypeDef hmdmaOut;

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
  /* USER CODE BEGIN CRYP_MspDeInit 0 */

  /* USER CODE END CRYP_MspDeInit 0 */
  /* Peripheral clock disable */
  __HAL_RCC_SAES_CLK_ENABLE();
  /* USER CODE BEGIN CRYP_MspDeInit 1 */
  __HAL_RCC_MDMA_CLK_ENABLE();

  /* Set hmdmaIn and hmdmaOut memory block at 0 */
  memset(&hmdmaIn,  0, sizeof(MDMA_HandleTypeDef));
  memset(&hmdmaOut, 0, sizeof(MDMA_HandleTypeDef));

 /***************** Configure common MDMA In parameters ***********************/
  hmdmaIn.Init.Request              = MDMA_REQUEST_SAES_IN;
  hmdmaIn.Init.TransferTriggerMode  = MDMA_BUFFER_TRANSFER;
  hmdmaIn.Init.Priority             = MDMA_PRIORITY_LOW;
  hmdmaIn.Init.SecureMode           = MDMA_SECURE_MODE_ENABLE;
  hmdmaIn.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdmaIn.Init.SourceInc            = MDMA_SRC_INC_WORD;
  hmdmaIn.Init.DestinationInc       = MDMA_DEST_INC_DISABLE;
  hmdmaIn.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  hmdmaIn.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;
  hmdmaIn.Init.DataAlignment        = MDMA_DATAALIGN_RIGHT;
  hmdmaIn.Init.BufferTransferLength = 16;
  hmdmaIn.Init.SourceBurst          = MDMA_SOURCE_BURST_4BEATS;
  hmdmaIn.Init.DestBurst            = MDMA_DEST_BURST_4BEATS;
  hmdmaIn.Instance  = MDMA_Channel_SAES_IN;

  /* Deinitialize the Stream for new transfer */
  HAL_MDMA_DeInit(hcryp->hmdmain);

  /* Associate the DMA handle */
  __HAL_LINKDMA(hcryp, hmdmain, hmdmaIn);

  /* Configure the MDMA Stream */
  HAL_MDMA_Init(hcryp->hmdmain);

  /***************** Configure common MDMA Out parameters **********************/
  hmdmaOut.Init.Request              = MDMA_REQUEST_SAES_OUT;
  hmdmaOut.Init.TransferTriggerMode  = MDMA_BUFFER_TRANSFER;
  hmdmaOut.Init.Priority             = MDMA_PRIORITY_HIGH;
  hmdmaOut.Init.SecureMode           = MDMA_SECURE_MODE_ENABLE;
  hmdmaOut.Init.Endianness           = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdmaOut.Init.SourceInc            = MDMA_SRC_INC_DISABLE;
  hmdmaOut.Init.DestinationInc       = MDMA_DEST_INC_WORD;
  hmdmaOut.Init.SourceDataSize       = MDMA_SRC_DATASIZE_WORD;
  hmdmaOut.Init.DestDataSize         = MDMA_DEST_DATASIZE_WORD;
  hmdmaOut.Init.DataAlignment        = MDMA_DATAALIGN_RIGHT;
  hmdmaOut.Init.BufferTransferLength = 16;
  hmdmaOut.Init.SourceBurst          = MDMA_SOURCE_BURST_4BEATS;
  hmdmaOut.Init.DestBurst            = MDMA_DEST_BURST_4BEATS;
  hmdmaOut.Instance = MDMA_Channel_SAES_OUT;

  /* Deinitialize the Stream for new processing */
  HAL_MDMA_DeInit(&hmdmaOut);

  /* Associate the DMA handle */
  __HAL_LINKDMA(hcryp, hmdmaout, hmdmaOut);

  /* Configure the DMA Stream */
  HAL_MDMA_Init(&hmdmaOut);

  /* GIC configuration for MDMA Input data interrupt */
  IRQ_SetPriority(MDMA_SEC_IT_IRQn, 0x01);
  IRQ_Enable(MDMA_SEC_IT_IRQn);

  /* GIC configuration for SAES Input data interrupt */
  IRQ_SetPriority(SAES_IRQn, 0x00);
  IRQ_Enable(SAES_IRQn);
  /* USER CODE END CRYP_MspDeInit 1 */
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
  if(hrng->Instance==RNG1)
  {
  /* USER CODE BEGIN RNG_MspInit 0 */

  /* USER CODE END RNG_MspInit 0 */

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

