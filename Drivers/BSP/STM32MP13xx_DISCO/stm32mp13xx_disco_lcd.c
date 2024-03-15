/**
  ******************************************************************************
  * @file    stm32mp13xx_disco_lcd.c
  * @author  MCD Application Team
  * @brief   This file includes the driver for Liquid Crystal Display (LCD) module
  *          mounted on STM32MP135F-DK board.
  @verbatim
  1. How To use this driver:
  --------------------------
     - This driver is used to drive directly an LCD TFT using the LTDC controller.
     - This driver uses timing and setting for RK043FN48H LCD.

  2. Driver description:
  ---------------------
    + Initialization steps:
       o Initialize the LCD using the BSP_LCD_Init() function.
       o Apply the Layer configuration using the BSP_LCD_LayerDefaultInit() function.
       o Select the LCD layer to be used using the BSP_LCD_SelectLayer() function.
       o Enable the LCD display using the BSP_LCD_DisplayOn() function.

    + Options
       o Configure and enable the color keying functionality using the
         BSP_LCD_SetColorKeying() function.
       o Modify in the fly the transparency and/or the frame buffer address
         using the following functions:
         - BSP_LCD_SetTransparency()
         - BSP_LCD_SetLayerAddress()
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Dependencies
- stm32mp13xx_disco.c
- stm32mp13xx_hal_ltdc.c
- stm32mp13xx_hal_ltdc_ex.c
- stm32mp13xx_hal_rcc_ex.c
- stm32mp13xx_hal_gpio.c
- stm32mp13xx_hal_cortex.c
- rk043fn48h.h
- fonts.h
- font24.c
- font20.c
- font16.c
- font12.c
- font8.c"
EndDependencies */

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_disco_lcd.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX_DISCO
  * @{
  */

/** @addtogroup STM32MP13XX_DISCO_LCD
  * @{
  */

/** @defgroup STM32MP13XX_DISCO_LCD_Private_TypesDefinitions STM32MP13XX_DISCO_LCD Private Types Definitions
  * @{
  */
const LCD_UTILS_Drv_t LCD_Driver =
{
  BSP_LCD_DrawBitmap,
  BSP_LCD_FillRGBRect,
  BSP_LCD_DrawHLine,
  BSP_LCD_DrawVLine,
  BSP_LCD_FillRect,
  BSP_LCD_ReadPixel,
  BSP_LCD_WritePixel,
  BSP_LCD_GetXSize,
  BSP_LCD_GetYSize,
  BSP_LCD_SelectLayer,
  BSP_LCD_GetPixelFormat

};
/**
  * @}
  */
/** @defgroup STM32MP13XX_DISCO_LCD_Private_Defines STM32MP13XX_DISCO LCD Private Defines
  * @{
  */
#define POLY_X(Z)              ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)              ((int32_t)((Points + Z)->Y))
#define PIXEL_PER_LINE         (480*2)
#define ALPHA_VALUE             255
/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_LCD_Private_Macros STM32MP13XX_DISCO_LCD Private Macros
  * @{
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))
/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_LCD_Private_Variables STM32MP13XX_DISCO_LCD Private Variables
  * @{
  */
LTDC_HandleTypeDef  hLtdcHandler;

/* Default LCD configuration with LCD Layer 1 */
static uint32_t            ActiveLayer = 0;
static LCD_DrawPropTypeDef DrawProp[MAX_LAYER_NUMBER];
/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_LCD_Private_FunctionPrototypes STM32MP13XX_DISCO_LCD Private Function Prototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_LCD_Exported_Functions STM32MP13XX_DISCO_LCD Exported Functions
  * @{
  */

/**
  * @brief  Initializes the LCD.
  * @param  Instance    LCD Instance
  * @retval LCD state
  */
int32_t BSP_LCD_Init(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {

    /* DeInit */
    if (HAL_LTDC_DeInit(&hLtdcHandler) != HAL_OK)
    {
      return LCD_ERROR;
    }

    /* Select the used LCD */

    /* The RK043FN48H LCD 480x272 is selected */
    /* Timing Configuration */
    hLtdcHandler.Init.HorizontalSync = (RK043FN48H_HSYNC - (uint16_t)1);
    hLtdcHandler.Init.VerticalSync = (RK043FN48H_VSYNC - (uint16_t)1);
    hLtdcHandler.Init.AccumulatedHBP = (RK043FN48H_HSYNC + RK043FN48H_HBP - (uint16_t)1);
    hLtdcHandler.Init.AccumulatedVBP = (RK043FN48H_VSYNC + RK043FN48H_VBP - (uint16_t)1);
    hLtdcHandler.Init.AccumulatedActiveH = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP - (uint16_t)1);
    hLtdcHandler.Init.AccumulatedActiveW = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + RK043FN48H_HBP - (uint16_t)1);
    hLtdcHandler.Init.TotalHeigh = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP +
                                    RK043FN48H_VFP - (uint16_t)1);
    hLtdcHandler.Init.TotalWidth = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + RK043FN48H_HBP +
                                    RK043FN48H_HFP - (uint16_t)1);

    /* LCD clock configuration */
    BSP_LCD_ClockConfig(&hLtdcHandler, NULL);

    /* Initialize the LCD pixel width and pixel height */
    hLtdcHandler.LayerCfg->ImageWidth  = RK043FN48H_WIDTH;
    hLtdcHandler.LayerCfg->ImageHeight = RK043FN48H_HEIGHT;

    /* Background value */
    hLtdcHandler.Init.Backcolor.Blue = 0;
    hLtdcHandler.Init.Backcolor.Green = 0;
    hLtdcHandler.Init.Backcolor.Red = 0;

    /* Polarity */
    hLtdcHandler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hLtdcHandler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hLtdcHandler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hLtdcHandler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hLtdcHandler.Instance = LTDC;

    if (HAL_LTDC_GetState(&hLtdcHandler) == HAL_LTDC_STATE_RESET)
    {
      /* Initialize the LCD Msp: this __weak function can be rewritten by the application */
      BSP_LCD_MspInit(&hLtdcHandler, NULL);
    }
    if (HAL_LTDC_Init(&hLtdcHandler) != HAL_OK)
    {
      return LCD_ERROR;
    }

  }

  return ret;
}

/**
  * @brief  DeInitializes the LCD.
  * @param  Instance    LCD Instance
  * @retval LCD state
  */
int32_t BSP_LCD_DeInit(uint32_t Instance)
{

  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Initialize the hLtdcHandler Instance parameter */
    hLtdcHandler.Instance = LTDC;

    /* Disable LTDC block */
    __HAL_LTDC_DISABLE(&hLtdcHandler);

    /* DeInit the LTDC */
    if (HAL_LTDC_DeInit(&hLtdcHandler) != HAL_OK)
    {
      return LCD_ERROR;
    }

    /* DeInit the LTDC MSP : this __weak function can be rewritten by the application */
    BSP_LCD_MspDeInit(&hLtdcHandler, NULL);
  }
  return ret;
}

/**
  * @brief  Gets the LCD X size.
  * @param  Instance LCD Instance
  * @retval XSize pointer Used LCD X size
  */
int32_t BSP_LCD_GetXSize(uint32_t Instance, uint32_t *XSize)
{

  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    *XSize = hLtdcHandler.LayerCfg[ActiveLayer].ImageWidth;
  }

  return ret;
}

/**
  * @brief  Gets the LCD Y size.
  * @param  Instance LCD Instance
  * @retval YSize pointer Used LCD Y size
  */
int32_t BSP_LCD_GetYSize(uint32_t Instance, uint32_t *YSize)
{

  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    *YSize = hLtdcHandler.LayerCfg[ActiveLayer].ImageHeight;
  }

  return ret;
}

/**
  * @brief  Set the LCD X size.
  * @param  Instance    LCD Instance
  * @param  imageWidthPixels : image width in pixels unit
  * @retval None
  */
int32_t BSP_LCD_SetXSize(uint32_t Instance, uint32_t imageWidthPixels)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    hLtdcHandler.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
  }
  return ret;
}

/**
  * @brief  Set the LCD Y size.
  * @param  Instance    LCD Instance
  * @param  imageHeightPixels : image height in lines unit
  * @retval None
  */
int32_t BSP_LCD_SetYSize(uint32_t Instance, uint32_t imageHeightPixels)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    hLtdcHandler.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
  }
  return ret;

}

/**
  * @brief  Initializes the LCD layer in ARGB8888 format (32 bits per pixel).
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
int32_t BSP_LCD_LayerDefaultInit(uint32_t Instance, uint16_t LayerIndex, uint32_t FB_Address)
{

  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    LCD_LayerCfgTypeDef  layer_cfg;
    /* Layer Init */
    BSP_LCD_GetLTDCPixelFormat(0, LCD_PIXEL_FORMAT_ARGB8888, &layer_cfg.PixelFormat);
    layer_cfg.WindowX0 = 0;
    BSP_LCD_GetXSize(0, &layer_cfg.WindowX1);
    layer_cfg.WindowY0 = 0;
    BSP_LCD_GetYSize(0, &layer_cfg.WindowY1);
    layer_cfg.FBStartAdress = FB_Address;
    layer_cfg.Alpha = ALPHA_VALUE;
    layer_cfg.Alpha0 = 0;
    layer_cfg.Backcolor.Blue = 0;
    layer_cfg.Backcolor.Green = 0;
    layer_cfg.Backcolor.Red = 0;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    BSP_LCD_GetXSize(0, &layer_cfg.ImageWidth);
    BSP_LCD_GetYSize(0, &layer_cfg.ImageHeight);
    layer_cfg.HorMirrorEn = 0;
    layer_cfg.VertMirrorEn = 0;

    if (HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }

    if (HAL_LTDC_ProgramLineEvent(&hLtdcHandler, BSP_LCD_GetYSize(0, &layer_cfg.ImageHeight) / (uint32_t)2) != HAL_OK)
    {
      return LCD_ERROR;
    }

    DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
    DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;
  }

  return ret;
}

/**
  * @brief  Initializes the LCD layer in RGB565 format (16 bits per pixel).
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
int32_t BSP_LCD_LayerRgb565Init(uint32_t Instance, uint16_t LayerIndex, uint32_t FB_Address)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    LCD_LayerCfgTypeDef  layer_cfg;
    /* Layer Init */
    BSP_LCD_GetLTDCPixelFormat(0, LCD_PIXEL_FORMAT_RGB565, &layer_cfg.PixelFormat);
    layer_cfg.WindowX0 = 0;
    BSP_LCD_GetXSize(0, &layer_cfg.WindowX1);
    layer_cfg.WindowY0 = 0;
    BSP_LCD_GetYSize(0, &layer_cfg.WindowY1);
    layer_cfg.FBStartAdress = FB_Address;
    layer_cfg.Alpha = ALPHA_VALUE;
    layer_cfg.Alpha0 = 0;
    layer_cfg.Backcolor.Blue = 0;
    layer_cfg.Backcolor.Green = 0;
    layer_cfg.Backcolor.Red = 0;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    BSP_LCD_GetXSize(0, &layer_cfg.ImageWidth);
    BSP_LCD_GetYSize(0, &layer_cfg.ImageHeight);
    layer_cfg.HorMirrorEn = 0;
    layer_cfg.VertMirrorEn = 0;
    if (HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }

    DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
    DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;
  }
  return  ret;
}

/**
  * @brief  Selects the LCD Layer.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
int32_t BSP_LCD_SelectLayer(uint32_t Instance, uint32_t LayerIndex)
{
  uint32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    ActiveLayer = LayerIndex;
  }
  return ret;
}

/**
  * @brief  Sets an LCD Layer visible
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Visible Layer
  * @param  State: New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE
  * @retval None
  */
int32_t BSP_LCD_SetLayerVisible(uint32_t Instance, uint32_t LayerIndex, FunctionalState State)
{
  uint32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (State == ENABLE)
    {
      __HAL_LTDC_LAYER_ENABLE(&hLtdcHandler, LayerIndex);
    }
    else
    {
      __HAL_LTDC_LAYER_DISABLE(&hLtdcHandler, LayerIndex);
    }
    __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&hLtdcHandler);
  }
  return ret;
}

/**
  * @brief  Sets an LCD Layer visible without reloading.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Visible Layer
  * @param  State: New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE
  * @retval None
  */
int32_t BSP_LCD_SetLayerVisible_NoReload(uint32_t Instance, uint32_t LayerIndex, FunctionalState State)
{
  uint32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (State == ENABLE)
    {
      __HAL_LTDC_LAYER_ENABLE(&hLtdcHandler, LayerIndex);
    }
    else
    {
      __HAL_LTDC_LAYER_DISABLE(&hLtdcHandler, LayerIndex);
    }
  }
  return ret;
  /* Do not Sets the Reload  */
}

/**
  * @brief  Gets the LCD Active LCD Pixel Format.
  * @param  Instance    LCD Instance
  * @param  PixelFormat Active LCD Pixel Format
  * @retval BSP status
  */
int32_t BSP_LCD_GetPixelFormat(uint32_t Instance, uint32_t *PixelFormat)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t LCDPixelFormat;
    BSP_LCD_GetLCDPixelFormat(Instance, hLtdcHandler.LayerCfg[Instance].PixelFormat, &LCDPixelFormat);
    *PixelFormat = LCDPixelFormat;
  }

  return ret;
}
/**
  * @brief  Gets the LCD Active LTDC Pixel Format.
  * @param  Instance    LCD Instance
  * @param  PixelFormat Active LCD Pixel Format
  * @param  LTDCPixelFormat Pointer to Active LTDC Pixel Format
  * @retval BSP status
  */
int32_t BSP_LCD_GetLTDCPixelFormat(uint32_t Instance, uint32_t PixelFormat, uint32_t *LTDCPixelFormat)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {

    switch (PixelFormat)
    {
      case LCD_PIXEL_FORMAT_RGB565:
        *LTDCPixelFormat = LTDC_PIXEL_FORMAT_RGB565;
        break;
      case LCD_PIXEL_FORMAT_ARGB8888:
        *LTDCPixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
        break;
      default:
        *LTDCPixelFormat = LTDC_PIXEL_FORMAT_RGB565;
        break;
    }
  }

  return ret;
}

/**
  * @brief  Gets the LCD Active LCD Pixel Format.
  * @param  Instance    LCD Instance
  * @param  PixelFormat Active LTDC Pixel Format
  * @param  LCDPixelFormat Pointer to Active LCD Pixel Format
  * @retval BSP status
  */
int32_t BSP_LCD_GetLCDPixelFormat(uint32_t Instance, uint32_t PixelFormat, uint32_t *LCDPixelFormat)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {

    switch (PixelFormat)
    {
      case LTDC_PIXEL_FORMAT_RGB565:
        *LCDPixelFormat = LCD_PIXEL_FORMAT_RGB565;
        break;
      case LTDC_PIXEL_FORMAT_ARGB8888:
        *LCDPixelFormat = LCD_PIXEL_FORMAT_ARGB8888;
        break;
      default:
        *LCDPixelFormat = LCD_PIXEL_FORMAT_RGB565;
        break;
    }
  }

  return ret;
}
/**
  * @brief  Configures the transparency.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background.
  * @param  Transparency: Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
  * @retval None
  */
int32_t BSP_LCD_SetTransparency(uint32_t Instance, uint32_t LayerIndex, uint8_t Transparency)
{
  uint32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (HAL_LTDC_SetAlpha(&hLtdcHandler, Transparency, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Configures the transparency without reloading.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background.
  * @param  Transparency: Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
  * @retval None
  */
int32_t BSP_LCD_SetTransparency_NoReload(uint32_t Instance, uint32_t LayerIndex, uint8_t Transparency)
{
  uint32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (HAL_LTDC_SetAlpha_NoReload(&hLtdcHandler, Transparency, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Sets an LCD layer frame buffer address.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background
  * @param  Address: New LCD frame buffer value
  * @retval None
  */
int32_t BSP_LCD_SetLayerAddress(uint32_t Instance, uint32_t LayerIndex, uint32_t Address)
{
  uint32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (HAL_LTDC_SetAddress(&hLtdcHandler, Address, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Sets an LCD layer frame buffer address without reloading.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background
  * @param  Address: New LCD frame buffer value
  * @retval None
  */
int32_t BSP_LCD_SetLayerAddress_NoReload(uint32_t Instance, uint32_t LayerIndex, uint32_t Address)
{
  uint32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (HAL_LTDC_SetAddress_NoReload(&hLtdcHandler, Address, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;

}

/**
  * @brief  Sets display window.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height
  * @retval None
  */
int32_t BSP_LCD_SetLayerWindow(uint32_t Instance, uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width,
                               uint16_t Height)
{
  uint32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Reconfigure the layer size */
    if (HAL_LTDC_SetWindowSize(&hLtdcHandler, Width, Height, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }

    /* Reconfigure the layer position */
    if (HAL_LTDC_SetWindowPosition(&hLtdcHandler, Xpos, Ypos, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Sets display window without reloading.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height
  * @retval None
  */
int32_t BSP_LCD_SetLayerWindow_NoReload(uint32_t Instance, uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos,
                                        uint16_t Width, uint16_t Height)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Reconfigure the layer size */
    if (HAL_LTDC_SetWindowSize_NoReload(&hLtdcHandler, Width, Height, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }

    /* Reconfigure the layer position */
    if (HAL_LTDC_SetWindowPosition_NoReload(&hLtdcHandler, Xpos, Ypos, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Configures and sets the color keying.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background
  * @param  RGBValue: Color reference
  * @retval None
  */
int32_t BSP_LCD_SetColorKeying(uint32_t Instance, uint32_t LayerIndex, uint32_t RGBValue)
{

  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Configure and Enable the color Keying for LCD Layer */
    if (HAL_LTDC_ConfigColorKeying(&hLtdcHandler, RGBValue, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }

    if (HAL_LTDC_EnableColorKeying(&hLtdcHandler, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Configures and sets the color keying without reloading.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background
  * @param  RGBValue: Color reference
  * @retval None
  */
int32_t BSP_LCD_SetColorKeying_NoReload(uint32_t Instance, uint32_t LayerIndex, uint32_t RGBValue)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Configure and Enable the color Keying for LCD Layer */
    if (HAL_LTDC_ConfigColorKeying_NoReload(&hLtdcHandler, RGBValue, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }

    if (HAL_LTDC_EnableColorKeying_NoReload(&hLtdcHandler, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Disables the color keying.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
int32_t BSP_LCD_ResetColorKeying(uint32_t Instance, uint32_t LayerIndex)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Disable the color Keying for LCD Layer */
    if (HAL_LTDC_DisableColorKeying(&hLtdcHandler, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Disables the color keying without reloading.
  * @param  Instance    LCD Instance
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
int32_t BSP_LCD_ResetColorKeying_NoReload(uint32_t Instance, uint32_t LayerIndex)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Disable the color Keying for LCD Layer */
    if (HAL_LTDC_DisableColorKeying_NoReload(&hLtdcHandler, LayerIndex) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Disables the color keying without reloading.
  * @param  Instance    LCD Instance
  * @param  ReloadType: can be one of the following values
  *         - LCD_RELOAD_IMMEDIATE
  *         - LCD_RELOAD_VERTICAL_BLANKING
  * @retval None
  */
int32_t BSP_LCD_Reload(uint32_t Instance, uint32_t ReloadType)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (HAL_LTDC_Reload(&hLtdcHandler, ReloadType) != HAL_OK)
    {
      return LCD_ERROR;
    }
  }
  return ret;
}

/**
  * @brief  Sets the LCD text color.
  * @param  Instance    LCD Instance
  * @param  Color: Text color code ARGB(8-8-8-8)
  * @retval None
  */
int32_t BSP_LCD_SetTextColor(uint32_t Instance, uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    DrawProp[ActiveLayer].TextColor = Color;
  }
  return ret;
}

/**
  * @brief  Gets the LCD text color.
  * @param  Instance    LCD Instance
  * @retval Used text color.
  */
int32_t BSP_LCD_GetTextColor(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    return DrawProp[ActiveLayer].TextColor;
  }
  return ret;
}

/**
  * @brief  Sets the LCD background color.
  * @param  Instance    LCD Instance
  * @param  Color: Layer background color code ARGB(8-8-8-8)
  * @retval None
  */
int32_t BSP_LCD_SetBackColor(uint32_t Instance, uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    DrawProp[ActiveLayer].BackColor = Color;
  }
  return ret;
}

/**
  * @brief  Gets the LCD background color.
  * @param  Instance    LCD Instance
  * @retval Used background colour
  */
int32_t BSP_LCD_GetBackColor(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    return DrawProp[ActiveLayer].BackColor;
  }
  return ret;
}


/**
  * @brief  Enables the display.
  * @param  Instance    LCD Instance
  * @retval None
  */
int32_t BSP_LCD_DisplayOn(uint32_t Instance)
{

  int32_t ret = BSP_ERROR_NONE;
  GPIO_InitTypeDef GPIO_Init_Structure;
  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    LCD_RST_GPIO_CLK_ENABLE();
    /* Configure the GPIO RST pin */
    GPIO_Init_Structure.Pin   = LCD_RST_PIN;
    GPIO_Init_Structure.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_Init_Structure.Pull  = LCD_RST_PULL;
    GPIO_Init_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_Init_Structure);

    /* Activate XRES active low */
    HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);/* wait 20 ms */
    HAL_GPIO_WritePin(LCD_RST_GPIO_PORT, LCD_RST_PIN, GPIO_PIN_SET); /* Deactivate XRES */
    HAL_Delay(10);/* Wait for 10ms after releasing XRES before sending commands */

    GPIO_Init_Structure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_Init_Structure.Pin       = LCD_DISP_CTRL_PIN; /* BL_CTRL */
    HAL_GPIO_Init(LCD_DISP_CTRL_GPIO_PORT, &GPIO_Init_Structure);

    /* Assert LCD_DISP_EN pin */
    HAL_GPIO_WritePin(LCD_DISP_CTRL_GPIO_PORT, LCD_DISP_CTRL_PIN, GPIO_PIN_SET);

    /* re-connect the BL pin to the brightness TIMER */
    GPIO_Init_Structure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_Init_Structure.Alternate = GPIO_AF1_TIM1;
    GPIO_Init_Structure.Pin       = LCD_BL_CTRL_PIN; /* BL_CTRL */
    HAL_GPIO_Init(LCD_BL_CTRL_GPIO_PORT, &GPIO_Init_Structure);

    HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);

    /* Display On */
    __HAL_LTDC_ENABLE(&hLtdcHandler);
  }
  return ret;
}

/**
  * @brief  Disables the display.
  * @param  Instance    LCD Instance
  * @retval None
  */
int32_t BSP_LCD_DisplayOff(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    GPIO_InitTypeDef GPIO_Init_Structure;

    GPIO_Init_Structure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_Init_Structure.Pin       = LCD_DISP_CTRL_PIN; /* BL_CTRL */
    HAL_GPIO_Init(LCD_DISP_CTRL_GPIO_PORT, &GPIO_Init_Structure);

    /* Assert LCD_DISP_EN pin */
    HAL_GPIO_WritePin(LCD_DISP_CTRL_GPIO_PORT, LCD_DISP_CTRL_PIN, GPIO_PIN_RESET);

    /* re-connect the BL pin to the brightness TIMER */
    GPIO_Init_Structure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_Init_Structure.Alternate = GPIO_AF1_TIM1;
    GPIO_Init_Structure.Pin       = LCD_BL_CTRL_PIN; /* BL_CTRL */
    HAL_GPIO_Init(LCD_BL_CTRL_GPIO_PORT, &GPIO_Init_Structure);

    HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_RESET);

    /* Display Off */
    __HAL_LTDC_DISABLE(&hLtdcHandler);
  }
  return ret;
}

/**
  * @brief  Draw a horizontal line on LCD.
  * @param  Instance LCD Instance.
  * @param  Xpos X position.
  * @param  Ypos Y position.
  * @param  pData Pointer to RGB line data
  * @param  Width Rectangle width.
  * @param  Height Rectangle Height.
  * @retval BSP status.
  */
int32_t BSP_LCD_FillRGBRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width,
                            uint32_t Height)
{
  int32_t ret = BSP_ERROR_NONE;
  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t Xaddress;
    uint32_t StartAddress;
    uint32_t i;
    uint32_t j;
    uint32_t LCDPixelFormat;

    BSP_LCD_GetPixelFormat(Instance, &LCDPixelFormat);

    if (LCDPixelFormat == LCD_PIXEL_FORMAT_RGB565)
    {
      /* Set the start address */
      StartAddress = (hLtdcHandler.LayerCfg[Instance].FBStartAdress);
      StartAddress = StartAddress + (2 * (Ypos * (hLtdcHandler.LayerCfg[Instance].ImageWidth) + Xpos));

      /* Fill the rectangle */
      for (i = 0; i < Height; i++)
      {
        Xaddress =  StartAddress + (PIXEL_PER_LINE * i);
        for (j = 0; j < Width; j++)
        {
          *(__IO uint16_t *)(Xaddress)  = *(uint16_t *)(pData + (2U * j));
          Xaddress += 2U;
        }
      }
    }
    else if (LCDPixelFormat == LCD_PIXEL_FORMAT_ARGB8888)
    {
      /* Set the start address */
      StartAddress = (hLtdcHandler.LayerCfg[Instance].FBStartAdress);
      StartAddress = StartAddress + (4 * (Ypos * (hLtdcHandler.LayerCfg[Instance].ImageWidth) + Xpos));


      /* Fill the rectangle */
      for (i = 0; i < Height; i++)
      {
        Xaddress =  StartAddress + (2 * PIXEL_PER_LINE * i);
        for (j = 0; j < Width; j++)
        {
          *(__IO uint32_t *)(Xaddress)  = *(uint32_t *)(pData + (4U * j));
          Xaddress += 4U;
        }
      }
    }
    /* Flushing cache*/
#if defined(CACHE_USE)
    L1C_CleanInvalidateDCacheAll();
#endif /* CACHE_USE */
  }
  return ret;
}

/**
  * @brief  Draws a full rectangle in currently active layer.
  * @param  Instance   LCD Instance
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Width Rectangle width
  * @param  Height Rectangle height
  * @param  Color RGB color
  * @retval BSP status
  */
int32_t BSP_LCD_FillRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height,
                         uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;
  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t ref = Ypos + Height;
    /* Write line */
    while (Ypos < ref)
    {
      ret = BSP_LCD_DrawHLine(Instance, Xpos, Ypos++, Width, Color);
    }
  }
  return ret;
}

/**
  * @brief  Draws an horizontal line in currently active layer.
  * @param  Instance   LCD Instance
  * @param  Xpos  X position
  * @param  Ypos  Y position
  * @param  Length  Line length
  * @param  Color RGB color
  * @retval BSP status
  */
int32_t BSP_LCD_DrawHLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;
  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint16_t xRef = Xpos;
    /* Write line */
    while (Xpos < Length + xRef)
    {
      ret = BSP_LCD_WritePixel(Instance, Xpos++, Ypos, Color);
    }
    /* Flushing cache*/
#if defined(CACHE_USE)
    L1C_CleanInvalidateDCacheAll();
#endif /* CACHE_USE */
  }
  return ret;
}

/**
  * @brief  Draws a vertical line in currently active layer.
  * @param  Instance   LCD Instance
  * @param  Xpos  X position
  * @param  Ypos  Y position
  * @param  Length  Line length
  * @param  Color RGB color
  * @retval BSP status
  */
int32_t BSP_LCD_DrawVLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  int32_t ret = BSP_ERROR_NONE;
  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint16_t yRef = Ypos;

    while (Ypos < Length + yRef)
    {
      ret = BSP_LCD_WritePixel(Instance, Xpos, Ypos++, Color);
    }
    /* Flushing cache*/
#if defined(CACHE_USE)
    L1C_CleanInvalidateDCacheAll();
#endif /* CACHE_USE */
  }
  return ret;

}

/**
  * @brief  Reads a LCD pixel.
  * @param  Instance    LCD Instance
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Color RGB pixel color
  * @retval BSP status
  */
int32_t BSP_LCD_ReadPixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t *Color)
{
  int32_t ret = BSP_ERROR_NONE;
  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t LCDPixelFormat;
    BSP_LCD_GetPixelFormat(Instance, &LCDPixelFormat);

    if (LCDPixelFormat == LCD_PIXEL_FORMAT_RGB565)
    {
      /* Read data value from RAM memory */
      *Color = *(__IO uint16_t *)(hLtdcHandler.LayerCfg[Instance].FBStartAdress +
                                  (2 * (Ypos * (hLtdcHandler.LayerCfg[Instance].ImageWidth) + Xpos)));
    }
    else if (LCDPixelFormat == LCD_PIXEL_FORMAT_ARGB8888)
    {
      /* Read data value from RAM memory */
      *Color = *(__IO uint32_t *)(hLtdcHandler.LayerCfg[Instance].FBStartAdress +
                                  (4 * (Ypos * (hLtdcHandler.LayerCfg[Instance].ImageWidth) + Xpos)));
    }
  }
  return ret;
}
/**
  * @brief  Draws a pixel on LCD.
  * @param  Instance LCD Instance
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  RGB_Code Pixel color
  * @retval BSP status
  */
int32_t BSP_LCD_WritePixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t RGB_Code)
{
  int32_t ret = BSP_ERROR_NONE;
  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t LCDPixelFormat;
    BSP_LCD_GetPixelFormat(Instance, &LCDPixelFormat);
    /* Write data value to all SDRAM memory */
    if (LCDPixelFormat == LCD_PIXEL_FORMAT_RGB565)
    {
      /* RGB565 format */
      *(__IO uint16_t *)(hLtdcHandler.LayerCfg[Instance].FBStartAdress + \
                         (2 * (Ypos * (hLtdcHandler.LayerCfg[Instance].ImageWidth) + Xpos))) = (uint16_t)RGB_Code;
    }
    else if (LCDPixelFormat == LCD_PIXEL_FORMAT_ARGB8888)
    {
      /* ARGB8888 format */
      *(__IO uint32_t *)(hLtdcHandler.LayerCfg[Instance].FBStartAdress + \
                         (4 * (Ypos * (hLtdcHandler.LayerCfg[Instance].ImageWidth) + Xpos))) = RGB_Code;
    }
  }
  return ret;
}

/**
  * @brief  Draws a bitmap picture loaded in the internal Flash in currently active layer.
  * @param  Instance LCD Instance
  * @param  Xpos Bmp X position in the LCD
  * @param  Ypos Bmp Y position in the LCD
  * @param  pBmp Pointer to Bmp picture address in the internal Flash.
  * @retval BSP status
  */
int32_t BSP_LCD_DrawBitmap(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp)
{
  int32_t ret = BSP_ERROR_NONE;

  if (Instance >= LCD_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    uint32_t index;
    uint32_t width;
    uint32_t height;
    uint32_t bit_pixel;
    uint32_t StartAddress;
    uint32_t xStartAddress;
    uint32_t input_color_mode;

    /* Get bitmap data address offset */
    index = (uint32_t)pBmp[10] + ((uint32_t)pBmp[11] << 8) + ((uint32_t)pBmp[12] << 16)  + ((uint32_t)pBmp[13] << 24);

    /* Read bitmap width */
    width = (uint32_t)pBmp[18] + ((uint32_t)pBmp[19] << 8) + ((uint32_t)pBmp[20] << 16)  + ((uint32_t)pBmp[21] << 24);

    /* Read bitmap height */
    height = (uint32_t)pBmp[22] + ((uint32_t)pBmp[23] << 8) + ((uint32_t)pBmp[24] << 16)  + ((uint32_t)pBmp[25] << 24);

    /* Read bit/pixel */
    bit_pixel = (uint32_t)pBmp[28] + ((uint32_t)pBmp[29] << 8);


    /* Get the layer pixel format */
    if ((bit_pixel / 8U) == 2U)
    {
      input_color_mode = LCD_PIXEL_FORMAT_RGB565;
      /* Set the address */
      StartAddress = (hLtdcHandler.LayerCfg[Instance].FBStartAdress);
      StartAddress = StartAddress + (2 * (Ypos * (hLtdcHandler.LayerCfg[Instance].ImageWidth) + Xpos));
    }
    else if ((bit_pixel / 8U) == 4U)
    {
      input_color_mode = LCD_PIXEL_FORMAT_ARGB8888;
      /* Set the address */
      StartAddress = (hLtdcHandler.LayerCfg[Instance].FBStartAdress);
      StartAddress = StartAddress + (4 * (Ypos * (hLtdcHandler.LayerCfg[Instance].ImageWidth) + Xpos));
    }


    /*Read the Pixel*/
    uint32_t size =  width * height;
    Pixel_t pixels[MAX_PIXEL_SIZE];
    uint16_t i;

    for (i = 0; i < size; i++)
    {
      if (input_color_mode == LCD_PIXEL_FORMAT_RGB565)
      {
        uint16_t temp_pixel = (pBmp[index + i * 2 + 1] << 8) | pBmp[index + i * 2];
        pixels[i].red = (temp_pixel & 0xF800) >> 8;
        pixels[i].green = (temp_pixel & 0x07E0) >> 3;
        pixels[i].blue = (temp_pixel & 0x001F) << 3;
      }
      else if (input_color_mode == LCD_PIXEL_FORMAT_ARGB8888)
      {
        uint32_t temp_pixel = (pBmp[index + i * 4 + 3] << 24) | (pBmp[index + i * 4 + 2] << 16) |
                              (pBmp[index + i * 4 + 1] << 8) | pBmp[index + i * 4];
        pixels[i].alpha = (temp_pixel & 0xFF000000) >> 24 ;
        pixels[i].red = (temp_pixel & 0x00FF0000) >> 16;
        pixels[i].green = (temp_pixel & 0x0000FF00) >> 8;
        pixels[i].blue = temp_pixel & 0x000000FF;
      }
    }

    /* Write the pixel data to the display */
    if (input_color_mode == LCD_PIXEL_FORMAT_RGB565)
    {
      int16_t y;
      int16_t x;
      for (y = height - 1; y >= 0; y--)
      {
        xStartAddress = StartAddress;
        for (x = 0; x < width; x++)
        {
          uint16_t rgbCode;
          rgbCode = ((pixels[y * width + x].red & 0xF8) << 8) |
                    ((pixels[y * width + x].green & 0xFC) << 3) |
                    (pixels[y * width + x].blue >> 3);


          *(__IO uint16_t *)(xStartAddress) = rgbCode;
          xStartAddress += 2U;
        }
        StartAddress += hLtdcHandler.LayerCfg[Instance].ImageWidth * 2;
      }
    }
    else if ((input_color_mode == LCD_PIXEL_FORMAT_ARGB8888))
    {
      int32_t y;
      int32_t x;
      for (y = height - 1; y >= 0; y--)
      {
        xStartAddress = StartAddress;
        for (x = 0; x < width; x++)
        {
          uint32_t argbCode;
          argbCode = (pixels[y * width + x].alpha << 24) |
                     (pixels[y * width + x].red << 16) |
                     (pixels[y * width + x].green << 8) |
                     pixels[y * width + x].blue;
          *(__IO uint32_t *)(xStartAddress) = argbCode;
          xStartAddress += 4U;
        }
        StartAddress += hLtdcHandler.LayerCfg[Instance].ImageWidth * 4;
      }
    }
    /* Flushing cache*/
#if defined(CACHE_USE)
    L1C_CleanInvalidateDCacheAll();
#endif /* CACHE_USE */
  }
  return ret;
}


/**
  * @brief  Initializes the LTDC MSP.
  * @param  hltdc: LTDC handle
  * @param  Params
  * @retval None
  */
__weak void BSP_LCD_MspInit(LTDC_HandleTypeDef *hltdc, void *Params)
{
  GPIO_InitTypeDef GPIO_Init_Structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);
  UNUSED(hltdc);

  /* Enable the LTDC clocks */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*
   * CLK  ----> D9  (AF13)
   * HSYNC----> C6  (AF14)
   * VSYNC----> G4  (AF11)
   * DE   ----> H9  (AF11)
   *
   * R2   ----> G7  (AF14)
   * R3   ----> B12 (AF13)
   * R4   ----> D14 (AF14)
   * R5   ----> E7  (AF14)
   * R6   ----> E13 (AF14)
   * R7   ----> E9  (AF14)
   *
   * G2   ----> H3  (AF14)
   * G3   ----> F3  (AF14)
   * G4   ----> D5  (AF14)
   * G5   ----> G0  (AF14)
   * G6   ----> C7  (AF14)
   * G7   ----> A15 (AF11)
   *
   * B2   ----> D10 (AF14)
   * B3   ----> F2  (AF14)
   * B4   ----> H4  (AF11)
   * B5   ----> E0  (AF14)
   * B6   ----> B6  (AF7)
   * B7   ----> F1  (AF13)
   * */

  /*** LTDC Pins configuration ***/
  /* ('D', 9, AF13) LCD_CLK */
  GPIO_Init_Structure.Pin       = GPIO_PIN_9;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_LCD;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* ('C', 6, AF14) LCD_HSYNC */
  GPIO_Init_Structure.Pin       = GPIO_PIN_6;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

  /* ('G',  4, AF11) LCD_VSYNC */
  GPIO_Init_Structure.Pin       = GPIO_PIN_4;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF11_LCD;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /* ('H',  9, AF11) LCD_DE */
  GPIO_Init_Structure.Pin       = GPIO_PIN_9;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF11_LCD;
  HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

  /* ('G', 7, AF14) LCD_R2 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_7;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /* ('B', 12, AF13) LCD_R3 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_12;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_LCD;
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  /* ('D', 14, AF14) LCD_R4 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_14;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* ('E',  7, AF14) LCD_R5 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_7;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* ('E',  13, AF14) LCD_R6 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_13;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* ('E',  9, AF14) LCD_R7 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_9;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* ('H', 3, AF14) LCD_G2 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_3;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

  /* ('F',  3, AF14) LCD_G3 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_3;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

  /* ('D',  5, AF14) LCD_G4 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_5;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* ('G',  0, AF14) LCD_G5 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_0;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /* ('C',  7, AF14) LCD_G6 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_7;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

  /* ('A', 15, AF11) LCD_G7 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_15;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF11_LCD;
  HAL_GPIO_Init(GPIOA, &GPIO_Init_Structure);

  /* ('D',  10, AF14) LCD_B2 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_10;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* ('F',  2, AF14) LCD_B3 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

  /* ('H',  4, AF11) LCD_B4 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_4;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF11_LCD;
  HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

  /* ('E',  0, AF14) LCD_B5 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_0;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* ('B',  6, AF7)  LCD_B6 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_6;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  /* ('F',  1, AF13) LCD_B7 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_1;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_LCD;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

  /* GIC configuration for LTDC interrupt */
  IRQ_SetPriority(LTDC_IRQn, 0);
  IRQ_Enable(LTDC_IRQn);

  /* Enable LTDC reset state */
  __HAL_RCC_LTDC_FORCE_RESET();

  /* Release LTDC from reset state */
  __HAL_RCC_LTDC_RELEASE_RESET();

}

/**
  * @brief  DeInitializes BSP_LCD MSP.
  * @param  hltdc: LTDC handle
  * @param  Params
  * @retval None
  */
__weak void BSP_LCD_MspDeInit(LTDC_HandleTypeDef *hltdc, void *Params)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Params);

  /* Disable LTDC block */
  __HAL_LTDC_DISABLE(hltdc);

  /*
   * CLK  ----> D9  (AF13)
   * HSYNC----> C6  (AF14)
   * VSYNC----> G4  (AF11)
   * DE   ----> H9  (AF11)
   *
   * R2   ----> G7  (AF14)
   * R3   ----> B12 (AF13)
   * R4   ----> D14 (AF14)
   * R5   ----> E7  (AF14)
   * R6   ----> E13 (AF14)
   * R7   ----> E9  (AF14)
   *
   * G2   ----> H3  (AF14)
   * G3   ----> F3  (AF14)
   * G4   ----> D5  (AF14)
   * G5   ----> G0  (AF14)
   * G6   ----> C7  (AF14)
   * G7   ----> A15 (AF11)
   *
   * B2   ----> D10 (AF14)
   * B3   ----> F2  (AF14)
   * B4   ----> H4  (AF11)
   * B5   ----> E0  (AF14)
   * B6   ----> B6  (AF7)
   * B7   ----> F1  (AF13)
   * */

  /* ('D', 9, AF13) LCD_CLK */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_9);

  /* ('C', 6, AF14) LCD_HSYNC */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6);

  /* ('G',  4, AF11) LCD_VSYNC */
  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_4);

  /* ('H',  9, AF11) LCD_DE */
  HAL_GPIO_DeInit(GPIOH, GPIO_PIN_9);

  /* ('H',  8, AF13) LCD_R2 */
  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_7);

  /* ('B', 12, AF14) LCD_R3 */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12);

  /* ('E',  3, AF14) LCD_R4 */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_4);

  /* ('F',  5, AF14) LCD_R5 */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7);

  /* ('C',  6, AF14) LCD_R6 */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_13);

  /* ('G',  6, AF14) LCD_R7 */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_9);

  /* ('H', 13, AF14) LCD_G2 */
  HAL_GPIO_DeInit(GPIOH, GPIO_PIN_3);

  /* ('F',  3, AF14) LCD_G3 */
  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_3);

  /* ('G',  5, AF11) LCD_G4 */
  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_5);

  /* ('G',  0, AF14) LCD_G5 */
  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0);

  /* ('G',  7, AF14) LCD_G6 */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);

  /* ('A', 15, AF11) LCD_G7 */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

  /* ('B',  3, AF14) LCD_B2 */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_10);

  /* ('F',  2, AF14) LCD_B3 */
  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_2);

  /* ('B',  4, AF14) LCD_B4 */
  HAL_GPIO_DeInit(GPIOH, GPIO_PIN_4);

  /* ('E',  0, AF14) LCD_B5 */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0);

  /* ('B',  6, AF7)  LCD_B6 */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

  /* ('F',  1, AF13) LCD_B7 */
  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_1);

  /* Disable IRQ */
  IRQ_Disable(LTDC_IRQn);

  /* Disable LTDC clock */
  __HAL_RCC_LTDC_CLK_DISABLE();

}

/**
  * @brief  Clock Config.
  * @param  hltdc: LTDC handle
  * @param  Params
  * @note   This API is called by BSP_LCD_Init()
  *         Being __weak it can be overwritten by the application
  * @retval None
  */
__weak void BSP_LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hltdc);
  UNUSED(Params);


}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
