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
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "fonts.h"

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
/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_LCD_Private_Defines STM32MP13XX_DISCO LCD Private Defines
  * @{
  */
#define POLY_X(Z)              ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)              ((int32_t)((Points + Z)->Y))
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
  * @retval LCD state
  */
uint8_t BSP_LCD_Init(void)
{

  /* DeInit */
  if(HAL_LTDC_DeInit(&hLtdcHandler) != HAL_OK)
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
  hLtdcHandler.Init.TotalHeigh = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP + RK043FN48H_VFP - (uint16_t)1);
  hLtdcHandler.Init.TotalWidth = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + RK043FN48H_HBP + RK043FN48H_HFP - (uint16_t)1);

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

  if(HAL_LTDC_GetState(&hLtdcHandler) == HAL_LTDC_STATE_RESET)
  {
    /* Initialize the LCD Msp: this __weak function can be rewritten by the application */
    BSP_LCD_MspInit(&hLtdcHandler, NULL);
  }
  if(HAL_LTDC_Init(&hLtdcHandler) != HAL_OK)
  {
      return LCD_ERROR;
  }

  /* Initialize the font */
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

  return LCD_OK;
}

/**
  * @brief  DeInitializes the LCD.
  * @retval LCD state
  */
uint8_t BSP_LCD_DeInit(void)
{
  /* Initialize the hLtdcHandler Instance parameter */
  hLtdcHandler.Instance = LTDC;

 /* Disable LTDC block */
  __HAL_LTDC_DISABLE(&hLtdcHandler);

  /* DeInit the LTDC */
  if(HAL_LTDC_DeInit(&hLtdcHandler) != HAL_OK)
  {
    return LCD_ERROR;
  }

  /* DeInit the LTDC MSP : this __weak function can be rewritten by the application */
  BSP_LCD_MspDeInit(&hLtdcHandler, NULL);

  return LCD_OK;
}

/**
  * @brief  Gets the LCD X size.
  * @retval Used LCD X size
  */
uint32_t BSP_LCD_GetXSize(void)
{
  return hLtdcHandler.LayerCfg[ActiveLayer].ImageWidth;
}

/**
  * @brief  Gets the LCD Y size.
  * @retval Used LCD Y size
  */
uint32_t BSP_LCD_GetYSize(void)
{
  return hLtdcHandler.LayerCfg[ActiveLayer].ImageHeight;
}

/**
  * @brief  Set the LCD X size.
  * @param  imageWidthPixels : image width in pixels unit
  * @retval None
  */
void BSP_LCD_SetXSize(uint32_t imageWidthPixels)
{
  hLtdcHandler.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

/**
  * @brief  Set the LCD Y size.
  * @param  imageHeightPixels : image height in lines unit
  * @retval None
  */
void BSP_LCD_SetYSize(uint32_t imageHeightPixels)
{
  hLtdcHandler.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}

/**
  * @brief  Initializes the LCD layer in ARGB8888 format (32 bits per pixel).
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
uint8_t BSP_LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{
  LCD_LayerCfgTypeDef  layer_cfg;

  /* Layer Init */
  layer_cfg.WindowX0 = 0;
  layer_cfg.WindowX1 = BSP_LCD_GetXSize();
  layer_cfg.WindowY0 = 0;
  layer_cfg.WindowY1 = BSP_LCD_GetYSize();

  layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_BGR565;
  layer_cfg.FBStartAdress = FB_Address;

  layer_cfg.Alpha = 255;
  layer_cfg.Alpha0 = 0;
  layer_cfg.Backcolor.Blue = 0;
  layer_cfg.Backcolor.Green = 0;
  layer_cfg.Backcolor.Red = 0;

  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;

  layer_cfg.ImageWidth = BSP_LCD_GetXSize();
  layer_cfg.ImageHeight = BSP_LCD_GetYSize();

  if(HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  if(HAL_LTDC_ProgramLineEvent(&hLtdcHandler, BSP_LCD_GetYSize()/(uint32_t)2) != HAL_OK)
  {
    return LCD_ERROR;
  }

  DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
  DrawProp[LayerIndex].pFont     = &Font24;
  DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;

  return LCD_OK;
}

/**
  * @brief  Initializes the LCD layer in RGB565 format (16 bits per pixel).
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
uint8_t BSP_LCD_LayerRgb565Init(uint16_t LayerIndex, uint32_t FB_Address)
{
  LCD_LayerCfgTypeDef  layer_cfg;

  /* Layer Init */
  layer_cfg.WindowX0 = 0;
  layer_cfg.WindowX1 = BSP_LCD_GetXSize();
  layer_cfg.WindowY0 = 0;
  layer_cfg.WindowY1 = BSP_LCD_GetYSize();
  layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  layer_cfg.FBStartAdress = FB_Address;
  layer_cfg.Alpha = 255;
  layer_cfg.Alpha0 = 0;
  layer_cfg.Backcolor.Blue = 0;
  layer_cfg.Backcolor.Green = 0;
  layer_cfg.Backcolor.Red = 0;
  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  layer_cfg.ImageWidth = BSP_LCD_GetXSize();
  layer_cfg.ImageHeight = BSP_LCD_GetYSize();

  if(HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }

  DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
  DrawProp[LayerIndex].pFont     = &Font24;
  DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;
  
  return  LCD_OK;
}

/**
  * @brief  Selects the LCD Layer.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
void BSP_LCD_SelectLayer(uint32_t LayerIndex)
{
  ActiveLayer = LayerIndex;
}

/**
  * @brief  Sets an LCD Layer visible
  * @param  LayerIndex: Visible Layer
  * @param  State: New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE
  * @retval None
  */
void BSP_LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State)
{
  if(State == ENABLE)
  {
    __HAL_LTDC_LAYER_ENABLE(&hLtdcHandler, LayerIndex);
  }
  else
  {
    __HAL_LTDC_LAYER_DISABLE(&hLtdcHandler, LayerIndex);
  }
  __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&hLtdcHandler);
}

/**
  * @brief  Sets an LCD Layer visible without reloading.
  * @param  LayerIndex: Visible Layer
  * @param  State: New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE
  * @retval None
  */
void BSP_LCD_SetLayerVisible_NoReload(uint32_t LayerIndex, FunctionalState State)
{
  if(State == ENABLE)
  {
    __HAL_LTDC_LAYER_ENABLE(&hLtdcHandler, LayerIndex);
  }
  else
  {
    __HAL_LTDC_LAYER_DISABLE(&hLtdcHandler, LayerIndex);
  }
  /* Do not Sets the Reload  */
}

/**
  * @brief  Configures the transparency.
  * @param  LayerIndex: Layer foreground or background.
  * @param  Transparency: Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
  * @retval None
  */
uint8_t BSP_LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{
  if(HAL_LTDC_SetAlpha(&hLtdcHandler, Transparency, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
   return LCD_OK;
}

/**
  * @brief  Configures the transparency without reloading.
  * @param  LayerIndex: Layer foreground or background.
  * @param  Transparency: Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
  * @retval None
  */
uint8_t BSP_LCD_SetTransparency_NoReload(uint32_t LayerIndex, uint8_t Transparency)
{
  if(HAL_LTDC_SetAlpha_NoReload(&hLtdcHandler, Transparency, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Sets an LCD layer frame buffer address.
  * @param  LayerIndex: Layer foreground or background
  * @param  Address: New LCD frame buffer value
  * @retval None
  */
uint8_t BSP_LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address)
{
  if(HAL_LTDC_SetAddress(&hLtdcHandler, Address, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Sets an LCD layer frame buffer address without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @param  Address: New LCD frame buffer value
  * @retval None
  */
uint8_t BSP_LCD_SetLayerAddress_NoReload(uint32_t LayerIndex, uint32_t Address)
{
  if(HAL_LTDC_SetAddress_NoReload(&hLtdcHandler, Address, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Sets display window.
  * @param  LayerIndex: Layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height
  * @retval None
  */
uint8_t BSP_LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Reconfigure the layer size */
  if(HAL_LTDC_SetWindowSize(&hLtdcHandler, Width, Height, LayerIndex) != HAL_OK)
  {
     return LCD_ERROR; 
  }
  
  /* Reconfigure the layer position */
  if(HAL_LTDC_SetWindowPosition(&hLtdcHandler, Xpos, Ypos, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Sets display window without reloading.
  * @param  LayerIndex: Layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height
  * @retval None
  */
uint8_t BSP_LCD_SetLayerWindow_NoReload(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Reconfigure the layer size */
  if(HAL_LTDC_SetWindowSize_NoReload(&hLtdcHandler, Width, Height, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }

  /* Reconfigure the layer position */
  if(HAL_LTDC_SetWindowPosition_NoReload(&hLtdcHandler, Xpos, Ypos, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Configures and sets the color keying.
  * @param  LayerIndex: Layer foreground or background
  * @param  RGBValue: Color reference
  * @retval None
  */
uint8_t BSP_LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue)
{
  /* Configure and Enable the color Keying for LCD Layer */
  if(HAL_LTDC_ConfigColorKeying(&hLtdcHandler, RGBValue, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  if(HAL_LTDC_EnableColorKeying(&hLtdcHandler, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Configures and sets the color keying without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @param  RGBValue: Color reference
  * @retval None
  */
uint8_t BSP_LCD_SetColorKeying_NoReload(uint32_t LayerIndex, uint32_t RGBValue)
{
  /* Configure and Enable the color Keying for LCD Layer */
  if(HAL_LTDC_ConfigColorKeying_NoReload(&hLtdcHandler, RGBValue, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  if(HAL_LTDC_EnableColorKeying_NoReload(&hLtdcHandler, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Disables the color keying.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
uint8_t BSP_LCD_ResetColorKeying(uint32_t LayerIndex)
{
  /* Disable the color Keying for LCD Layer */
  if(HAL_LTDC_DisableColorKeying(&hLtdcHandler, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Disables the color keying without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
uint8_t BSP_LCD_ResetColorKeying_NoReload(uint32_t LayerIndex)
{
  /* Disable the color Keying for LCD Layer */
  if(HAL_LTDC_DisableColorKeying_NoReload(&hLtdcHandler, LayerIndex) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Disables the color keying without reloading.
  * @param  ReloadType: can be one of the following values
  *         - LCD_RELOAD_IMMEDIATE
  *         - LCD_RELOAD_VERTICAL_BLANKING
  * @retval None
  */
uint8_t BSP_LCD_Reload(uint32_t ReloadType)
{
  if(HAL_LTDC_Reload (&hLtdcHandler, ReloadType) != HAL_OK)
  {
    return LCD_ERROR;
  }
  
  return LCD_OK;
}

/**
  * @brief  Sets the LCD text color.
  * @param  Color: Text color code ARGB(8-8-8-8)
  * @retval None
  */
void BSP_LCD_SetTextColor(uint32_t Color)
{
  DrawProp[ActiveLayer].TextColor = Color;
}

/**
  * @brief  Gets the LCD text color.
  * @retval Used text color.
  */
uint32_t BSP_LCD_GetTextColor(void)
{
  return DrawProp[ActiveLayer].TextColor;
}

/**
  * @brief  Sets the LCD background color.
  * @param  Color: Layer background color code ARGB(8-8-8-8)
  * @retval None
  */
void BSP_LCD_SetBackColor(uint32_t Color)
{
  DrawProp[ActiveLayer].BackColor = Color;
}

/**
  * @brief  Gets the LCD background color.
  * @retval Used background colour
  */
uint32_t BSP_LCD_GetBackColor(void)
{
  return DrawProp[ActiveLayer].BackColor;
}

/**
  * @brief  Sets the LCD text font.
  * @param  fonts: Layer font to be used
  * @retval None
  */
void BSP_LCD_SetFont(sFONT *fonts)
{
  DrawProp[ActiveLayer].pFont = fonts;
}

/**
  * @brief  Gets the LCD text font.
  * @retval Used layer font
  */
sFONT *BSP_LCD_GetFont(void)
{
  return DrawProp[ActiveLayer].pFont;
}

/**
  * @brief  Enables the display.
  * @retval None
  */
void BSP_LCD_DisplayOn(void)
{
  GPIO_InitTypeDef GPIO_Init_Structure;

  LCD_RST_GPIO_CLK_ENABLE();

  /* Configure the GPIO RST pin */
  GPIO_Init_Structure.Pin   = LCD_RST_PIN;
  GPIO_Init_Structure.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_Init_Structure.Pull  = LCD_RST_PULL;
  GPIO_Init_Structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD_RST_GPIO_PORT , &GPIO_Init_Structure);

  /* Activate XRES active low */
  HAL_GPIO_WritePin(LCD_RST_GPIO_PORT , LCD_RST_PIN, GPIO_PIN_RESET);
  HAL_Delay(20);/* wait 20 ms */
  HAL_GPIO_WritePin(LCD_RST_GPIO_PORT , LCD_RST_PIN, GPIO_PIN_SET);/* Deactivate XRES */
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

/**
  * @brief  Disables the display.
  * @retval None
  */
void BSP_LCD_DisplayOff(void)
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