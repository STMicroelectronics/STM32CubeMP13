/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    SD/SD_ReadWrite_DMALinkedList/Src/stm32mp13xx_hal_msp.c
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

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

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
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/* USER CODE BEGIN 1 */
/**
  * @brief SD MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for requests by peripheral
  *           - interrupt configuration for DMA and SD interrupts
  * @param hsd: SD handle pointer
  * @retval None
  */

void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc)
{
  GPIO_InitTypeDef GPIO_Init_Structure = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  if(hmmc->Instance==SDMMC2)
  {
  /* USER CODE BEGIN SDMMC1_MspInit 0 */

  /* USER CODE END SDMMC1_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC2;
    PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC2CLKSOURCE_PLL4;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }


    /* Enable SDMMC Clock */
    __HAL_RCC_SDMMC2_CLK_ENABLE();
    /* Force the SDMMC Periheral Clock Reset */
    __HAL_RCC_SDMMC2_FORCE_RESET();
    /* Release the SDMMC Periheral Clock Reset */
    __HAL_RCC_SDMMC2_RELEASE_RESET();

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* Common GPIO configuration */
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;

    /* Common GPIO configuration */
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;

    /* D0 D1 D2 D3 CK on PC8 PC9 PC10 PC11 PC12 - AF12 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

    /* CMD on PD2 - AF12 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

    /* D5 on PB12 - AF12 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_9 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

    /* D6 on PC6 - AF8 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

    /* D7 on PC7 - AF12 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* USER CODE BEGIN SDMMC1_MspInit 1 */

  /* USER CODE END SDMMC1_MspInit 1 */
  }
}

/**
  * @brief SD MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and interrupt configuration to their default state
  * @param hsd: SD handle pointer
  * @retval None
  */
void HAL_MMC_MspDeInit(MMC_HandleTypeDef *hmmc)
{

  if(hmmc->Instance==SDMMC2)
  {
  /* USER CODE BEGIN SDMMC1_MspDeInit 0 */

  /* USER CODE END SDMMC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDMMC2_CLK_DISABLE();

    /**SDMMC1 GPIO Configuration
    PC11     ------> SDMMC1_D3
    PC10     ------> SDMMC1_D2
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD
    PC9     ------> SDMMC1_D1
    PC8     ------> SDMMC1_D0
    */
    HAL_GPIO_DeInit(GPIOB, (GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_9 | GPIO_PIN_14 | GPIO_PIN_15));

    HAL_GPIO_DeInit(GPIOC, (GPIO_PIN_6 | GPIO_PIN_7));

    HAL_GPIO_DeInit(GPIOE, (GPIO_PIN_3));

    HAL_GPIO_DeInit(GPIOF, (GPIO_PIN_0));

    HAL_GPIO_DeInit(GPIOG, (GPIO_PIN_6));

  /* USER CODE BEGIN SDMMC1_MspDeInit 1 */

  /* USER CODE END SDMMC1_MspDeInit 1 */
  }
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

