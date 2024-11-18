
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SECTORS_COUNT 3
#define FLASH_ADDRESS 0x70000000

uint8_t buffer_test[MX25L512_SECTOR_SIZE];

typedef struct {
	int (*Init) (void);
	int (*MassErase) (void);
	int (*Erase) (uint32_t start_addr, uint32_t end_addr);
	int (*Write) (uint32_t BlockAdd, uint32_t tot_blocks, uint8_t *buffer);
	int (*Read) (uint32_t BlockAdd, uint32_t tot_blocks, uint8_t *buffer);
	uint64_t (*Verify) (uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t missalignement);
	void (*configure)(void *);
} loader_fptr_t;

const loader_fptr_t loader_fptr_sg __attribute__ ((section(".loader"), used)) = {
		.Init = Init,
		.MassErase = MassErase,
		.Erase = SectorErase,
		.Write = Write,
		.Read = NULL,
		.Verify = Verify,
		.configure = configuration,
};

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void) {

  /* USER CODE BEGIN 1 */
  uint32_t var = 0;
  uint32_t res = 0;
  /* USER CODE END 1 */

  /* USER CODE BEGIN 2 */
  Init();

  for (var = 0; var < MX25L512_SECTOR_SIZE; var++) {
    buffer_test[var] = (var & 0xFF);
  }

  for (var = 0; var < SECTORS_COUNT; var++) {
    if (SectorErase((FLASH_ADDRESS + var * MX25L512_SECTOR_SIZE), (FLASH_ADDRESS + (var + 1) * MX25L512_SECTOR_SIZE - 1)) != 0x1) {
        while (1) {}
    }
  }

  HAL_Delay(2000);

  for (var = 0; var < SECTORS_COUNT; var++) {
    if (Write((FLASH_ADDRESS + var * MX25L512_SECTOR_SIZE), sizeof(buffer_test), buffer_test) != 0x1) {
        while (1) {}
    }
  }

  HAL_Delay(2000);

  res = Verify(FLASH_ADDRESS, (uint32_t)buffer_test, (SECTORS_COUNT * MX25L512_SECTOR_SIZE), 0);
  if (res < (FLASH_ADDRESS + ((SECTORS_COUNT - 1) * MX25L512_SECTOR_SIZE)))
	{
    while (1) {};
	}

  MassErase();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
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
#if defined (USE_STM32MP13XX_DK)
  RCC_OscInitStructure.PLL3.PLLR = 2;
#else
  RCC_OscInitStructure.PLL3.PLLR = 37;
#endif
  RCC_OscInitStructure.PLL3.PLLRGE = RCC_PLL3IFRANGE_1;
  RCC_OscInitStructure.PLL3.PLLFRACV = 0x1a04;
  RCC_OscInitStructure.PLL3.PLLMODE = RCC_PLL_FRACTIONAL;

  RCC_OscInitStructure.PLL4.PLLState = RCC_PLL_ON;
  RCC_OscInitStructure.PLL4.PLLSource = RCC_PLL4SOURCE_HSE;
#if defined (USE_STM32MP13XX_DK)
  RCC_OscInitStructure.PLL4.PLLM = 2;
  RCC_OscInitStructure.PLL4.PLLN = 50;
  RCC_OscInitStructure.PLL4.PLLP = 12;
#elif defined (USE_STM32MP13XX_VALID2)
  RCC_OscInitStructure.PLL4.PLLM = 2;
  RCC_OscInitStructure.PLL4.PLLN = 50;
  RCC_OscInitStructure.PLL4.PLLP = 6;
#else
  RCC_OscInitStructure.PLL4.PLLM = 4;
  RCC_OscInitStructure.PLL4.PLLN = 99;
  RCC_OscInitStructure.PLL4.PLLP = 6;
#endif
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

/* USER CODE END 4 */


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */

  /* User may add here some code to deal with this error */
  while(1)
  {
    HAL_Delay(500);
    BSP_LED_Toggle(LED_ORANGE);
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
void assert_failed(uint8_t* file, uint32_t line)
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

/**
  * @}
  */

/**
  * @}
  */

