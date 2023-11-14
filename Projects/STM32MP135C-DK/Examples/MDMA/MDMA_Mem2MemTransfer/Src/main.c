
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

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Private variables ---------------------------------------------------------*/
MDMA_HandleTypeDef hmdma_memtomem;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
static const uint32_t aSRC_Const_Buffer[BUFFER_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

uint32_t __attribute__((section (".myBufCPUSRAMBlock._dstBufSRAM3"))) aDST_Buffer_SRAM3[BUFFER_SIZE];

static __IO uint32_t TComplete; /* Set to 1 if transfer is COMPLETED */
static __IO uint32_t transferErrorDetected; /* Set to 1 if an error transfer is detected */
static HAL_StatusTypeDef transferstatus;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_MDMA_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

static void TransferComplete(MDMA_HandleTypeDef *MdmaHandle);
static void TransferError(MDMA_HandleTypeDef *MdmaHandle);
static HAL_StatusTypeDef BufferCmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint32_t waitTime =0;
  /* Set to 1 if an transfer error is detected */
  transferErrorDetected = 0;

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
#if defined (USE_STM32MP13XX_DK)
  /* Initialize the IO expander */
  BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE);
#endif

  /* Initialize LED */
  BSP_LED_Init(LED_GREEN);

  /* Enable write access to Backup domain */
  PWR->CR1 |= PWR_CR1_DBP;

  while((PWR->CR1 & PWR_CR1_DBP) == RESET)
  {
  }

  MX_MDMA_Init();

  /* Select Callbacks functions called after Transfer complete and Transfer error */
  HAL_MDMA_RegisterCallback(&hmdma_memtomem, HAL_MDMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_MDMA_RegisterCallback(&hmdma_memtomem, HAL_MDMA_XFER_ERROR_CB_ID, TransferError);

  /* Start MDMA transfer : Source = SYSRAM and Destination = SRAM3 */
  if (HAL_MDMA_Start_IT(&hmdma_memtomem, (uint32_t)&aSRC_Const_Buffer, (uint32_t)&aDST_Buffer_SRAM3, (BUFFER_SIZE * 4), 1)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Waiting for transfer complete flag */
  while((TComplete != 1) && (waitTime < 0xFF))
  {
    HAL_Delay(1);
    waitTime++;
  }

  /* Compare the Source and Destination buffers */
  if(BufferCmp((uint32_t*)aSRC_Const_Buffer,(uint32_t*)aDST_Buffer_SRAM3, (BUFFER_SIZE)) != HAL_OK)
  {
    /* Wrong encryption */
    transferstatus = HAL_ERROR;
  }

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if ((transferstatus == HAL_OK) && (TComplete == 1 ))
    {
      /* Turn LED_GREEN on: Transfer correct */
      BSP_LED_On(LED_GREEN);
    }
    else
    {
      /* Transfer KO : Toggle LED_GREEN with a period of 200 ms */
      BSP_LED_Toggle(LED_GREEN);
      HAL_Delay(200);
    }
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
  * Enable MDMA controller clock
  * Configure MDMA for memory to memory transfers
  *   hmdma_memtomem
  */
static void MX_MDMA_Init(void)
{
  /* MDMA controller clock enable */
  __HAL_RCC_MDMA_CLK_ENABLE();

  /* ETZPC clock enable */
  __HAL_RCC_ETZPC_CLK_ENABLE();

  /* Unsecure SYSRAM so that the unsecured MDMA channel can access it. */
  LL_ETZPC_SetSecureSysRamSize(ETZPC, 0);

  /* Unsecure SRAM3 so that the unsecured MDMA channel can access it. */
  LL_ETZPC_Set_SRAM3_PeriphProtection(ETZPC, LL_ETZPC_PERIPH_PROTECTION_READ_WRITE_NONSECURE);

  /* Set the parameters to be configured */
  hmdma_memtomem.Init.Request = MDMA_REQUEST_SW;
  /* Configure MDMA in Non-SECURE Mode */
  hmdma_memtomem.Init.SecureMode = MDMA_SECURE_MODE_DISABLE;

  hmdma_memtomem.Init.Priority = MDMA_PRIORITY_HIGH;
  hmdma_memtomem.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hmdma_memtomem.Init.SourceInc = MDMA_SRC_INC_WORD;
  hmdma_memtomem.Init.DestinationInc = MDMA_DEST_INC_WORD;
  hmdma_memtomem.Init.SourceDataSize = MDMA_SRC_DATASIZE_WORD;
  hmdma_memtomem.Init.DestDataSize = MDMA_DEST_DATASIZE_WORD;
  hmdma_memtomem.Init.DataAlignment = MDMA_DATAALIGN_RIGHT;
  /* BURST Mode*/
  hmdma_memtomem.Init.SourceBurst = MDMA_SOURCE_BURST_16BEATS;
  hmdma_memtomem.Init.DestBurst = MDMA_DEST_BURST_16BEATS;

  hmdma_memtomem.Init.BufferTransferLength = 128;
  hmdma_memtomem.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
  hmdma_memtomem.Init.SourceBlockAddressOffset = 0;
  hmdma_memtomem.Init.DestBlockAddressOffset = 0;
  hmdma_memtomem.Instance = MDMA_Channel0;

  /* MDMA interrupt init */
  /* MDMA_IRQn interrupt configuration */
  IRQ_SetPriority(MDMA_IRQn, 0);
  IRQ_Enable(MDMA_IRQn);

  /* DeInitialize the DMA Stream */
  if (HAL_MDMA_DeInit(&hmdma_memtomem)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Initialize the MDMA channel */
  if (HAL_MDMA_Init(&hmdma_memtomem)!= HAL_OK)
  {
    Error_Handler();
  }

}

/* USER CODE BEGIN 4 */
/**
  * @brief  MDMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void TransferComplete(MDMA_HandleTypeDef *MdmaHandle)
{
  TComplete = 1;
  transferstatus = HAL_OK;
}

/**
  * @brief  MDMA conversion error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during MDMA transfer
  * @retval None
  */
static void TransferError(MDMA_HandleTypeDef *MdmaHandle)
{
  transferErrorDetected = 1;
  transferstatus = HAL_ERROR;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
    /* Toggle LED_GREEN with a period of 500 ms */
    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(500);
  }
  /* USER CODE END Error_Handler_Debug */
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval HAL_OK: pBuffer identical to pBuffer1
  *         HAL_ERROR: pBuffer differs from pBuffer1
  */
static HAL_StatusTypeDef  BufferCmp(uint32_t* pBuffer, uint32_t* pBuffer1, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != *pBuffer1)
    {
      return HAL_ERROR;
    }

    pBuffer++;
    pBuffer1++;
  }

  return HAL_OK;
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

