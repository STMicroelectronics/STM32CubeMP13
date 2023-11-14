/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         FSBLA//FSBLA_Sdmmc1/Src/stm32mp13xx_hal_msp.c
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
#if (USE_STPMIC1x)
#define STPMIC1_DEFAULT_START_UP_DELAY_MS  1
#endif
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

#if (USE_STPMIC1x)
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

  switch (type) {
  case STM32MP_DDR3:
    STPMU1_Regulator_Voltage_Set(STPMU1_BUCK2, buck2_mv);
    STPMU1_Regulator_Enable(STPMU1_BUCK2);
    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);
    STPMU1_Regulator_Enable(STPMU1_VREFDDR);
    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);
    break;
  case STM32MP_LPDDR2_16:
  case STM32MP_LPDDR2_32:
  case STM32MP_LPDDR3_16:
  case STM32MP_LPDDR3_32:
    /* Set BUCK2 to 1.2V (16 bits) or 1.25V (32 bits) */
    switch (type)
    {
    case STM32MP_LPDDR2_32:
    case STM32MP_LPDDR3_32:
      buck2_mv = 1250;
      break;
    default:
    case STM32MP_LPDDR2_16:
    case STM32MP_LPDDR3_16:
      buck2_mv = 1200;
      break;
    }
    STPMU1_Regulator_Voltage_Set(STPMU1_BUCK2, buck2_mv);
    STPMU1_Regulator_Enable(STPMU1_BUCK2);
    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);
    STPMU1_Regulator_Enable(STPMU1_VREFDDR);
    HAL_Delay(STPMIC1_DEFAULT_START_UP_DELAY_MS);
    break;
  default:
    break;
  }
  return 0;
}
#endif

void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
  GPIO_InitTypeDef GPIO_Init_Structure;

  /* Enable SDMMC Clock */
  __HAL_RCC_SDMMC1_CLK_ENABLE();
  /* Force the SDMMC Periheral Clock Reset */
  __HAL_RCC_SDMMC1_FORCE_RESET();
  /* Release the SDMMC Periheral Clock Reset */
  __HAL_RCC_SDMMC1_RELEASE_RESET();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Common GPIO configuration */
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;

  /* Common GPIO configuration */
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;

  /* D0 D1 D2 D3 CK on PC8 PC9 PC10 PC11 PC12 - AF12 NOPULL*/
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Alternate = GPIO_AF12_SDIO1;
  GPIO_Init_Structure.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

  /* CMD on PD2 - AF12 NOPULL*/
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Alternate = GPIO_AF12_SDIO1;
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* CKIN on PB15 - AF8 NOPULL*/
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Alternate = GPIO_AF8_SDIO1;
  GPIO_Init_Structure.Pin       = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  /* D4 on PB14 - AF11 NOPULL*/
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Alternate = GPIO_AF11_SDIO1;
  GPIO_Init_Structure.Pin       = GPIO_PIN_14;
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  /* D5 on PB12 - AF12 NOPULL*/
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Alternate = GPIO_AF12_SDIO1;
  GPIO_Init_Structure.Pin       = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  /* D6 on PC6 - AF8 NOPULL*/
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Alternate = GPIO_AF8_SDIO1;
  GPIO_Init_Structure.Pin       = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

  /* D7 on PC7 - AF12 NOPULL*/
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO1;
  GPIO_Init_Structure.Pin       = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);


  /* Enable configuration for SDMMC interrupts */
  IRQ_SetPriority(SDMMC1_IRQn, 0x00);
  IRQ_Enable(SDMMC1_IRQn);
}


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
