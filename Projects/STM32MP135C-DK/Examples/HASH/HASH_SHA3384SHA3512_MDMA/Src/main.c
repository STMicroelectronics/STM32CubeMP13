/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main program body
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

/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

HASH_HandleTypeDef hhash;
MDMA_HandleTypeDef hmdma_hash_in;

/* USER CODE BEGIN PV */
__ALIGN_BEGIN const uint8_t aInput[] __ALIGN_END = "The STM32 MP1 series is the result of a perfect symbiosis of the real-time control capabilities of an MCU and the signal processing performance of a DSP, and thus complements the STM32 portfolio with a new class of devices, digital signal controllers (DSC).";
__ALIGN_BEGIN static uint8_t aSHA3_384Digest[48] __ALIGN_END;
__ALIGN_BEGIN static uint8_t aSHA3_512Digest[64] __ALIGN_END;
__ALIGN_BEGIN static uint8_t aExpectSHA3_384Digest[48] __ALIGN_END = { 0x2c, 0x52, 0xcf, 0x2d, 0x40, 0x8e, 0x02, 0xe0, 0x12, 0x6e, 0x66, 0x68, 0x8a,
                                                                       0x82, 0xf4, 0x55, 0xe1, 0x7c, 0x00, 0x66, 0x3b, 0xf0, 0x3a, 0xe2, 0x18, 0xa8,
                                                                       0xac, 0xf8, 0x75, 0x28, 0x8f, 0x36, 0x6a, 0xc8, 0xd2, 0xc3, 0x57, 0x2c, 0x22,
                                                                       0xc4, 0xd6, 0x2a, 0x3a, 0x88, 0xb8, 0xce, 0x38, 0xc9};
__ALIGN_BEGIN static uint8_t aExpectSHA3_512Digest[64] __ALIGN_END = { 0x29, 0x76, 0x0d, 0x50, 0xc9, 0x7c, 0x9c, 0xfa, 0xb6, 0xd5, 0xb6, 0x05, 0x45,
                                                                       0x82, 0x30, 0xd2, 0x6b, 0x32, 0x45, 0x4d, 0xad, 0x06, 0xc3, 0xae, 0x6f, 0x34,
                                                                       0xbb, 0xe3, 0xe6, 0xd9, 0xaf, 0x89, 0xe5, 0xbf, 0x83, 0x0b, 0x8b, 0x99, 0x33,
                                                                       0x42, 0xfc, 0xe5, 0x6b, 0x76, 0x01, 0x4b, 0x16, 0x80, 0xb0, 0xf4, 0xa0, 0xcc,
                                                                       0xc6, 0x27, 0x18, 0xff, 0x9e, 0x9c, 0xd6, 0xf8, 0x2d, 0x49, 0xc4, 0x6f};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_MDMA_Init(void);
static void MX_HASH_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initialize the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* Configure the system clock */
  SystemClock_Config();
 
#if (USE_STPMIC1x) && !defined(USE_DDR)
  /* Configure PMIC */
  BSP_PMIC_Init();
  BSP_PMIC_InitRegulators();
#endif

  /* USER CODE END Init */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_MDMA_Init();
  MX_HASH_Init();

  /* USER CODE BEGIN 2 */
  /* Initialize the IO expander */
  BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE);

  /* Configure LED_GREEN */
  BSP_LED_Init(LED_GREEN);

  /****************************************************************************/
  /****************************** SHA3_384 **************************************/
  /****************************************************************************/
  /* Start HASH computation using MDMA transfer */
  if (HAL_HASHEx_SHA3_384_Start_DMA(&hhash, (uint8_t*)aInput, strlen((char const*)aInput)) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait for MDMA transfer to complete */
  while (HAL_HASH_GetState(&hhash) == HAL_HASH_STATE_BUSY);

  /* Get the computed digest value */
  if(HAL_HASHEx_SHA3_384_Finish(&hhash, aSHA3_384Digest, 0xFF) != HAL_OK)
  {
    Error_Handler();
  }

  /* Compare computed digest with expected one */
  if(memcmp(aSHA3_384Digest, aExpectSHA3_384Digest, sizeof(aExpectSHA3_384Digest)/sizeof(aExpectSHA3_384Digest[0])) != 0)
  {
    Error_Handler();
  }
  else
  {
    BSP_LED_On(LED_GREEN);
  }

  /****************************************************************************/
  /***************************** SHA3_512 ***************************************/
  /****************************************************************************/
  if(HAL_HASH_DeInit(&hhash) != HAL_OK)
  {
    Error_Handler();
  }
  MX_HASH_Init();

  /* Start HASH computation using MDMA transfer */
  if (HAL_HASHEx_SHA3_512_Start_DMA(&hhash, (uint8_t*)aInput, strlen((char const*)aInput)) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait for MDMA transfer to complete */
  while (HAL_HASH_GetState(&hhash) == HAL_HASH_STATE_BUSY);

  /* Get the computed digest value */
  if (HAL_HASHEx_SHA3_512_Finish(&hhash, aSHA3_512Digest, 0xFF) != HAL_OK)
  {
    Error_Handler();
  }

  /* Compare computed digest with expected one */
  if(memcmp(aSHA3_512Digest, aExpectSHA3_512Digest, sizeof(aExpectSHA3_512Digest)/sizeof(aExpectSHA3_512Digest[0])) != 0)
  {
    Error_Handler();
  }
  else
  {
    BSP_LED_On(LED_GREEN);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
  /* USER CODE BEGIN WHILE */

  /* USER CODE END WHILE */
  }

  /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */

}

/** System Clock Configuration
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
  RCC_ClkInitStructure.APB6_Div = RCC_APB6_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStructure) != HAL_OK) {
    /* HAL RCC configuration error */
    Error_Handler();
    }

/*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
  (GPIO, SPI, FMC, XSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
  The I/O Compensation Cell activation  procedure requires :
  - The activation of the CSI clock
  - The activation of the SYSCFG clock
  - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR

  To do this please uncomment the following code
  */

  /*
  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
*/
#endif

}

/* HASH init function */
static void MX_HASH_Init(void)
{

  hhash.Init.DataType = HASH_DATATYPE_8B;
  if (HAL_HASH_Init(&hhash) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * Enable MDMA controller clock
  */
static void MX_MDMA_Init(void)
{
  /* MDMA controller clock enable */
  __HAL_RCC_MDMA_CLK_ENABLE();

  /* MDMA interrupt configuration */
  IRQ_SetPriority(MDMA_SEC_IT_IRQn, 0);
  IRQ_Enable(MDMA_SEC_IT_IRQn);

}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
    HAL_Delay(200);
    BSP_LED_Toggle(LED_GREEN);
  }
  /* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

