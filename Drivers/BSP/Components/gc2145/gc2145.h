/**
  ******************************************************************************
  * @file    gc2145.h
  * @author  GPM Application Team
  * @brief   This file contains all the functions prototypes for the gc2145.c
  *          driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GC2145_H
#define GC2145_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc2145_reg.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup gc2145
  * @{
  */

/** @defgroup GC2145_Exported_Types Exported Types
  * @{
  */
/* Exported types ------------------------------------------------------------*/

typedef int32_t (*GC2145_Init_Func)(void);
typedef int32_t (*GC2145_DeInit_Func)(void);
typedef int32_t (*GC2145_GetTick_Func)(void);
typedef int32_t (*GC2145_Delay_Func)(uint32_t);
typedef int32_t (*GC2145_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*GC2145_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  GC2145_Init_Func          Init;
  GC2145_DeInit_Func        DeInit;
  uint16_t                  Address;
  GC2145_WriteReg_Func      WriteReg;
  GC2145_ReadReg_Func       ReadReg;
  GC2145_GetTick_Func       GetTick;
} GC2145_IO_t;

typedef struct
{
  GC2145_IO_t         IO;
  gc2145_ctx_t        Ctx;
  uint8_t             IsInitialized;
} GC2145_Object_t;

typedef struct
{
  uint32_t Config_Resolution;
  uint32_t Config_LightMode;
  uint32_t Config_SpecialEffect;
  uint32_t Config_Brightness;
  uint32_t Config_Saturation;
  uint32_t Config_Contrast;
  uint32_t Config_HueDegree;
  uint32_t Config_MirrorFlip;
  uint32_t Config_Zoom;
  uint32_t Config_NightMode;
} GC2145_Capabilities_t;

typedef struct
{
  int32_t (*Init)(GC2145_Object_t *, uint32_t, uint32_t);
  int32_t (*DeInit)(GC2145_Object_t *);
  int32_t (*ReadID)(GC2145_Object_t *, uint32_t *);
  int32_t (*GetCapabilities)(GC2145_Object_t *, GC2145_Capabilities_t *);
  int32_t (*SetLightMode)(GC2145_Object_t *, uint32_t);
  int32_t (*SetColorEffect)(GC2145_Object_t *, uint32_t);
  int32_t (*SetBrightness)(GC2145_Object_t *, int32_t);
  int32_t (*SetSaturation)(GC2145_Object_t *, int32_t);
  int32_t (*SetContrast)(GC2145_Object_t *, int32_t);
  int32_t (*SetHueDegree)(GC2145_Object_t *, int32_t);
  int32_t (*MirrorFlipConfig)(GC2145_Object_t *, uint32_t);
  int32_t (*ZoomConfig)(GC2145_Object_t *, uint32_t);
  int32_t (*SetResolution)(GC2145_Object_t *, uint32_t);
  int32_t (*GetResolution)(GC2145_Object_t *, uint32_t *);
  int32_t (*SetPixelFormat)(GC2145_Object_t *, uint32_t);
  int32_t (*GetPixelFormat)(GC2145_Object_t *, uint32_t *);
  int32_t (*NightModeConfig)(GC2145_Object_t *, uint32_t);
} GC2145_CAMERA_Drv_t;
/**
  * @}
  */

/** @defgroup GC2145_Exported_Constants
  * @{
  */
#define GC2145_OK                      (0)
#define GC2145_ERROR                   (-1)
/**
  * @brief  GC2145 Features Parameters
  */
/* Camera resolutions */
#define GC2145_R640x480                 3U     /* VGA Resolution              */
#define GC2145_R1600x1200               6U     /* 1600x1200 Resolution        */

/* Camera Pixel Format */
#define GC2145_PF_RGB565                0U     /* Pixel Format RGB565         */
#define GC2145_PF_YUV422                2U     /* Pixel Format YUV422         */

/* Mirror/Flip */
#define GC2145_MIRROR_FLIP_NONE         0x00U   /* Set camera normal mode     */
#define GC2145_FLIP                     0x01U   /* Set camera flip config     */
#define GC2145_MIRROR                   0x02U   /* Set camera mirror config   */
#define GC2145_MIRROR_FLIP              0x03U   /* Set camera mirror and flip */

/* Special Effect */
#define GC2145_SPECIAL_EFFECT_GRAY      0x01    /* Gray        */
#define GC2145_SPECIAL_EFFECT_HIGH      0x02    /* 7f High     */
#define GC2145_SPECIAL_EFFECT_DARK      0x03    /* Dark        */
#define GC2145_SPECIAL_EFFECT_SP_RED    0x04    /* Special Red */
#define GC2145_SPECIAL_EFFECT_GREEN     0x05    /* Green       */
#define GC2145_SPECIAL_EFFECT_BLUE      0x06    /* Blue        */
#define GC2145_SPECIAL_EFFECT_YELLOW    0x07    /* Yellow      */
#define GC2145_SPECIAL_EFFECT_ANCIENTS  0x08    /* ancients    */
#define GC2145_SPECIAL_EFFECT_SOLARIZE  0x09    /* solarize 1  */



/**
  * @}
  */

/** @defgroup GC2145_Exported_Variables GC2145 Exported Variables
  * @{
  */
/* Exported variables --------------------------------------------------------*/
/* CAMERA driver structure */
extern GC2145_CAMERA_Drv_t   GC2145_CAMERA_Driver;
/**
  * @}
  */

/** @defgroup GC2145_Exported_Functions GC2145 Exported Functions
  * @{
  */
/* Exported functions ------------------------------------------------------- */
int32_t GC2145_RegisterBusIO(GC2145_Object_t *pObj, GC2145_IO_t *pIO);
int32_t GC2145_Init(GC2145_Object_t *pObj, uint32_t Resolution, uint32_t PixelFormat);
int32_t GC2145_DeInit(GC2145_Object_t *pObj);
int32_t GC2145_ReadID(GC2145_Object_t *pObj, uint32_t *Id);
int32_t GC2145_GetCapabilities(GC2145_Object_t *pObj, GC2145_Capabilities_t *Capabilities);
int32_t GC2145_SetMirrorFlipConfig(GC2145_Object_t *pObj, uint32_t Config);
int32_t GC2145_SetSpecialEffect(GC2145_Object_t *pObj, uint32_t Config);
int32_t GC2145_SetLightMode(GC2145_Object_t *pObj, uint32_t LightMode);
int32_t GC2145_SetBrightness(GC2145_Object_t *pObj, int32_t Level);
int32_t GC2145_SetSaturation(GC2145_Object_t *pObj, int32_t Level);
int32_t GC2145_SetContrast(GC2145_Object_t *pObj, int32_t Level);
int32_t GC2145_SetHueDegree(GC2145_Object_t *pObj, int32_t Degree);
int32_t GC2145_ZoomConfig(GC2145_Object_t *pObj, uint32_t Zoom);
int32_t GC2145_SetResolution(GC2145_Object_t *pObj, uint32_t Resolution);
int32_t GC2145_GetResolution(GC2145_Object_t *pObj, uint32_t *Resolution);
int32_t GC2145_SetPixelFormat(GC2145_Object_t *pObj, uint32_t PixelFormat);
int32_t GC2145_GetPixelFormat(GC2145_Object_t *pObj, uint32_t *PixelFormat);
int32_t GC2145_NightModeConfig(GC2145_Object_t *pObj, uint32_t Cmd);

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

#endif /* GC2145_H */


