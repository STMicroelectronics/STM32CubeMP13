/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    SD/SD_Ext_Loader/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use SD through
  *          the STM32MP13xx HAL API.
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
#ifdef DEBUG
#include "stm32mp13xx_ll_etzpc.h"
#endif
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#ifdef DEBUG
#define BUFFER_SIZE            ((uint32_t)0x00000400U) /* 1KB */
#define BUFFER_WORD_SIZE       (uint32_t)10            /* Buffer size in Word */
#define SD_BLOCKADDRESS        ((uint32_t)128)         /* SD Card Block Address to write/read data */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t Buffer0[BUFFER_WORD_SIZE] = {8,9,7,9,5,9,8,1,7,7};
uint8_t Buffer1[BUFFER_WORD_SIZE];
#endif /* DEBUG */

SD_HandleTypeDef hsd1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

/* Private function prototypes -----------------------------------------------*/
static void MX_SDMMC1_SD_Init(void);
int Init(void);
void MassErase(void);
void Erase(uint32_t block_start_addr, uint32_t block_end_addr);
void Write(uint32_t BlockAdd, uint32_t tot_blocks, uint8_t *buffer);
void Read(uint32_t BlockAdd, uint32_t tot_blocks, uint8_t *buffer);
uint64_t Verify(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t missalignement);
void configuration(void *ptr);

typedef struct {
	int (*Init) (void);
	void (*MassErase) (void);
	void (*Erase) (uint32_t start_addr, uint32_t end_addr);
	void (*Write) (uint32_t BlockAdd, uint32_t tot_blocks, uint8_t *buffer);
	void (*Read) (uint32_t BlockAdd, uint32_t tot_blocks, uint8_t *buffer);
	uint64_t (*Verify) (uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t missalignement);
	void (*configure)(void *);
} loader_fptr_t;

#if defined ( __ICCARM__ ) /*!< IAR Compiler */

const loader_fptr_t loader_fptr_sg  @"loader"= {
		.Init = Init,
		.MassErase = MassErase,
		.Erase = Erase,
		.Write = Write,
		.Read = Read,
		.Verify = Verify,
		.configure = configuration,
};

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

#elif defined ( __GNUC__ ) /* GNU Compiler */

const loader_fptr_t loader_fptr_sg __attribute__ ((section(".loader"), used)) = {
		.Init = Init,
		.MassErase = MassErase,
		.Erase = Erase,
		.Write = Write,
		.Read = Read,
		.Verify = Verify,
		.configure = configuration,
};

#endif
/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
#ifdef DEBUG
  uint32_t status = 9;

  /* Reset of all peripherals, Initialize the Systick. */
  HAL_Init();

  /* Configure the system clock */
  /* Uncomment SystemClock_Config(); when used as a standalone project */
  SystemClock_Config();

  /* Set SYSRAM in Non Secure */
  __HAL_RCC_TZC_CLK_ENABLE();
  LL_ETZPC_SetSecureSysRamSize(ETZPC, 0);

  Init();

  /* Erase first 2 MB */
  Erase(SD_BLOCKADDRESS, ((2*1024*1024)/512));
  /* Write 2 blocks of data */
  Write(SD_BLOCKADDRESS, 1, Buffer0);
  /* Read 2 blocks of data */
  Read(SD_BLOCKADDRESS, 1, Buffer1);
  /* Verify Data Read */
  status = Verify((uint32_t)Buffer0, (uint32_t)Buffer1, 1, 0);
  status = status;
  /* Erase 2 MB */
  Erase(SD_BLOCKADDRESS, ((2*1024*1024)/512));
#endif /* DEBUG */
  while(1);
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
  (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
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
int Init(void)
{
	/*##-2- Initialize SD instance #####################*/
	/* Initialize all configured peripherals */
	MX_SDMMC1_SD_Init();

	/* Try to switch to High Speed Mode , if supported by the card */
	HAL_SD_ConfigSpeedBusOperation(&hsd1,SDMMC_SPEED_MODE_HIGH);

	return 1;
}

/**
 * @brief  SDCARD Configuration.
 * @param  void * pointer to the configuration structure
 */
void configuration(void *ptr)
{
	return;
}

void MassErase()
{
//    if(HAL_SD_Erase(&hsd1, 0, 1) != HAL_OK)
//    {
//      Error_Handler();
//	}
//    /* Wait for the Card Transfer status */
//    while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
//    {
//    }
}

void Erase(uint32_t block_start_addr,uint32_t block_end_addr)
{
    if(HAL_SD_Erase(&hsd1,block_start_addr,block_end_addr) != HAL_OK)
    {
      Error_Handler();
	}
    /* Wait for the Card Transfer status */
    while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
    {
    }
}

void Write(uint32_t BlockAdd, uint32_t tot_blocks, uint8_t *buffer)
{
	if(HAL_SD_WriteBlocks(&hsd1, buffer, BlockAdd, tot_blocks, SDMMC_DATATIMEOUT)!= HAL_OK)
    {
        Error_Handler();
	}
	/* Wait for the Card Transfer status */
	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
	{
	}
}

void Read(uint32_t BlockAdd, uint32_t tot_blocks, uint8_t *buffer)
{
    /* Read back written block*/
	if(HAL_SD_ReadBlocks(&hsd1, buffer, BlockAdd, tot_blocks, SDMMC_DATATIMEOUT)!= HAL_OK)
	{
		Error_Handler();
	}
	/* Wait for the Card Transfer status */
	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
	{
	}
}

uint64_t Verify(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t missalignement)
{
	return 0;
}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  HAL_SD_DeInit(&hsd1);

  /* SDMMC IP clock xx Mhz, SDCard clock xx Mhz  */
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = SDMMC_NSPEED_CLK_DIV;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
    Error_Handler();
  }

  while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER) {}
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */
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

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
//  BSP_LED_Off(LED_GREEN);
//  BSP_LED_Off(LED_BLUE);
  while(1)
  {
//    BSP_LED_Toggle(LED_RED);
    HAL_Delay(200);
  }
  /* USER CODE END Error_Handler_Debug */
}
