/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FSBLA/FSBLA_Sdmmc1/Src/main.c
  * @author  MCD Application Team
  * @brief   main program
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
DDR_InitTypeDef hddr;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DDR_PATTERN 1234
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/******** SD Receive Buffer definition *******/

void (*p_AppEntryPoint)(void); /* Entry point of the application*/
__IO uint8_t RxCplt; /* Variable used to indicate that load in memory is over*/
SD_HandleTypeDef SDHandle;

/* DDR_MEM_ADD = By default base address of DRAM (DDR) over AXI  = DRAM_MEM_BASE */
/* Possible to select a specific address in DDR, but the cube example has to be linked with the same address */
#define DDR_MEM_ADD DRAM_MEM_BASE

#define NB_BLOCK_HEADER 1 /* Number of block for the cube example header */
#define OFFSET_HEADER 640 /* Offset of the cube example header */
#define OFFSET_CUBE_EXAMPLE OFFSET_HEADER + NB_BLOCK_HEADER /* Offset of the cube example */
uint8_t aRxBuffer[BLOCKSIZE * NB_BLOCK_HEADER];

#define IMAGE_LENGTH_POSITION 76 /* First hexadecimal number position (aRxBuffer position) of cube example image length */
#define NB_HEXA_NB_FOR_IMAGE_LENGTH 4 /* Number of hexadecimal number representing the cube example image length */
#define LITLE_ENDIAN 1 /* Cube example header endianness */

#define  IMAGE_ENTRY_POINT_POSITION 80 /* First hexadecimal number position (aRxBuffer position) of cube example image entry point */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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

  /*# Variables to read image length from cubeExampleHeader #*/
  uint32_t sizeCubeExample = 0;
  uint32_t actualHexadecimalNbPosition;
  uint32_t bitShift;
  uint32_t nbCubeExampleBlock;
  /* DRAM_MEM_BASE = Base address of DRAM (DDR) over AXI */
  uint32_t *p = (uint32_t*)(DDR_MEM_ADD);

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /*##- Enable MCE ####################*/
  __HAL_RCC_MCE_CLK_ENABLE();

#if (USE_STPMIC1x) && !defined(USE_DDR)
  /* Configure PMIC */
  BSP_PMIC_Init();
  BSP_PMIC_InitRegulators();
#endif

  /* Configure LED_BLUE and LED_RED */
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);

  BSP_LED_Off(LED_BLUE);
  BSP_LED_Off(LED_RED);
  /* USER CODE END SysInit */

  /* USER CODE BEGIN 2 */

  /*##- TZC configuration ####################*/
  __HAL_RCC_TZC_CLK_ENABLE();

  /* Configure TZC to allow  DDR Region0 R/W  non secure for all IDs */
  TZC->GATE_KEEPER =0;
  TZC->REG_ID_ACCESSO=0xFFFFFFFF;  //Allow DDR Region0 R/W  non secure for all IDs
  TZC->REG_ATTRIBUTESO = DDR_MEM_ADD + 1;
  TZC->GATE_KEEPER |=1;  //Enable the access in secure Mode  // filter 0 request close

  /* Unsecure SYSRAM so that SDMMC1 (which we configure as non-secure) can access it. */
  LL_ETZPC_SetSecureSysRamSize(ETZPC, 0);

  /* Unsecure SDMMC1. */
  LL_ETZPC_Set_SDMMC1_PeriphProtection(ETZPC, LL_ETZPC_PERIPH_PROTECTION_READ_WRITE_NONSECURE);

  /*## No need to Enable ETZPC for security : Cortex A7 is secure master and can access all secure peripherals & memories####################*/

  /*##- Unlock debugger ####################*/
  BSEC->BSEC_DENABLE = 0x47f;

  /*##- Enable clock debug CK_DBG ####################*/
  RCC->DBGCFGR |= RCC_DBGCFGR_DBGCKEN ;

  /*##- Init DDR then Check ####################*/
  hddr.wakeup_from_standby = false;
  hddr.self_refresh = false;
  hddr.zdata = 0;
  hddr.clear_bkp = false;
  if (HAL_DDR_Init(&hddr) != HAL_OK)
  {
    Error_Handler();
  }

  /*##- Check DDR Write/Read ####################*/
  *p=DDR_PATTERN;
  if (*p!=DDR_PATTERN)
  {
    Error_Handler();
  }

  BSP_LED_On(LED_BLUE);

  /*##- Initialize SDMMC1 ####################*/
  SDHandle.Instance = SDMMC1;
  HAL_SD_DeInit(&SDHandle);

  /* SDMMC IP clock xx Mhz, SDCard clock xx Mhz  */
  SDHandle.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  SDHandle.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  SDHandle.Init.BusWide             = SDMMC_BUS_WIDE_4B;
  SDHandle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  SDHandle.Init.ClockDiv            = SDMMC_NSPEED_CLK_DIV;

  if(HAL_SD_Init(&SDHandle) != HAL_OK)
  {
    Error_Handler();
  }

  while(HAL_SD_GetCardState(&SDHandle) != HAL_SD_CARD_TRANSFER) {}


  /*##- Read CubeExampleHeader ####################*/

  /* Read Header from the SD */
  RxCplt=0;
  if(HAL_SD_ReadBlocks_DMA(&SDHandle, aRxBuffer, OFFSET_HEADER, NB_BLOCK_HEADER) != HAL_OK)
  {
    Error_Handler();
  }

  while (RxCplt == 0) {};

  /*##- Convert image length to number of block ####################*/

  for (int i = 1;i <= NB_HEXA_NB_FOR_IMAGE_LENGTH; i++)
  {
    actualHexadecimalNbPosition = NB_HEXA_NB_FOR_IMAGE_LENGTH-i;

    if(LITLE_ENDIAN == 1)
    {
      /*Little Endian : The most significant byte of the data is placed at the byte with the lowest address */
      bitShift = 8 * (NB_HEXA_NB_FOR_IMAGE_LENGTH - i);
    }else if (LITLE_ENDIAN == 0)
    {
      /* Big Endian : The least significant byte of the data is placed at the byte with the lowest address */
      bitShift = 8 * (i - 1);
    }
    sizeCubeExample |= aRxBuffer[IMAGE_LENGTH_POSITION + actualHexadecimalNbPosition] << bitShift;
  }

  nbCubeExampleBlock = sizeCubeExample / BLOCKSIZE;

  /*##- Load application into DDR ####################*/

  /* Read application from the SD */
  RxCplt = 0;
  if(HAL_SD_ReadBlocks_DMA(&SDHandle, (uint8_t *)DDR_MEM_ADD, OFFSET_CUBE_EXAMPLE, nbCubeExampleBlock+1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait until Application if loaded into DDR from SD storage */
  while (RxCplt == 0) {};

  /* LED_BLUE OFF */
  BSP_LED_Off(LED_BLUE);

  /* Reset_handler of the Application to jump */
  p_AppEntryPoint = (void *)(DDR_MEM_ADD);
  p_AppEntryPoint();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Toggle LED_BLUE */
    BSP_LED_Toggle(LED_BLUE);
    HAL_Delay(1000);
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

void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  while(HAL_SD_GetCardState(&SDHandle) != HAL_SD_CARD_TRANSFER) {}
  RxCplt=1;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* Turn LED_BLUE on */

  /* Infinite loop */
  while(1)
  {
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(1000);
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

