/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    CRYP/CRYP_TDESmodes/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use the STM32MP13xx CRYP HAL API
  *          to encrypt and decrypt data using TDES in ECB and CBC Algorithm.
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

/** @addtogroup STM32MP13xx_HAL_Examples
  * @{
  */

/** @addtogroup CRYP_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* CRYP handler declaration */
static CRYP_HandleTypeDef hcryp;
static CRYP_ConfigTypeDef Conf;

/* Triple Key  */
uint32_t TDESKey[6] = { 0x01234567,0x89ABCDEF, 
                        0x23456789,0xABCDEF01, 
                        0x456789AB,0xCDEF0123};

/*Initialization Vector*/ 
uint32_t InitVector[2] = {0xF69F2445 ,0xDF4F9B17};
  
/* Plaintext */
uint32_t Plaintext[8] = { 0x6BC1BEE2, 0x2E409F96, 0xE93D7E11, 0x7393172A,
                          0xAE2D8A57, 0x1E03AC9C, 0x9EB76FAC, 0x45AF8E51};

/* Expected ECB Ciphertext */
uint32_t Ciphertext_ECB[8] = {0x714772F3, 0x39841D34,0x267FCC4B,0xD2949CC3,
                              0xEE11C22A, 0x576A3038, 0x76183F99, 0xC0B6DE87};

/* Expected CBC Ciphertext */
uint32_t Ciphertext_CBC[8] = {0x2079C3D5, 0x3AA763E1, 0x93B79E25 , 0x69AB5262,
                              0x51657048, 0x1F25B50F , 0x73C0BDA8 , 0x5C8E0DA7}; 

/* Used for storing Encrypted text */
uint32_t Encryptedtext[8]={0}; 

/* Used for storing Decrypted text */
uint32_t Decryptedtext[8]={0}; 

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void MX_CRYP_Init(void);
static void MX_GPIO_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_CRYP_Init();
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  /* Initialize the IO expander */
  BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE);

  /* Configure LED_BLUE and LED_GREEN */
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);
   
  /*## Set the CRYP parameters and initialize the IP  ######################################*/ 
  hcryp.Instance       = CRYP;
  hcryp.Init.DataType  = CRYP_DATATYPE_32B;
  hcryp.Init.pKey      = TDESKey;
  hcryp.Init.Algorithm = CRYP_TDES_ECB;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }

  /*##-1- TDES ECB Encryption  #######################################*/ 

  /* Encryption, result in  Encryptedtext buffer */ 
  if (HAL_CRYP_Encrypt(&hcryp, Plaintext, 8, Encryptedtext, TIMEOUT_VALUE)!= HAL_OK)
  {
    Error_Handler();
  }
  if(memcmp(Encryptedtext, Ciphertext_ECB, 32) != 0)
  {
    /* Not expected result, wrong on Encryptedtext buffer: Turn LED_BLUE on */
    Error_Handler();
  }
  /*##-2- TDES CBC Encryption  #######################################*/   

  /* Get current CRYP parameters */  
  if (HAL_CRYP_GetConfig(&hcryp, &Conf)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Set new parameters and configure the CRYP */
  Conf.pInitVect = InitVector;  
  Conf.Algorithm = CRYP_TDES_CBC;
  if (HAL_CRYP_SetConfig(&hcryp, &Conf)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Encryption, result in  Encryptedtext buffer */ 
  if (HAL_CRYP_Encrypt(&hcryp, Plaintext, 8, Encryptedtext, TIMEOUT_VALUE)!= HAL_OK)
  {
    Error_Handler();
  }
  
  if(memcmp(Encryptedtext, Ciphertext_CBC, 32) != 0)
  {   /* Not expected result, wrong on Encryptedtext buffer: Turn LED_BLUE on */
    Error_Handler();
  }
  /*##-3- TDES ECB Decryption  #######################################*/ 

  /* Set new parameters and configure the CRYP */
  Conf.Algorithm = CRYP_TDES_ECB;
  if (HAL_CRYP_SetConfig(&hcryp, &Conf)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Decryption, result in  Decryptedtext buffer */ 
  if (HAL_CRYP_Decrypt(&hcryp, Ciphertext_ECB , 8, Decryptedtext, TIMEOUT_VALUE)!= HAL_OK)
  {
    Error_Handler();
  }

  if(memcmp(Decryptedtext, Plaintext, 32) != 0)
  {
    /* Not expected result, wrong on Decryptedtext buffer: Turn LED_BLUE on */
    Error_Handler();
  }
  /*##-4- TDES CBC Decryption  #######################################*/   
  
  /* Set new parameters and configure the CRYP */
  Conf.Algorithm = CRYP_TDES_CBC;
  if (HAL_CRYP_SetConfig(&hcryp, &Conf)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Decryption, result in  Decryptedtext buffer */ 
  if (HAL_CRYP_Decrypt(&hcryp, Ciphertext_CBC, 8, Decryptedtext, TIMEOUT_VALUE)!= HAL_OK)
  {
    Error_Handler();
  }

  if(memcmp(Decryptedtext, Plaintext, 32) != 0)
  {   /* Not expected result, wrong on Decryptedtext buffer: Turn LED_BLUE on */
    Error_Handler();
  }  
  else
  {
    /* Right Encryptedtext & Decryptedtext buffer : Turn LED_GREEN on */
    BSP_LED_On(LED_GREEN);
  }
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
static void SystemClock_Config(void)
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

/* Initialize CRYP IP */
static void MX_CRYP_Init(void)
{
  HAL_CRYP_MspInit(&hcryp);
}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
     USB_DM1   ------> USBH_HS1_DM
     USB_DP1   ------> USBH_HS1_DP
*/
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED_BLUE on */
  BSP_LED_On(LED_BLUE);
  while (1)
  {
  }  

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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

