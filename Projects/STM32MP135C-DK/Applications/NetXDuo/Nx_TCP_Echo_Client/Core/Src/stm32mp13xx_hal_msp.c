/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file         stm32mp13xx_hal_msp.c
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
}

/**
 * @brief  Initializes the ETH MSP.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */

void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  BSP_IO_Init_t io_init_structure;

  if(heth->Instance==ETH)
  {
    /* Ethernet MSP init:

  REF_CLK  -------------> PA1
  TXD0    --------------> PG13
  TXD1    --------------> PG14
  RXD0    --------------> PC4
  RXD1    --------------> PC5
  TX_EN   --------------> PB11
  CRS_DV  --------------> PC1
  MDC     --------------> PG2
  MDIO    --------------> PA2

     */

    /* Enable GPIOs clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* Configure PA1, PA2 */
    GPIO_InitStructure.Pin =  GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PB11 */
    GPIO_InitStructure.Pin = GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PC1 */
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.Alternate = GPIO_AF10_ETH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PC4 and PC5 */
    GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PG2, PG13 and PG14 */
    GPIO_InitStructure.Pin = GPIO_PIN_2 | GPIO_PIN_13 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* Configure ETH1_NRST pin */
    io_init_structure.Pin  = MCP23x17_GPIO_PIN_9;
    io_init_structure.Pull = IO_NOPULL;
    io_init_structure.Mode = IO_MODE_OUTPUT_PP;
    BSP_IO_Init(0, &io_init_structure);
    BSP_IO_WritePin(0, MCP23x17_GPIO_PIN_9, IO_PIN_SET);

    /* Configure IRQ */
    IRQ_SetPriority(ETH1_IRQn, 7);
    IRQ_Enable(ETH1_IRQn);

    /* Enable Ethernet clocks */
    __HAL_RCC_ETH1CK_CLK_ENABLE();
    __HAL_RCC_ETH1MAC_CLK_ENABLE();
    __HAL_RCC_ETH1TX_CLK_ENABLE();
    __HAL_RCC_ETH1RX_CLK_ENABLE();
  }

}

/**
 * @brief  DeInitializes ETH MSP.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{

  if(heth->Instance==ETH)
  {
    __HAL_RCC_ETH1CK_CLK_DISABLE();
    __HAL_RCC_ETH1MAC_CLK_DISABLE();
    __HAL_RCC_ETH1TX_CLK_DISABLE();
    __HAL_RCC_ETH1RX_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_2);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_2 | GPIO_PIN_13 | GPIO_PIN_14);
    BSP_IO_DeInit(0);

    IRQ_Disable(ETH1_IRQn);
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

