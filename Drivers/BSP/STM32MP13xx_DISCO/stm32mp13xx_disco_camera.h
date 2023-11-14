/**
  ******************************************************************************
  * @file    stm32mp13xx_disco_camera.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32mp13xx_disco_camera.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32MP13XX_DISCO_CAMERA_H
#define STM32MP13XX_DISCO_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_disco_errno.h"
#include "stm32mp13xx_disco_conf.h"

#ifndef USE_CAMERA_SENSOR_OV5640
#ifndef USE_CAMERA_SENSOR_S5K5CAG
#ifndef USE_CAMERA_SENSOR_GC2145
#error No CAMERA SENSOR defined. Please define either USE_CAMERA_SENSOR_OV5640, USE_CAMERA_SENSOR_S5K5CAG \
or USE_CAMERA_SENSOR_GC2145
#endif /* USE_CAMERA_SENSOR_GC2145 */
#endif /* USE_CAMERA_SENSOR_S5K5CAG */
#endif /* USE_CAMERA_SENSOR_OV5640 */

#ifdef USE_CAMERA_SENSOR_OV5640
#include "../Components/ov5640/ov5640.h"
#endif /* USE_CAMERA_SENSOR_OV5640 */
#ifdef USE_CAMERA_SENSOR_S5K5CAG
#include "../Components/s5k5cag/s5k5cag.h"
#endif /* USE_CAMERA_SENSOR_S5K5CAG */

#ifdef USE_CAMERA_SENSOR_GC2145
#include "../Components/gc2145/gc2145.h"
#endif /* USE_CAMERA_SENSOR_GC2145 */
#include "camera.h"

#ifdef USE_STMIPID02
#include "../Components/stmipid02/stmipid02.h"
#endif /* USE_STMIPID02 */

/** @addtogroup BSP
  * @{
  */





/** @defgroup CAMERA_Exported_Types Exported Types
  * @{
  */
typedef struct
{
  uint32_t Resolution;
  uint32_t PixelFormat;
  uint32_t LightMode;
  uint32_t ColorEffect;
  int32_t  Brightness;
  int32_t  Saturation;
  int32_t  Contrast;
  int32_t  HueDegree;
  uint32_t MirrorFlip;
  uint32_t Zoom;
  uint32_t NightMode;
  uint32_t IsMspCallbacksValid;
} CAMERA_Ctx_t;

typedef struct
{
  uint32_t Resolution;
  uint32_t LightMode;
  uint32_t ColorEffect;
  uint32_t Brightness;
  uint32_t Saturation;
  uint32_t Contrast;
  uint32_t HueDegree;
  uint32_t MirrorFlip;
  uint32_t Zoom;
  uint32_t NightMode;
} CAMERA_Capabilities_t;

/**
  * @}
  */

/** @defgroup CAMERA_Exported_Constants Exported Constants
  * @{
  */
/* Camera instance number */
#define CAMERA_INSTANCES_NBR           1U

#define CAMERA_MODE_CONTINUOUS         DCMI_MODE_CONTINUOUS
#define CAMERA_MODE_SNAPSHOT           DCMI_MODE_SNAPSHOT

/* Camera resolutions */
#define CAMERA_R160x120                 0U     /* QQVGA Resolution            */
#define CAMERA_R320x240                 1U     /* QVGA Resolution             */
#define CAMERA_R480x272                 2U     /* 480x272 Resolution          */
#define CAMERA_R640x480                 3U     /* VGA Resolution              */
#define CAMERA_R800x480                 4U     /* WVGA Resolution             */
#define CAMERA_R1280x720                5U     /* 720p Resolution             */
#define CAMERA_R1600x1200               6U     /* 1600x1200 Resolution        */
#define CAMERA_R2592x1944               7U     /* QSXGA Resolution            */

/* Camera Pixel Format */
#define CAMERA_PF_RGB565                0U     /* Pixel Format RGB565         */
#define CAMERA_PF_RGB888                1U     /* Pixel Format RGB888         */
#define CAMERA_PF_YUV422                2U     /* Pixel Format YUV422         */
#define CAMERA_PF_BGGR8                 3U     /* Pixel Format RAW_BGGR8      */
#define CAMERA_PF_GBRG8                 4U     /* Pixel Format RAW_GBRG8      */
#define CAMERA_PF_GRBG8                 5U     /* Pixel Format RAW_GRBG8      */
#define CAMERA_PF_RGGB8                 6U     /* Pixel Format RAW_RGGB8      */
#define CAMERA_PF_Y8                    7U     /* Pixel Format Y8             */
#define CAMERA_PF_JPEG                  8U     /* Compressed format JPEG      */

/* Brightness */
#define CAMERA_BRIGHTNESS_MIN          -4
#define CAMERA_BRIGHTNESS_MAX           4

/* Saturation */
#define CAMERA_SATURATION_MIN          -4
#define CAMERA_SATURATION_MAX           4

/* Contrast */
#define CAMERA_CONTRAST_MIN            -4
#define CAMERA_CONTRAST_MAX             4

/* Hue Control */
#define CAMERA_HUEDEGREE_MIN           -6
#define CAMERA_HUEDEGREE_MAX            5

/* Mirror/Flip */
#define CAMERA_MIRRORFLIP_NONE          0x00U   /* Set camera normal mode     */
#define CAMERA_MIRRORFLIP_FLIP          0x01U   /* Set camera flip config     */
#define CAMERA_MIRRORFLIP_MIRROR        0x02U   /* Set camera mirror config   */
#define CAMERA_MIRRORFLIP_MIRRORFLIP    0x03U   /* Set camera mirror flip config   */

/* Zoom */
#define CAMERA_ZOOM_x8                  0x00U   /* Set zoom to x8             */
#define CAMERA_ZOOM_x4                  0x11U   /* Set zoom to x4             */
#define CAMERA_ZOOM_x2                  0x22U   /* Set zoom to x2             */
#define CAMERA_ZOOM_x1                  0x44U   /* Set zoom to x1             */

/* Color Effect */
#if defined(USE_CAMERA_SENSOR_GC2145)
#define CAMERA_COLOR_EFFECT_NONE       0X00U     /*  No effect  */
#define CAMERA_COLOR_EFFECT_GRAY       0x01U     /*  gray       */
#define CAMERA_COLOR_EFFECT_HIGH       0x02U     /*  7f high    */
#define CAMERA_COLOR_EFFECT_DARK       0x03U     /*  dark       */
#define CAMERA_COLOR_EFFECT_SP_RED     0x04U     /*  sp red     */
#define CAMERA_COLOR_EFFECT_GREEN      0x05U     /*  green      */
#define CAMERA_COLOR_EFFECT_BLUE       0x06U     /*  blue       */
#define CAMERA_COLOR_EFFECT_YELLOW     0x07U     /*  yellow     */
#define CAMERA_COLOR_EFFECT_ANCIENTS   0x08U     /*  ancients   */
#define CAMERA_COLOR_EFFECT_SOLARIZE   0x09U     /*  solarize 1 */
#else
#define CAMERA_COLOR_EFFECT_NONE        0x00U   /* No effect                  */
#define CAMERA_COLOR_EFFECT_BLUE        0x01U   /* Blue effect                */
#define CAMERA_COLOR_EFFECT_RED         0x02U   /* Red effect                 */
#define CAMERA_COLOR_EFFECT_GREEN       0x04U   /* Green effect               */
#define CAMERA_COLOR_EFFECT_BW          0x08U   /* Black and White effect     */
#define CAMERA_COLOR_EFFECT_SEPIA       0x10U   /* Sepia effect               */
#define CAMERA_COLOR_EFFECT_NEGATIVE    0x20U   /* Negative effect            */
#endif /* USE_CAMERA_SENSOR_GC2145 */

/* Light Mode */
#define CAMERA_LIGHT_AUTO               0x00U   /* Light Mode Auto            */
#define CAMERA_LIGHT_SUNNY              0x01U   /* Light Mode Sunny           */
#define CAMERA_LIGHT_OFFICE             0x02U   /* Light Mode Office          */
#define CAMERA_LIGHT_HOME               0x04U   /* Light Mode Home            */
#define CAMERA_LIGHT_CLOUDY             0x08U   /* Light Mode Claudy          */

/* Night Mode */
#define CAMERA_NIGHT_MODE_SET           0x00U   /* Disable night mode         */
#define CAMERA_NIGHT_MODE_RESET         0x01U   /* Enable night mode          */

/* Colorbar Mode */
#define CAMERA_COLORBAR_NONE            0x00U   /* Disable colorbar mode      */
#define CAMERA_COLORBAR_DEFAULT         0x01U   /* 8 bars W/Y/C/G/M/R/B/Bl    */
#define CAMERA_COLORBAR_GRADUALV        0x02U   /* Gradual vertical colorbar  */


#define CAMERA_IRQHandler               DCMIPP_IRQHandler
#define CAMERA_DMA_IRQHandler           DMA2_Stream3_IRQHandler

#define CAMERA_STBYN_PIN                GPIO_PIN_9
#define CAMERA_STBYN_GPIO_PORT          GPIOB
#define CAMERA_STBYN_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define CAMERA_RST_PIN                  GPIO_PIN_8
#define CAMERA_RST_GPIO_PORT            GPIOB
#define CAMERA_RST_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

#if defined (MCP_IOEXPANDER)
#define XSDN_PIN                       IO_PIN_3
#define RSTI_PIN                       IO_PIN_4
#endif /* (MCP_IOEXPANDER) */

#ifdef USE_CAMERA_SENSOR_OV5640
#define CAMERA_OV5640_ADDRESS          0x78U
#endif /* USE_CAMERA_SENSOR_OV5640 */

#ifdef USE_CAMERA_SENSOR_S5K5CAG
#define CAMERA_S5K5CAG_ADDRESS         0x5AU
#endif /* USE_CAMERA_SENSOR_S5K5CAG */

#ifdef USE_CAMERA_SENSOR_GC2145
#define CAMERA_GC2145_ADDRESS          0x78U
#endif /* USE_CAMERA_SENSOR_GC2145 */

#ifdef USE_STMIPID02
#define STMIPID02_ADDRESS              0X28U
#define STMIPID02_XSDN_PIN            IO_PIN_2
#define STMIPID02_CLK_PIN             GPIO_PIN_7
#define STMIPID02_CLK_PORT            GPIOD
#endif /* USE_STMIPID02 */

/**
  * @}
  */

/** @addtogroup CAMERA_Exported_Variables
  * @{
  */
extern CAMERA_Ctx_t        CameraCtx[];
/**
  * @}
  */

/** @defgroup Exported Functions
  * @{
  */
int32_t GetSize(uint32_t Resolution, uint32_t PixelFormat);
int32_t BSP_CAMERA_Init(uint32_t Instance, uint32_t Resolution, uint32_t PixelFormat);
int32_t BSP_CAMERA_DeInit(uint32_t Instance);
int32_t BSP_CAMERA_Start(uint32_t Instance, uint8_t *buff, uint32_t Mode);
int32_t BSP_CAMERA_Stop(uint32_t Instance);
int32_t BSP_CAMERA_Suspend(uint32_t Instance);
int32_t BSP_CAMERA_Resume(uint32_t Instance);
int32_t BSP_CAMERA_GetCapabilities(uint32_t Instance, CAMERA_Capabilities_t *Capabilities);

int32_t BSP_CAMERA_SetResolution(uint32_t Instance, uint32_t Resolution);
int32_t BSP_CAMERA_GetResolution(uint32_t Instance, uint32_t *Resolution);

int32_t BSP_CAMERA_SetPixelFormat(uint32_t Instance, uint32_t PixelFormat);
int32_t BSP_CAMERA_GetPixelFormat(uint32_t Instance, uint32_t *PixelFormat);

int32_t BSP_CAMERA_SetLightMode(uint32_t Instance, uint32_t LightMode);
int32_t BSP_CAMERA_GetLightMode(uint32_t Instance, uint32_t *LightMode);

int32_t BSP_CAMERA_SetColorEffect(uint32_t Instance, uint32_t ColorEffect);
int32_t BSP_CAMERA_GetColorEffect(uint32_t Instance, uint32_t *ColorEffect);

int32_t BSP_CAMERA_SetBrightness(uint32_t Instance, int32_t Brightness);
int32_t BSP_CAMERA_GetBrightness(uint32_t Instance, int32_t *Brightness);

int32_t BSP_CAMERA_SetSaturation(uint32_t Instance, int32_t Saturation);
int32_t BSP_CAMERA_GetSaturation(uint32_t Instance, int32_t *Saturation);

int32_t BSP_CAMERA_SetContrast(uint32_t Instance, int32_t Contrast);
int32_t BSP_CAMERA_GetContrast(uint32_t Instance, int32_t *Contrast);

int32_t BSP_CAMERA_SetHueDegree(uint32_t Instance, int32_t HueDegree);
int32_t BSP_CAMERA_GetHueDegree(uint32_t Instance, int32_t *HueDegree);

int32_t BSP_CAMERA_SetMirrorFlip(uint32_t Instance, uint32_t MirrorFlip);
int32_t BSP_CAMERA_GetMirrorFlip(uint32_t Instance, uint32_t *MirrorFlip);

int32_t BSP_CAMERA_SetZoom(uint32_t Instance, uint32_t Zoom);
int32_t BSP_CAMERA_GetZoom(uint32_t Instance, uint32_t *Zoom);

int32_t BSP_CAMERA_EnableNightMode(uint32_t Instance);
int32_t BSP_CAMERA_DisableNightMode(uint32_t Instance);

int32_t BSP_CAMERA_SetColorbarMode(uint32_t Instance, uint32_t Mode);
int32_t BSP_CAMERA_SetPolarities(uint32_t Instance, uint32_t Pclk, uint32_t Href, uint32_t Vsync);
int32_t BSP_CAMERA_SetSynchroCodes(uint32_t Instance, uint32_t FSC, uint32_t LSC, uint32_t LEC, uint32_t FEC);

int32_t BSP_CAMERA_HwReset(uint32_t Instance);
void    BSP_CAMERA_LineEventCallback(uint32_t Instance);
void    BSP_CAMERA_FrameEventCallback(uint32_t Instance);
void    BSP_CAMERA_VsyncEventCallback(uint32_t Instance);
void    BSP_CAMERA_ErrorCallback(uint32_t Instance);

void    BSP_CAMERA_IRQHandler(void);
void    BSP_CAMERA_DMA_IRQHandler(void);

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

#ifdef __cplusplus
}
#endif

#endif /* STM32MP13XX_DISCO_CAMERA_H */
