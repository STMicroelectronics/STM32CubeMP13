/**
  ******************************************************************************
  * @file    BSP/BSP_TS/Src/main.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the STM32MP135F-DK BSP Drivers
  *          This is the main program.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stlogo.h"
#include <string.h>

/** @addtogroup STM32MP13xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */


/* Private define ------------------------------------------------------------*/
#define IMAGE_HOR_SIZE    480
#define IMAGE_VER_SIZE    272
#define ALPHA_VALUE       255
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t DemoIndex = 0;
__IO uint32_t UserButtonPressed = 0;

/*
 *
 * can be adjusted according to the single frame size. must be 4 byte aligned
 */
static uint32_t Buffer[130560] __attribute__((aligned(16)));

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void LCD_Config(void);
static void Display_DemoDescription(void);

/* Private typedef -----------------------------------------------------------*/

BSP_DemoTypedef  BSP_examples[] =
{
  {Touchscreen_demo1, "TOUCHSCREEN DEMO 1", 0},
  {Touchscreen_demo2, "TOUCHSCREEN DEMO 2", 1},
  {Touchscreen_demo3, "TOUCHSCREEN DEMO 3", 2},

};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* Reset of all peripherals, Initialize the Systick. */
 HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* When system initialization is finished, Cortex-M7 could wakeup (when needed) the Cortex-M4  by means of
     HSEM notification or by any D2 wakeup source (SEV,EXTI..)   */
#if (USE_STPMIC1x) && !defined(USE_DDR)
  /* Configure PMIC */
  BSP_PMIC_Init();
  BSP_PMIC_InitRegulators();
#endif /* (USE_STPMIC1x) && !defined(USE_DDR) */

  /* Configure LED_BLUE & LED_RED */
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);

  /* Configure the User push-button in EXTI Mode */
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /*##   LCD Configuration ##################################################*/
  LCD_Config();


  UTIL_LCD_SetFuncDriver(&LCD_Driver);

  Display_DemoDescription();

  while (1)
  {
    /* Wait For User inputs */
    if (UserButtonPressed == 1)
    {
      UserButtonPressed = 0;
      BSP_examples[DemoIndex++].DemoFunc();
      if (DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
      {
        UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_FillRect(0, 200, 480, 72, UTIL_LCD_COLOR_BLUE);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_DisplayStringAt(480, 136, (uint8_t *)" END OF TOUCHSCREEN DEMO ", CENTER_MODE);
        HAL_Delay(1000);
        break;
      }
      Display_DemoDescription();
    }
  }
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

  RCC_OscInitStructure.HSICalibrationValue = 0x00; /* Default reset value */
  RCC_OscInitStructure.CSICalibrationValue = 0x10; /* Default reset value */
  RCC_OscInitStructure.HSIDivValue = RCC_HSI_DIV1; /* Default value */

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

  if (HAL_RCC_OscConfig(&RCC_OscInitStructure) != HAL_OK)
  {
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStructure) != HAL_OK)
  {
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
#endif /* !USE_DDR */

}
/**
  * @brief LCD Configuration.
  * @note  This function Configures the LTDC peripheral :
  *        1) DeInit LCD
  *        2) Init LCD
  *        3) Configure the LTDC Layer 1 :
  *           - color RGB565/YUV as pixel format
  *           - The frame buffer is located at DDR memory
  *           - The Layer size configuration : 482*272
  *        4) Enable Display On
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

  BSP_LCD_GetLTDCPixelFormat(0, LCD_PIXEL_FORMAT_RGB565, &layer_cfg.PixelFormat);
  layer_cfg.WindowX0 = 0;
  layer_cfg.WindowX1 = IMAGE_HOR_SIZE;
  layer_cfg.WindowY0 = 0;
  layer_cfg.WindowY1 = IMAGE_VER_SIZE;

  layer_cfg.FBStartAdress = (uint32_t)Buffer;
  layer_cfg.Alpha = ALPHA_VALUE;
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

  /* Disable the FIFO underrun interrupt */
  __HAL_LTDC_DISABLE_IT(&hLtdcHandler, LTDC_IT_FU);

  /* Enable Display On */
  BSP_LCD_DisplayOn(0);
}


/**
  * @brief  Display main demo messages
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  char desc[64];
  uint32_t x_size;
  uint32_t y_size;

  /* Get LCD width and Height */
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);

  /* Display LCD messages */
  UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32MP135F-DK BSP", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 35, (uint8_t *)"Drivers examples", CENTER_MODE);

  /* Draw Bitmap */
  UTIL_LCD_DrawBitmap((x_size - 80) / 2, 65, (uint8_t *)stlogo);

  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, y_size - 20, (uint8_t *)"Copyright (c) STMicroelectronics 2024", CENTER_MODE);

  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_FillRect(0, y_size / 2 - 15, x_size, 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(0, y_size / 2, (uint8_t *)"Press User button to start :", CENTER_MODE);
  sprintf(desc, "%s example", BSP_examples[DemoIndex].DemoName);
  UTIL_LCD_DisplayStringAt(0, y_size / 2 + 15, (uint8_t *)desc, CENTER_MODE);

}

/**
  * @brief  Check for user input
  * @param  None
  * @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  return UserButtonPressed;
}
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if (Button == BUTTON_USER)
  {
    UserButtonPressed = 1;
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{


  while (1)
  {
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(1000);
  }

}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file  pointer to the source file name
  * @param  line  assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(file);
  UNUSED(line);

  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

}
#endif /* USE_FULL_ASSERT */


/**
  * @}
  */
