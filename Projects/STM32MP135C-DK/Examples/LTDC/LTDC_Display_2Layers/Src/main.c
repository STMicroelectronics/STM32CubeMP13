/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    LTDC/LTDC_Display_2Layers/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure the LTDC peripheral
  *          to display two Layers at the same time.
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
#include "st_logo_320x240.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define IMAGE_HOR_SIZE    320
#define IMAGE_VER_SIZE    240
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern LTDC_HandleTypeDef hLtdcHandler;

__IO uint32_t ReloadFlag = 0;

/* Pictures position */
uint32_t Xpos1 = 0;
uint32_t Xpos2 = 160;
uint32_t Ypos1 = 0;
uint32_t Ypos2 = 32;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void LCD_Config(void);
static void PicturesPosition(uint32_t* x1,
                         uint32_t* y1,
                         uint32_t* x2,
                         uint32_t* y2,
                         uint32_t index);
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
  uint32_t index = 0;
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
  /* Configure LED_RED */
  BSP_LED_Init(LED_RED);

  /* USER CODE END SysInit */

  /* USER CODE BEGIN 2 */

  /*##-1- LCD Configuration ##################################################*/
  /* Configure 2 layers w/ Blending */
  LCD_Config();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    for (index = 0; index < 32; index++)
    {
      /* calculate new picture position */
       PicturesPosition(&Xpos1, &Ypos1, &Xpos2, &Ypos2, index);

       /* reconfigure the layer1 position  without Reloading*/
       HAL_LTDC_SetWindowPosition_NoReload(&hLtdcHandler, Xpos1, Ypos1, LTDC_LAYER_1);
       /* reconfigure the layer2 position  without Reloading*/
       HAL_LTDC_SetWindowPosition_NoReload(&hLtdcHandler, Xpos2, Ypos2, LTDC_LAYER_2);
       /*Ask for LTDC reload within next vertical blanking*/
       ReloadFlag = 0;
       HAL_LTDC_Reload(&hLtdcHandler,LTDC_RELOAD_VERTICAL_BLANKING);

      while(ReloadFlag == 0)
      {
        /* wait till reload takes effect (in the next vertical blanking period) */
        HAL_Delay(50);
      }
    }


    for (index = 0; index < 32; index++)
    {
      /* calculate new picture position */
       PicturesPosition(&Xpos2, &Ypos2, &Xpos1, &Ypos1, index);

       /* reconfigure the layer1 position  without Reloading*/
       HAL_LTDC_SetWindowPosition_NoReload(&hLtdcHandler, Xpos1, Ypos1, LTDC_LAYER_1);
       /* reconfigure the layer2 position  without Reloading*/
       HAL_LTDC_SetWindowPosition_NoReload(&hLtdcHandler, Xpos2, Ypos2, LTDC_LAYER_2);
       /*Ask for LTDC reload within next vertical blanking*/
       ReloadFlag = 0;
       HAL_LTDC_Reload(&hLtdcHandler,LTDC_RELOAD_VERTICAL_BLANKING);

      while(ReloadFlag == 0)
      {
        /* wait till reload takes effect (in the next vertical blanking period) */
        HAL_Delay(50);
      }
    }

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
/**
  * @brief  calculate pictures position.
  * @param  x1:    picture1 x position
  * @param  y1:    picture1 y position
  * @param  x2:    picture2 x position
  * @param  y2:    picture2 y position
  * @param  index:
  * @retval None
  */
static void PicturesPosition(uint32_t* x1, uint32_t* y1, uint32_t* x2, uint32_t* y2, uint32_t index)
{
  /* picture1 position */
  *x1 = index*5;
  *y1 = index;

  /* picture2 position */
  *x2 = 160 - index*5;
  *y2 = 32 - index;
}

/**
  * @brief  Error callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_ErrorCallback(LTDC_HandleTypeDef *hltdc)
{
  Error_Handler();
}

/**
  * @brief  Reload Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc)
{
  ReloadFlag = 1;
}

/**
  * @brief LCD Configuration.
  * @note  This function Configures the LTDC peripheral :
  *        1) DeInit LCD
  *        2) Init LCD
  *        3) Configure the LTDC Layer 1 :
  *           - color RGB888 as pixel format
  *           - The frame buffer is located at DDR memory
  *           - The Layer size configuration : 320x240
  *        4) Configure the LTDC Layer 2 :
  *           - color RGB888 as pixel format
  *           - The frame buffer is located at DDR memory
  *           - The Layer size configuration : 320x240
  *        5) Enable Display On
  * @retval
  *  None
  */
static void LCD_Config(void)
{
  LCD_LayerCfgTypeDef  layer_cfg;

  /* LTDC Initialization -------------------------------------------------------*/
  BSP_LCD_DeInit(0);

  BSP_LCD_Init(0);

  /* Layer1 Configuration ------------------------------------------------------*/
  /* Configure the Background Layer*/
  /* Start Address configuration : frame buffer is located in DDR memory */
  layer_cfg.WindowX0 = 0;
  layer_cfg.WindowX1 = IMAGE_HOR_SIZE;
  layer_cfg.WindowY0 = 0;
  layer_cfg.WindowY1 = IMAGE_VER_SIZE;
  layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
  layer_cfg.FBStartAdress = ( uint32_t)ST_logo_RGB888;
  layer_cfg.Alpha = 255;
  layer_cfg.Alpha0 = 0;
  layer_cfg.Backcolor.Blue = 0;
  layer_cfg.Backcolor.Green = 0;
  layer_cfg.Backcolor.Red = 0;
  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  layer_cfg.ImageWidth = IMAGE_HOR_SIZE;
  layer_cfg.ImageHeight = IMAGE_VER_SIZE;
  layer_cfg.HorMirrorEn = 0;
  layer_cfg.VertMirrorEn = 0;

  HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LTDC_LAYER_1);

  /* Layer2 Configuration ------------------------------------------------------*/
  /* Configure the Foreground Layer*/
  /* Start Address configuration : frame buffer is located in DDR memory */
  HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LTDC_LAYER_2);

  /* Disable the FIFO underrun interrupt */
  __HAL_LTDC_DISABLE_IT(&hLtdcHandler, LTDC_IT_FU);

  /* Enable Display On */
  BSP_LCD_DisplayOn(0);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
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
