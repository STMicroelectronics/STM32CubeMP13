/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Core/Src/main.c
  * @author  MCD Application Team
  * @brief   USB device CDC demo main file
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart4;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initialize the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
 
#if (USE_STPMIC1x) && !defined(USE_DDR)
  /* Configure PMIC */
  BSP_PMIC_Init();
  BSP_PMIC_InitRegulators();
#endif


  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USB_Device_Init();
  /* USER CODE BEGIN 2 */
  BSP_LED_Init(LED_BLUE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
#if !defined(USE_DDR)
  HAL_RCC_DeInit();
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    RCC_OscInitTypeDef RCC_OscInitStructure;

    /* Enable all available oscillators*/
    RCC_OscInitStructure.OscillatorType = (RCC_OSCILLATORTYPE_HSI |
                                           RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_CSI |
                                           RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE);

    RCC_OscInitStructure.HSIState = RCC_HSI_ON;
    RCC_OscInitStructure.HSEState = RCC_HSE_ON;
    RCC_OscInitStructure.LSEState = RCC_LSE_ON;
    RCC_OscInitStructure.LSIState = RCC_LSI_ON;
    RCC_OscInitStructure.CSIState = RCC_CSI_ON;

    RCC_OscInitStructure.HSICalibrationValue = 0x00; //Default reset value
    RCC_OscInitStructure.CSICalibrationValue = 0x10; //Default reset value
    RCC_OscInitStructure.HSIDivValue = RCC_HSI_DIV1; //Default value

    /* PLL configuration */
    RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStructure.PLL.PLLSource = RCC_PLL12SOURCE_HSE;
    RCC_OscInitStructure.PLL.PLLM = 3;
    RCC_OscInitStructure.PLL.PLLN = 81;
    RCC_OscInitStructure.PLL.PLLP = 1;
    RCC_OscInitStructure.PLL.PLLQ = 2;
    RCC_OscInitStructure.PLL.PLLR = 2;
    RCC_OscInitStructure.PLL.PLLFRACV = 0x800;
    RCC_OscInitStructure.PLL.PLLMODE = RCC_PLL_FRACTIONAL;

    RCC_OscInitStructure.PLL2.PLLState = RCC_PLL_ON;
    RCC_OscInitStructure.PLL2.PLLSource = RCC_PLL12SOURCE_HSE;
    RCC_OscInitStructure.PLL2.PLLM = 3;
    RCC_OscInitStructure.PLL2.PLLN = 66;
    RCC_OscInitStructure.PLL2.PLLP = 2;
    RCC_OscInitStructure.PLL2.PLLQ = 2;
    RCC_OscInitStructure.PLL2.PLLR = 1;
    RCC_OscInitStructure.PLL2.PLLFRACV = 0x1400;
    RCC_OscInitStructure.PLL2.PLLMODE = RCC_PLL_FRACTIONAL;

    RCC_OscInitStructure.PLL3.PLLState = RCC_PLL_ON;
    RCC_OscInitStructure.PLL3.PLLSource = RCC_PLL3SOURCE_HSE;
    RCC_OscInitStructure.PLL3.PLLM = 2;
    RCC_OscInitStructure.PLL3.PLLN = 34;
    RCC_OscInitStructure.PLL3.PLLP = 2;
    RCC_OscInitStructure.PLL3.PLLQ = 17;
    RCC_OscInitStructure.PLL3.PLLR = 2;
    RCC_OscInitStructure.PLL3.PLLRGE = RCC_PLL3IFRANGE_1;
    RCC_OscInitStructure.PLL3.PLLFRACV = 0x1a04;
    RCC_OscInitStructure.PLL3.PLLMODE = RCC_PLL_FRACTIONAL;

    RCC_OscInitStructure.PLL4.PLLState = RCC_PLL_ON;
    RCC_OscInitStructure.PLL4.PLLSource = RCC_PLL4SOURCE_HSE;
    RCC_OscInitStructure.PLL4.PLLM = 2;
    RCC_OscInitStructure.PLL4.PLLN = 50;
    RCC_OscInitStructure.PLL4.PLLP = 12;
    RCC_OscInitStructure.PLL4.PLLQ = 60;
    RCC_OscInitStructure.PLL4.PLLR = 6;
    RCC_OscInitStructure.PLL4.PLLRGE = RCC_PLL4IFRANGE_1;
    RCC_OscInitStructure.PLL4.PLLFRACV = 0;
    RCC_OscInitStructure.PLL4.PLLMODE = RCC_PLL_INTEGER;

    /* Enable access to RTC and backup registers */
    SET_BIT(PWR->CR1, PWR_CR1_DBP);
    /* Configure LSEDRIVE value */
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_MEDIUMHIGH);

    if (HAL_RCC_OscConfig(&RCC_OscInitStructure) != HAL_OK) {
      /* HAL RCC configuration error */
      Error_Handler();
      }

    /* Select PLLx as MPU, AXI and MCU clock sources */
    RCC_ClkInitStructure.ClockType = (RCC_CLOCKTYPE_MPU   | RCC_CLOCKTYPE_ACLK  |
                                      RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_PCLK4 |
                                      RCC_CLOCKTYPE_PCLK5 | RCC_CLOCKTYPE_PCLK1 |
                                      RCC_CLOCKTYPE_PCLK6 |
                                      RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);

    RCC_ClkInitStructure.MPUInit.MPU_Clock = RCC_MPUSOURCE_PLL1;
    RCC_ClkInitStructure.MPUInit.MPU_Div = RCC_MPU_DIV2;
    RCC_ClkInitStructure.AXISSInit.AXI_Clock = RCC_AXISSOURCE_PLL2;
    RCC_ClkInitStructure.AXISSInit.AXI_Div = RCC_AXI_DIV1;
    RCC_ClkInitStructure.MLAHBInit.MLAHB_Clock = RCC_MLAHBSSOURCE_PLL3;
    RCC_ClkInitStructure.MLAHBInit.MLAHB_Div = RCC_MLAHB_DIV1;
    RCC_ClkInitStructure.APB1_Div = RCC_APB1_DIV2;
    RCC_ClkInitStructure.APB2_Div = RCC_APB2_DIV2;
    RCC_ClkInitStructure.APB3_Div = RCC_APB3_DIV2;
    RCC_ClkInitStructure.APB4_Div = RCC_APB4_DIV2;
    RCC_ClkInitStructure.APB5_Div = RCC_APB5_DIV4;
    RCC_ClkInitStructure.APB6_Div = RCC_APB6_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStructure) != HAL_OK) {
      /* HAL RCC configuration error */
      Error_Handler();
      }
#endif
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = (4000000/10000) - 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000 - 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
void MX_UART4_UART_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Unsecure SYSRAM so that DMA1 (which is always non-secure) can access it. */
  LL_ETZPC_SetSecureSysRamSize(ETZPC, 0);

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  IRQ_SetPriority(DMA1_Stream0_IRQn, 6);
  IRQ_Enable(DMA1_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/**
  * @brief This function provides accurate delay (in milliseconds) based
  * on SysTick counter flag.
  * @note This function is declared as __weak to be overwritten in case of other
  * implementations in user file.
  * @param Delay: specifies the delay time length, in milliseconds.
  * @retval None
  */
/*void HAL_Delay(__IO uint32_t Delay)
{
  while (Delay)
  {
    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
    {
      Delay--;
    }
  }
}*/
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  BSP_LED_On(LED_BLUE);
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {}
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

