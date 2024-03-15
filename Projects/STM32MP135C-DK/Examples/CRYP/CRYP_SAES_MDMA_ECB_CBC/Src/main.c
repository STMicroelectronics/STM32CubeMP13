/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
RNG_HandleTypeDef hrng;

CRYP_HandleTypeDef hcryp;
uint32_t pKeySAES[4] = {0x2B7E1516,0x28AED2A6,0xABF71588,0x09CF4F3C};

/* USER CODE BEGIN PV */

static CRYP_ConfigTypeDef Conf;

/* The size of the plaintext is in words */
#define PLAINTEXT_SIZE            16
#define PLAINTEXT_SIZE_IN_BYTES   (PLAINTEXT_SIZE * 4U)

uint32_t AESIV[4] =   {0x00010203 , 0x04050607 , 0x08090A0B , 0x0C0D0E0F};

uint32_t AESKey256[8] = {0x603DEB10 ,0x15CA71BE ,0x2B73AEF0 ,0x857D7781 ,
                          0x1F352C07 ,0x3B6108D7 ,0x2D9810A3 ,0x0914DFF4};

uint32_t Plaintext[16] = { 0x6BC1BEE2 ,0x2E409F96 ,0xE93D7E11 ,0x7393172A ,

                          0xAE2D8A57 ,0x1E03AC9C ,0x9EB76FAC ,0x45AF8E51 ,
                          0x30C81C46 ,0xA35CE411 ,0xE5FBC119 ,0x1A0A52EF ,
                          0xF69F2445 ,0xDF4F9B17 ,0xAD2B417B ,0xE66C3710};

uint32_t CiphertextAESECB128[16] = {0x3AD77BB4 ,0x0D7A3660 ,0xA89ECAF3 ,0x2466EF97 ,
                          0xF5D3D585 ,0x03B9699D ,0xE785895A ,0x96FDBAAF ,
                          0x43B1CD7F ,0x598ECE23 ,0x881B00E3 ,0xED030688 ,
                          0x7B0C785E ,0x27E8AD3F ,0x82232071 ,0x04725DD4};

uint32_t CiphertextAESCBC256[16] = {0xF58C4C04 ,0xD6E5F1BA ,0x779EABFB ,0x5F7BFBD6 ,
                          0x9CFC4E96 ,0x7EDB808D ,0x679F777B ,0xC6702C7D ,
                          0x39F23369 ,0xA9D9BACF ,0xA530E263 ,0x04231461 ,
                          0xB2EB05E2 ,0xC39BE9FC ,0xDA6C1907 ,0x8C6A9D1B};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_RNG_Init(void);
static void MX_SAES_AES_Init(void);
/* USER CODE BEGIN PFP */

/* Used for storing the encrypted text */
uint32_t EncryptedText[PLAINTEXT_SIZE];

/* Used for storing the decrypted text */
uint32_t DecryptedText[PLAINTEXT_SIZE];

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
  /* Initialize the IO expander */
  BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE);
  /* Configure LED_ORANGE and LED_GREEN */
  BSP_LED_Init(LED_ORANGE);
  BSP_LED_Init(LED_GREEN);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_RNG_Init();
  MX_SAES_AES_Init();
  /* USER CODE BEGIN 2 */
  /* Waiting RNG activation */
  HAL_Delay(10);

  /*##-2- Secure AES ECB Encryption/Decryption #################################################*/
  if (HAL_CRYP_Encrypt_MDMA(&hcryp, Plaintext, PLAINTEXT_SIZE, EncryptedText) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY);

  /*Compare results with expected buffer*/
  if(memcmp(EncryptedText, CiphertextAESECB128, PLAINTEXT_SIZE_IN_BYTES) != 0)
  {
    /* Processing Error */
    Error_Handler();
  }

  if (HAL_CRYP_Decrypt_MDMA(&hcryp, CiphertextAESECB128, PLAINTEXT_SIZE, DecryptedText) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY);

  /*Compare results with expected buffer*/
  if(memcmp(DecryptedText, Plaintext, PLAINTEXT_SIZE_IN_BYTES) != 0)
  {
    /* Processing Error */
    Error_Handler();
  }
  /*##-3- Change Secure AES configuration #################################################*/

  /* Get the Secure AES parameters */
  HAL_CRYP_GetConfig(&hcryp, &Conf);

  /* Change CRYP parameters */
  Conf.KeySize   = CRYP_KEYSIZE_256B;
  Conf.pKey      = AESKey256;
  Conf.Algorithm = CRYP_AES_CBC;
  Conf.pInitVect = AESIV;

  /* Set the CRYP Configuration */
  if (HAL_CRYP_SetConfig(&hcryp, &Conf) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }

  /*##-4- Secure AES CBC Encryption/Decryption #################################################*/
  if (HAL_CRYP_Encrypt_MDMA(&hcryp, Plaintext, PLAINTEXT_SIZE, EncryptedText) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY);

  /*Compare results with expected buffer*/
  if(memcmp(EncryptedText, CiphertextAESCBC256, PLAINTEXT_SIZE_IN_BYTES) != 0)
  {
    /* Processing Error */
    Error_Handler();
  }

  __HAL_CRYP_CLEAR_FLAG(&hcryp, CRYP_CLEAR_CCF);
  if (HAL_CRYP_Decrypt_MDMA(&hcryp, CiphertextAESCBC256, PLAINTEXT_SIZE, DecryptedText) != HAL_OK)
  {
    /* Processing Error */
    Error_Handler();
  }
  while (HAL_CRYP_GetState(&hcryp) != HAL_CRYP_STATE_READY);

  /*Compare results with expected buffer*/
  if(memcmp(DecryptedText, Plaintext, PLAINTEXT_SIZE_IN_BYTES) != 0)
  {
    /* Processing Error */
    Error_Handler();
  }

  /* Turn LED_GREEN on */
  BSP_LED_On(LED_GREEN);
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


/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG1;
  hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief SAES Initialization Function
  * @param None
  * @retval None
  */
static void MX_SAES_AES_Init(void)
{

  /* USER CODE BEGIN SAES_Init 0 */

  /* USER CODE END SAES_Init 0 */

  /* USER CODE BEGIN SAES_Init 1 */

  /* USER CODE END SAES_Init 1 */
  hcryp.Instance             = SAES;
  hcryp.Init.DataType        = CRYP_NO_SWAP;
  hcryp.Init.KeySize         = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey            = (uint32_t *)pKeySAES;
  hcryp.Init.Algorithm       = CRYP_AES_ECB;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  hcryp.Init.KeyMode         = CRYP_KEYMODE_NORMAL;
  hcryp.Init.KeySelect       = CRYP_KEYSEL_NORMAL;
  hcryp.Init.KeyProtection   = CRYP_KEYPROT_DISABLE;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAES_Init 2 */

  /* USER CODE END SAES_Init 2 */

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  /* Turn LED_ORANGE on */
  BSP_LED_On(LED_ORANGE);
  __disable_irq();
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
