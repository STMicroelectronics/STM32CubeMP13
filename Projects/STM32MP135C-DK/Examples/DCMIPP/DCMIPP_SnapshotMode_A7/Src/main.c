/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    DCMIPP/DCMIPP_SnapshotMode_A7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure the DCMIPP peripheral
  *          in Snapshot mode.
  *
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define IMAGE_HOR_SIZE    480
#define IMAGE_VER_SIZE    272
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern LTDC_HandleTypeDef hLtdcHandler;
DCMIPP_HandleTypeDef Hdcmipp;

__IO uint32_t ReloadFlag = 0;

/*
 *
 * can be adjusted according to the single frame size. must be 4 byte aligned
 */
static uint32_t Buffer[960000] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void LCD_Config(void);
static void Camera_Config(void);
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
#endif /* (USE_STPMIC1x) && !defined(USE_DDR) */


  /* USER CODE BEGIN SysInit */
  /* Configure LED_RED */
  BSP_LED_Init(LED_BLUE);
  BSP_PB_Init(BUTTON_USER2, BUTTON_MODE_GPIO);
  /* USER CODE END SysInit */

  /* USER CODE BEGIN 2 */
  memset(Buffer, 0x7f, sizeof(Buffer));

#if defined(CACHE_USE)
  L1C_CleanInvalidateDCacheAll();
#endif /* CACHE_USE */

  /*##   LCD Configuration ##################################################*/
  LCD_Config();
  /*## Camera Configuration ##################################################*/
  Camera_Config();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_Delay(20);
  while (1)
  {
    while (BSP_PB_GetState(BUTTON_USER2) != SET)
    {
      HAL_Delay(100);
    }
    /* Wait for user release button */
    while (BSP_PB_GetState(BUTTON_USER2) != RESET);

    HAL_DCMIPP_PIPE_Start(&Hdcmipp, DCMIPP_PIPE0, (uint32_t)&Buffer, DCMIPP_MODE_SNAPSHOT);

    while (1)
    {
      HAL_Delay(100);
      BSP_LED_Toggle(LED_BLUE);
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

  /* Disable secure mode */
  ETZPC->DECPROT0 |= ETZPC_DECPROT0_DCMIPP;
  ETZPC->DECPROT0 |= ETZPC_DECPROT0_DDRCTRL;
}

/* USER CODE BEGIN 4 */
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
  BSP_LCD_DeInit();

  BSP_LCD_Init();

  /* Layer1 Configuration ------------------------------------------------------*/
  /* Configure the Background Layer*/
  /* Start Address configuration : frame buffer is located in DDR memory */
  layer_cfg.WindowX0 = 0;
  layer_cfg.WindowX1 = IMAGE_HOR_SIZE;
  layer_cfg.WindowY0 = 0;
  layer_cfg.WindowY1 = IMAGE_VER_SIZE;
  layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  layer_cfg.FBStartAdress = (uint32_t)Buffer;
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

  /* Disable the FIFO underrun interrupt */
  __HAL_LTDC_DISABLE_IT(&hLtdcHandler, LTDC_IT_FU);

  /* Enable Display On */
  BSP_LCD_DisplayOn();
}

/**
  * @brief Comara configuration
  * @note  This function Configures the DCMIPP peripheral :
  *        1) DeInit CAMERA
  *        2) Init CAMERA
  *        3) Configure the DCMIPP for requested mode
  *        4) Configure the DUMP pipe
  *        4) Configure decimation and cropping for display on lcd
  *        5) Enable pipe and dcmipp
  * @retval
  *  None
  */
static void Camera_Config(void)
{
  int32_t ret;
  HAL_StatusTypeDef status;
  DCMIPP_PipeConfTypeDef     pipeconf;
  DCMIPP_ParallelConfTypeDef parallelcfg;
  DCMIPP_CropConfTypeDef     cropcfg;
  BSP_CAMERA_DeInit(0);
  Hdcmipp.Instance = DCMIPP;

  status = HAL_DCMIPP_DeInit(&Hdcmipp);

  if (status != HAL_OK)
  {
    Error_Handler();
  }

  HAL_DCMIPP_PIPE_Stop(&Hdcmipp, DCMIPP_PIPE0);

  ret = BSP_CAMERA_Init(0, CAMERA_R1600x1200, CAMERA_PF_RGB565);

  if (ret  != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  HAL_Delay(20);
  HAL_DCMIPP_Init(&Hdcmipp);

  parallelcfg.Format           = DCMIPP_FORMAT_RGB565;
  parallelcfg.SwapCycles       = DCMIPP_SWAPCYCLES_DISABLE;
  parallelcfg.VSPolarity       = DCMIPP_VSPOLARITY_LOW;
  parallelcfg.HSPolarity       = DCMIPP_HSPOLARITY_LOW;
  parallelcfg.PCKPolarity      = DCMIPP_PCKPOLARITY_FALLING;
  parallelcfg.ExtendedDataMode = DCMIPP_INTERFACE_8BITS;
  parallelcfg.SynchroMode      = DCMIPP_SYNCHRO_HARDWARE;
  HAL_DCMIPP_SetParallelConfig(&Hdcmipp, (const DCMIPP_ParallelConfTypeDef *) &parallelcfg);

  HAL_DCMIPP_PIPE_SetBytesDecimationConfig(&Hdcmipp, DCMIPP_PIPE0, DCMIPP_OEBS_EVEN, DCMIPP_BSM_2BYTE_OUT_4);
  HAL_DCMIPP_PIPE_SetLinesDecimationConfig(&Hdcmipp, DCMIPP_PIPE0, DCMIPP_OELS_ODD, DCMIPP_LSM_ALTERNATE_2);

  pipeconf.FrameRate = DCMIPP_FRAME_RATE_ALL;
  HAL_DCMIPP_PIPE_Config(&Hdcmipp, DCMIPP_PIPE0, (const DCMIPP_PipeConfTypeDef *) &pipeconf);

  cropcfg.HSize = 240U;
  cropcfg.HStart = 130U;
  cropcfg.PipeArea = DCMIPP_POSITIVE_AREA;
  cropcfg.VSize = 272U;
  cropcfg.VStart = 169U;
  HAL_DCMIPP_PIPE_SetCropConfig(&Hdcmipp, DCMIPP_PIPE0, (const DCMIPP_CropConfTypeDef *) &cropcfg);
  HAL_DCMIPP_PIPE_EnableCrop(&Hdcmipp, DCMIPP_PIPE0);
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
  while (1)
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
