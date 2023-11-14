/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use GPIOs through
  *          the STM32MP13xx HAL API.
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

/* Private includes ----------------------------------------------------------*/
#include "stm32mp13xx_ll_etzpc.h"
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
DMA_HandleTypeDef hdma;
static __IO bool transferCompleted; /* Set to true if the transfer is done */
static __IO bool transferErrorDetected; /* Set to true if the transfer is an error occurred during the transfer */

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_DMA_Init(void);
static void testTransfer(char *src, bool srcSecure, char *dst, bool dstSecure);
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

  /* Reset of all peripherals, Initialize the Systick */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* Configure the system clock */
  SystemClock_Config();

#if (USE_STPMIC1x) && !defined(USE_DDR)
  /* Configure PMIC */
  BSP_PMIC_Init();
  BSP_PMIC_InitRegulators();
#endif

  /* ETZPC clock enable */
  __HAL_RCC_ETZPC_CLK_ENABLE();

  /* SYSRAM is unsecured for the last 4 Kbyte */
  LL_ETZPC_SetSecureSysRamSize(ETZPC, 0x1f);

  /* Lock SYSRAM secure size: it will not be modifiable until the next reset */
  LL_ETZPC_LockSecureSysRamSize(ETZPC);
  /* Get old TZMA1_SIZE value to check later that it cannot be changed once locked */
  uint32_t tzma1_size_old = ETZPC->TZMA1_SIZE;
  /* Try to change TZMA1_SIZE, it should not work*/
  LL_ETZPC_SetSecureSysRamSize(ETZPC, 0x00);
  /* Get new TZMA1_SIZE value */
  uint32_t tzma1_size_new = ETZPC->TZMA1_SIZE;
  /* Check if both TZMA1_SIZE values match */
  if (tzma1_size_old != tzma1_size_new)
  {
    /* TZMA1_SIZE should not have changed */
    Error_Handler();
  }

  /* SRAM1 is already secure for reading and writing after reset but the following line shows what to do to secure it if it was previously made unsecure */
  LL_ETZPC_Set_SRAM1_PeriphProtection(ETZPC, LL_ETZPC_PERIPH_PROTECTION_READ_WRITE_SECURE);

  /* SRAM2 is unsecure for reading, writing is secure */
  LL_ETZPC_Set_SRAM2_PeriphProtection(ETZPC, LL_ETZPC_PERIPH_PROTECTION_READ_NONSECURE_WRITE_SECURE);

  /* SRAM3 is unsecure for both reading and writing */
  LL_ETZPC_Set_SRAM3_PeriphProtection(ETZPC, LL_ETZPC_PERIPH_PROTECTION_READ_WRITE_NONSECURE);

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_DMA_Init();

  /* USER CODE BEGIN 2 */
  /* Initialize the IO expander */
  BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE);
  /* Initialize LED_GREEN */
  BSP_LED_Init(LED_GREEN);

  /* DMA from SYSRAM unsecure to SYSRAM secure */
  testTransfer(&SYSRAM_UNSECURE, false, &SYSRAM_SECURE, true);
  /* DMA from SYSRAM secure to SYSRAM unsecure */
  testTransfer(&SYSRAM_SECURE, true, &SYSRAM_UNSECURE, false);

  /* DMA from SYSRAM unsecure to SRAM1 read and write secure */
  testTransfer(&SYSRAM_UNSECURE, false, &SRAM1_READ_WRITE_SECURE, true);
  /* DMA from SRAM1 read and write secure to SYSRAM unsecure */
  testTransfer(&SRAM1_READ_WRITE_SECURE, true, &SYSRAM_UNSECURE, false);

  /* DMA from SYSRAM unsecure to SRAM2 read unsecure and write secure */
  testTransfer(&SYSRAM_UNSECURE, false, &SRAM2_READ_UNSECURE_WRITE_SECURE, true);
  /* DMA from SRAM2 read unsecure and write secure to SYSRAM unsecure */
  testTransfer(&SRAM2_READ_UNSECURE_WRITE_SECURE, false, &SYSRAM_UNSECURE, false);

  /* DMA from SYSRAM unsecure to SRAM3 read and write unsecure */
  testTransfer(&SYSRAM_UNSECURE, false, &SRAM3_READ_WRITE_UNSECURE, false);
  /* DMA from SRAM3 read and write unsecure to SYSRAM unsecure */
  testTransfer(&SRAM3_READ_WRITE_UNSECURE, false, &SYSRAM_UNSECURE, false);

  /* Turn on LED_GREEN if test passes then enter infinite loop */
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

/* USER CODE BEGIN 4 */
/** DMA init function */
/**
  * @brief  DMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void TransferComplete(DMA_HandleTypeDef *DmaHandle)
{
  transferCompleted = true;
}

/**
  * @brief  DMA conversion error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
static void TransferError(DMA_HandleTypeDef *DmaHandle)
{
  transferErrorDetected = true;
}

/* Compare one memory area against a char */
static bool isMemoryBlockEqualToValue(const char *p, char value)
{
  for (int i = 0; i < MEMORY_BLOCK_SIZE; i++)
  {
    if (p[i] != value)
      return false;
  }
  return true;
}

/*
 * Test DMA transfer from one memory area to another
 * It checks that the transfer is effectively cancelled if one area is secure since DMA1 is unsecure.
 */
static void testTransfer(char *src, bool srcSecure, char *dst, bool dstSecure)
{
  char srcChar = srcSecure ? SRC_SECURE_CHAR : SRC_UNSECURE_CHAR;
  char dstChar = dstSecure ? DST_SECURE_CHAR : DST_UNSECURE_CHAR;

  memset(src, srcChar, MEMORY_BLOCK_SIZE);
  memset(dst, dstChar, MEMORY_BLOCK_SIZE);

  transferCompleted = false;
  transferErrorDetected = false;
  if (HAL_DMA_Start_IT(&hdma, (uint32_t)src, (uint32_t)dst, MEMORY_BLOCK_SIZE / 4))
  {
    /* Transfer Error */
    Error_Handler();
  }
  while (!transferCompleted && !transferErrorDetected) ;

  if (transferErrorDetected)
  {
    if (srcSecure || dstSecure)
    {
      /* An unsecure read or write access (through unsecure DMA1) to a secure device generates a bus error and stops the DMA transfer */
    }
    else
    {
      /* Transfer Error */
      Error_Handler();
    }
  }

  if (memcmp(src, dst, MEMORY_BLOCK_SIZE))
  {
    /* Source and destination are different */
    if (!srcSecure && !dstSecure)
    {
      /* This is not correct if both source and destination are unsecure */
      Error_Handler();
    }
  }
  else
  {
    /* Source and destination are identical */
    if (srcSecure || dstSecure)
    {
      /* This is not correct if either source and destination are secure */
      Error_Handler();
    }
  }

  /* If the source is unsecure and the destination is unsecure, the destination must be equal to the source */
  if (!srcSecure && !dstSecure)
  {
    if (!isMemoryBlockEqualToValue(dst, srcChar))
    {
        /* This is not correct if the unsecure destination is different from the unsecure source */
        Error_Handler();
    }
  }

  /* If the destination is secure, it must not have changed */
  if (dstSecure)
  {
    if (!isMemoryBlockEqualToValue(dst, dstChar))
    {
        /* This is not correct if the secure destination has changed */
        Error_Handler();
    }
  }
}

static void MX_DMA_Init(void)
{
  /*
   * DMA1 can only access unsecure devices, for secure devices, DMA3 must be used.
   * DMA1 is used in this example to illustrate the effects of secure and unsecure accesses:
   * - DMA1 (or DMA2 if we chose it instead of DMA1) only do unsecure accesses
   * - If either the source or destination is unsecure, the DMA transfer is stopped
   */

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMAMUX1_CLK_ENABLE();

  /* Configure DMA request hdma on DMA1_Stream0 */
  hdma.Instance = DMA1_Stream0;
  hdma.Init.Request = DMA_REQUEST_MEM2MEM;
  hdma.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma.Init.MemInc = DMA_MINC_ENABLE;
  hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma.Init.Mode = DMA_NORMAL;
  hdma.Init.Priority = DMA_PRIORITY_LOW;
  hdma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma.Init.MemBurst = DMA_MBURST_INC4;
  hdma.Init.PeriphBurst = DMA_PBURST_INC4;
  if (HAL_DMA_Init(&hdma) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_DMA_RegisterCallback(&hdma, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&hdma, HAL_DMA_XFER_ERROR_CB_ID, TransferError);

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  IRQ_SetPriority(DMA1_Stream0_IRQn, 0);
  IRQ_Enable(DMA1_Stream0_IRQn);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
    /* Toggle LED_GREEN for error */
    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(500);
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
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

