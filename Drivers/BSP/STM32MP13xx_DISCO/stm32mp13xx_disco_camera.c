/**
  ******************************************************************************
  * @file    stm32mp13xx_disco_camera.c
  * @author  MCD Application Team
  * @brief   This file includes the driver for Camera modules .
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

/* File Info: ------------------------------------------------------------------
                                   User NOTES
1. How to use this driver:
--------------------------
   - This driver is used to drive the camera.
   - The S5K5CAG or OV5640 component driver MUST be included with this driver.

2. Driver description:
---------------------
     o Initialize the camera instance using the BSP_CAMERA_Init() function with the required
       Resolution and Pixel format where:
       - Instance: Is the physical camera interface and is always 0 on this board.
       - Resolution: The camera resolution
       - PixelFormat: The camera Pixel format

     o DeInitialize the camera instance using the BSP_CAMERA_Init() . This
       function will firstly stop the camera to insure the data transfer complete.

     o Get the camera instance capabilities using the BSP_CAMERA_GetCapabilities().
       This function must be called after the BSP_CAMERA_Init() to get the right
       sensor capabilities

     o Start the camera using the CAMERA_Start() function by specifying the capture Mode:
       - CAMERA_MODE_CONTINUOUS: For continuous capture
       - CAMERA_MODE_SNAPSHOT  : For on shot capture

     o Suspend, resume or stop the camera capture using the following functions:
      - BSP_CAMERA_Suspend()
      - BSP_CAMERA_Resume()
      - BSP_CAMERA_Stop()

     o Call BSP_CAMERA_SetResolution()/BSP_CAMERA_GetResolution() to set/get the camera resolution
       Resolution: - CAMERA_R160x120
                   - CAMERA_R320x240
                   - CAMERA_R480x272
                   - CAMERA_R640x480
                   - CAMERA_R800x480

     o Call BSP_CAMERA_SetPixelFormat()/BSP_CAMERA_GetPixelFormat() to set/get the camera pixel format
       PixelFormat: - CAMERA_PF_RGB565
                    - CAMERA_PF_RGB888
                    - CAMERA_PF_YUV422

     o Call BSP_CAMERA_SetLightMode()/BSP_CAMERA_GetLightMode() to set/get the camera light mode
       LightMode: - CAMERA_LIGHT_AUTO
                  - CAMERA_LIGHT_SUNNY
                  - CAMERA_LIGHT_OFFICE
                  - CAMERA_LIGHT_HOME
                  - CAMERA_LIGHT_CLOUDY

     o Call BSP_CAMERA_SetColorEffect()/BSP_CAMERA_GetColorEffect() to set/get the camera color effects
       Effect: - CAMERA_COLOR_EFFECT_NONE
               - CAMERA_COLOR_EFFECT_BLUE
               - CAMERA_COLOR_EFFECT_RED
               - CAMERA_COLOR_EFFECT_GREEN
               - CAMERA_COLOR_EFFECT_BW
               - CAMERA_COLOR_EFFECT_SEPIA
               - CAMERA_COLOR_EFFECT_NEGATIVE

     o Call BSP_CAMERA_SetBrightness()/BSP_CAMERA_GetBrightness() to set/get the camera Brightness
       Brightness is value between -4(Level 4 negative) and 4(Level 4 positive).

     o Call BSP_CAMERA_SetSaturation()/BSP_CAMERA_GetSaturation() to set/get the camera Saturation
       Saturation is value between -4(Level 4 negative) and 4(Level 4 positive).

     o Call BSP_CAMERA_SetContrast()/BSP_CAMERA_GetContrast() to set/get the camera Contrast
       Contrast is value between -4(Level 4 negative) and 4(Level 4 positive).

     o Call BSP_CAMERA_SetHueDegree()/BSP_CAMERA_GetHueDegree() to set/get the camera Hue Degree
       HueDegree is value between -4(180 degree negative) and 4(150 degree positive).

     o Call BSP_CAMERA_SetMirrorFlip()/BSP_CAMERA_GetMirrorFlip() to set/get the camera mirror and flip
       MirrorFlip could be any combination of: - CAMERA_MIRRORFLIP_NONE
                                               - CAMERA_MIRRORFLIP_FLIP
                                               - CAMERA_MIRRORFLIP_MIRROR
       Note that This feature is only supported with S5K5CAG sensor.

     o Call BSP_CAMERA_SetZoom()/BSP_CAMERA_GetZoom() to set/get the camera zooming
       Zoom is supported only with S5K5CAG sensor could be any value of:
       - CAMERA_ZOOM_x8 for CAMERA_R160x120 resolution only
       - CAMERA_ZOOM_x4 For all resolutions except CAMERA_R640x480 and CAMERA_R800x480
       - CAMERA_ZOOM_x2 For all resolutions except CAMERA_R800x480
       - CAMERA_ZOOM_x1 For all resolutions

     o Call BSP_CAMERA_EnableNightMode() to enable night mode. This feature is only supported
       with S5K5CAG sensor

     o Call BSP_CAMERA_DisableNightMode() to disable night mode. This feature is only supported
       with S5K5CAG sensor

    o Error, line event, vsync event and frame event are handled through dedicated weak
      callbacks that can be override at application level: BSP_CAMERA_LineEventCallback(),
      BSP_CAMERA_FrameEventCallback(), BSP_CAMERA_VsyncEventCallback(), BSP_CAMERA_ErrorCallback()

  Known Limitations:
  ------------------
   1- CAMERA_PF_RGB888 resolution is not supported with S5K5CAG sensor.
   2- The following feature are only supported through S5K5CAG sensor:
      o LightMode setting
      o Saturation setting
      o HueDegree setting
      o Mirror/Flip setting
      o Zoom setting
      o NightMode enable/disable
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_disco_camera.h"
#include "stm32mp13xx_disco_bus.h"
#include "stm32mp13xx_disco_io.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup _Variables Exported Variables
  * @{
  */
CAMERA_Ctx_t        CameraCtx[CAMERA_INSTANCES_NBR];

CAMERA_Capabilities_t Cap;

/** @defgroup CAMERA_Private_Variables Private Variables
  * @{
  */
static CAMERA_Drv_t *CameraDrv = NULL;
static void         *CompObj = NULL;
static CAMERA_Capabilities_t *CameraCap;

#ifdef USE_STMIPID02
static STMIPID02_Drv_t *DeSerializerDrv = NULL;
static void           *CompObj2 = NULL;
#endif /* USE_STMIPID02 */
/**
  * @}
  */

/** @defgroup CAMERA_Private_FunctionsPrototypes Private Functions Prototypes
  * @{
  */

#ifdef USE_CAMERA_SENSOR_S5K5CAG
static S5K5CAG_Object_t   S5K5CAGObj = { 0 };
static int32_t S5K5CAG_Probe(uint32_t Resolution, uint32_t PixelFormat);
#endif /* USE_CAMERA_SENSOR_S5K5CAG */

#ifdef USE_CAMERA_SENSOR_OV5640
static OV5640_Object_t   OV5640Obj = { 0 };
static int32_t OV5640_Probe(uint32_t Resolution, uint32_t PixelFormat);
#endif /* USE_CAMERA_SENSOR_OV5640 */

#ifdef USE_CAMERA_SENSOR_GC2145
static GC2145_Object_t   GC2145Obj = { 0 };
static int32_t GC2145_Probe(uint32_t Resolution, uint32_t PixelFormat);
#endif /* USE_CAMERA_SENSOR_GC2145 */

#ifdef USE_STMIPID02
static STMIPID02_Object_t   STMIPID02Obj = { 0 };
static int32_t STMIPID02_ClkEnable(void);
static int32_t STMIPID02_Probe(uint8_t PixelFormat);
#endif /* USE_STMIPID02 */

/**
  * @}
  */

/** @addtogroup CAMERA_Exported_Functions

  * @{
  */
/**
  * @brief  Initializes the camera.
  * @param  Instance    Camera instance.
  * @param  Resolution  Camera sensor requested resolution (x, y) : standard resolution
  *         naming QQVGA, QVGA, VGA ...
  * @param  PixelFormat Capture pixel format
  * @retval BSP status
  */
int32_t BSP_CAMERA_Init(uint32_t Instance, uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (BSP_CAMERA_HwReset(0) != BSP_ERROR_NONE)
  {
    return BSP_ERROR_BUS_FAILURE;
  }

  /* Read ID of Camera module via I2C */
#ifdef USE_CAMERA_SENSOR_S5K5CAG
  ret = S5K5CAG_Probe(Resolution, PixelFormat);
#endif /* USE_CAMERA_SENSOR_S5K5CAG */

#ifdef USE_CAMERA_SENSOR_OV5640
  ret = OV5640_Probe(Resolution, PixelFormat);
#endif /* USE_CAMERA_SENSOR_OV5640 */

#ifdef USE_CAMERA_SENSOR_GC2145
  ret = GC2145_Probe(Resolution, PixelFormat);
#endif /* USE_CAMERA_SENSOR_GC2145 */

  if (ret != BSP_ERROR_NONE)
  {
    return BSP_ERROR_UNKNOWN_COMPONENT;
  }

#ifdef USE_STMIPID02

  if (STMIPID02_ClkEnable() != BSP_ERROR_NONE) /* if Camera is OK setup the DeSerializer. */
  {
    return BSP_ERROR_BUS_FAILURE;
  }

  ret = STMIPID02_Probe(PixelFormat);

  if (ret != BSP_ERROR_NONE)
  {
    return BSP_ERROR_UNKNOWN_COMPONENT;
  }

#endif /* USE_STMIPID02 */

  CameraCtx[Instance].Resolution = Resolution;
  CameraCtx[Instance].PixelFormat = PixelFormat;

  /* BSP status */
  return BSP_ERROR_NONE;
}

/**
  * @brief  DeInitializes the camera.
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  /* First stop the camera to insure all data are transferred */
  ret = BSP_CAMERA_Stop(Instance);
  if (ret != BSP_ERROR_NONE)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* Return BSP status */
  return BSP_ERROR_NONE;
}

/**
  * @brief  Starts the camera capture in continuous mode.
  * @param  Instance Camera instance.
  * @param  pBff     pointer to the camera output buffer
  * @param  Mode CAMERA_MODE_CONTINUOUS or CAMERA_MODE_SNAPSHOT
  */
int32_t BSP_CAMERA_Start(uint32_t Instance, uint8_t *pBff, uint32_t Mode)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}

/**
  * @brief  Stop the CAMERA capture
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_Stop(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

#ifdef USE_CAMERA_SENSOR_OV5640
  CameraDrv = (CAMERA_Drv_t *) &OV5640_CAMERA_Driver;
  CompObj = &OV5640Obj;

  ret = CameraDrv->DeInit(CompObj);
  if (ret != OV5640_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  CompObj = NULL;
#endif /* USE_CAMERA_SENSOR_OV5640 */

#ifdef USE_CAMERA_SENSOR_GC2145
  CameraDrv = (CAMERA_Drv_t *) &GC2145_CAMERA_Driver;
  CompObj = &GC2145Obj;

  ret = CameraDrv->DeInit(CompObj);
  if (ret != GC2145_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  CompObj = NULL;
#endif /* USE_CAMERA_SENSOR_GC2145 */

#ifdef USE_STMIPID02
  DeSerializerDrv = (STMIPID02_Drv_t *) &STMIPID02_Driver;
  CompObj2 = &STMIPID02Obj;

  ret = DeSerializerDrv->DeInit(CompObj2);

  if (ret != STMIPID02_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  if (BSP_IO_WritePin(0, STMIPID02_XSDN_PIN, IO_PIN_RESET) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  CompObj2 = NULL;

#endif /* USE_STMIPID02 */

  return BSP_ERROR_NONE;
}

/**
  * @brief Suspend the CAMERA capture
  * @param  Instance Camera instance.
  */
int32_t BSP_CAMERA_Suspend(uint32_t Instance)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}

/**
  * @brief Resume the CAMERA capture
  * @param  Instance Camera instance.
  */
int32_t BSP_CAMERA_Resume(uint32_t Instance)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}

/**
  * @brief  Get the Camera Capabilities.
  * @param  Instance  Camera instance.
  * @param  Capabilities  pointer to camera Capabilities
  * @note   This function should be called after the init. This to get Capabilities
  *         from the right camera sensor(S5K5CAG or OV5640)
  * @retval Component status
  */
int32_t BSP_CAMERA_GetCapabilities(uint32_t Instance, CAMERA_Capabilities_t *Capabilities)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  ret = CameraDrv->GetCapabilities(CompObj, Capabilities);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the camera pixel format.
  * @param  Instance  Camera instance.
  * @param  PixelFormat pixel format to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetPixelFormat(uint32_t Instance, uint32_t PixelFormat)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  ret = CameraDrv->SetPixelFormat(CompObj, PixelFormat);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].PixelFormat = PixelFormat;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the camera pixel format.
  * @param  Instance  Camera instance.
  * @param  PixelFormat pixel format to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetPixelFormat(uint32_t Instance, uint32_t *PixelFormat)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  *PixelFormat = CameraCtx[Instance].PixelFormat;
  return BSP_ERROR_NONE;
}


/**
  * @brief  Set the camera Resolution.
  * @param  Instance  Camera instance.
  * @param  Resolution Resolution to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetResolution(uint32_t Instance, uint32_t Resolution)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Resolution == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->SetResolution(CompObj, Resolution);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].Resolution = Resolution;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the camera Resolution.
  * @param  Instance  Camera instance.
  * @param  Resolution Resolution to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetResolution(uint32_t Instance, uint32_t *Resolution)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Resolution == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  *Resolution = CameraCtx[Instance].Resolution;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the camera Light Mode.
  * @param  Instance  Camera instance.
  * @param  LightMode Light Mode to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetLightMode(uint32_t Instance, uint32_t LightMode)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->LightMode == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->SetLightMode(CompObj, LightMode);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].LightMode = LightMode;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the camera Light Mode.
  * @param  Instance  Camera instance.
  * @param  LightMode Light Mode to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetLightMode(uint32_t Instance, uint32_t *LightMode)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->LightMode == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  *LightMode = CameraCtx[Instance].LightMode;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the camera Special Effect.
  * @param  Instance Camera instance.
  * @param  ColorEffect Effect to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetColorEffect(uint32_t Instance, uint32_t ColorEffect)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->ColorEffect == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->SetColorEffect(CompObj, ColorEffect);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].ColorEffect = ColorEffect;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the camera Special Effect.
  * @param  Instance Camera instance.
  * @param  ColorEffect Effect to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetColorEffect(uint32_t Instance, uint32_t *ColorEffect)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->ColorEffect == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  *ColorEffect = CameraCtx[Instance].ColorEffect;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the camera Brightness Level.
  * @param  Instance   Camera instance.
  * @param  Brightness Brightness Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetBrightness(uint32_t Instance, int32_t Brightness)
{
  int32_t ret;

  if ((Instance >= CAMERA_INSTANCES_NBR) || ((Brightness < CAMERA_BRIGHTNESS_MIN)
                                             && (Brightness > CAMERA_BRIGHTNESS_MAX)))
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Brightness == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->SetBrightness(CompObj, Brightness);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].Brightness = Brightness;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the camera Brightness Level.
  * @param  Instance Camera instance.
  * @param  Brightness  Brightness Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetBrightness(uint32_t Instance, int32_t *Brightness)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Brightness == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  *Brightness = CameraCtx[Instance].Brightness;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the camera Saturation Level.
  * @param  Instance    Camera instance.
  * @param  Saturation  Saturation Level
  * @retval BSP status
  */
/**
  * @brief  Get the camera Saturation Level.
  * @param  Instance    Camera instance.
  * @param  Saturation  Saturation Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetSaturation(uint32_t Instance, int32_t *Saturation)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Saturation == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  *Saturation = CameraCtx[Instance].Saturation;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the camera Contrast Level.
  * @param  Instance Camera instance.
  * @param  Saturation  Saturation Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetSaturation(uint32_t Instance, int32_t Saturation)
{
  int32_t ret;

  if ((Instance >= CAMERA_INSTANCES_NBR) || ((Saturation < CAMERA_SATURATION_MIN)
                                             && (Saturation > CAMERA_SATURATION_MAX)))
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Saturation == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->SetSaturation(CompObj, Saturation);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].Saturation = Saturation;
  return BSP_ERROR_NONE;
}

int32_t BSP_CAMERA_SetContrast(uint32_t Instance, int32_t Contrast)
{
  int32_t ret;

  if ((Instance >= CAMERA_INSTANCES_NBR) || ((Contrast < CAMERA_CONTRAST_MIN)
                                             && (Contrast > CAMERA_CONTRAST_MAX)))
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Contrast == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->SetContrast(CompObj, Contrast);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].Contrast = Contrast;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the camera Contrast Level.
  * @param  Instance Camera instance.
  * @param  Contrast Contrast Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetContrast(uint32_t Instance, int32_t *Contrast)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Contrast == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  *Contrast = CameraCtx[Instance].Contrast;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the camera Hue Degree.
  * @param  Instance   Camera instance.
  * @param  HueDegree  Hue Degree
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetHueDegree(uint32_t Instance, int32_t HueDegree)
{
  int32_t ret;

  if ((Instance >= CAMERA_INSTANCES_NBR) || ((HueDegree < CAMERA_HUEDEGREE_MIN)
                                             && (HueDegree > CAMERA_HUEDEGREE_MAX)))
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->HueDegree == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->SetHueDegree(CompObj, HueDegree);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].HueDegree = HueDegree;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the camera Hue Degree.
  * @param  Instance   Camera instance.
  * @param  HueDegree  Hue Degree
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetHueDegree(uint32_t Instance, int32_t *HueDegree)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->HueDegree == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  *HueDegree = CameraCtx[Instance].HueDegree;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the camera Mirror/Flip.
  * @param  Instance  Camera instance.
  * @param  MirrorFlip CAMERA_MIRRORFLIP_NONE or any combination of
  *                    CAMERA_MIRRORFLIP_FLIP and CAMERA_MIRRORFLIP_MIRROR
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetMirrorFlip(uint32_t Instance, uint32_t MirrorFlip)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->MirrorFlip == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->MirrorFlipConfig(CompObj, MirrorFlip);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].MirrorFlip = MirrorFlip;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the camera Mirror/Flip.
  * @param  Instance   Camera instance.
  * @param  MirrorFlip Mirror/Flip config
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetMirrorFlip(uint32_t Instance, uint32_t *MirrorFlip)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->MirrorFlip == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  *MirrorFlip = CameraCtx[Instance].MirrorFlip;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set the camera zoom
  * @param  Instance Camera instance.
  * @param  Zoom     Zoom to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetZoom(uint32_t Instance, uint32_t Zoom)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Zoom == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->ZoomConfig(CompObj, Zoom);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  CameraCtx[Instance].Zoom = Zoom;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the camera zoom
  * @param  Instance Camera instance.
  * @param  Zoom     Zoom to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetZoom(uint32_t Instance, uint32_t *Zoom)
{
  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->Zoom == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  *Zoom = CameraCtx[Instance].Zoom;
  return BSP_ERROR_NONE;
}

/**
  * @brief  Enable the camera night mode
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_EnableNightMode(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->NightMode == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->NightModeConfig(CompObj, CAMERA_NIGHT_MODE_SET);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  Disable the camera night mode
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_DisableNightMode(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= CAMERA_INSTANCES_NBR)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  if (CameraCap->NightMode == 0U)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }

  ret = CameraDrv->NightModeConfig(CompObj, CAMERA_NIGHT_MODE_RESET);
  if (ret < 0)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  CAMERA hardware reset
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_HwReset(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

#if defined (MCP_IOEXPANDER)
  BSP_IO_Init_t io_init_structure;

  io_init_structure.Pin  = RSTI_PIN | XSDN_PIN;
  io_init_structure.Pull = IO_PULLDOWN;
  io_init_structure.Mode = IO_MODE_OUTPUT_PP;

  if (BSP_IO_Init(0, &io_init_structure) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else
  {
    BSP_IO_WritePin(0, XSDN_PIN, IO_PIN_SET);
    HAL_Delay(2);
    BSP_IO_WritePin(0, RSTI_PIN, IO_PIN_SET);

  }
#endif /* MCP_IOEXPANDER */

  return ret;
}

/**
  * @}
  */

/** @defgroup CAMERA_Private_Functions Private Functions
  * @{
  */

/**
  * @brief  Get the capture size in pixels unit.
  * @param  Resolution  the current resolution.
  * @param  PixelFormat Camera pixel format
  * @retval capture size in pixels unit.
  */
int32_t GetSize(uint32_t Resolution, uint32_t PixelFormat)
{
  uint32_t size = 0;
  uint32_t pf_div;
  if (PixelFormat == CAMERA_PF_RGB888)
  {
    pf_div = 3; /* each pixel on 3 bytes so 3/4 words */
  }
  else
  {
    pf_div = 2; /* each pixel on 2 bytes so 1/2 words*/
  }
  /* Get capture size */
  switch (Resolution)
  {
    case CAMERA_R160x120:
      size = ((uint32_t)(160 * 120) * pf_div);
      break;
    case CAMERA_R320x240:
      size = ((uint32_t)(320 * 240) * pf_div);
      break;
    case CAMERA_R480x272:
      size = ((uint32_t)(480 * 272) * pf_div);
      break;
    case CAMERA_R640x480:
      size = ((uint32_t)(640 * 480) * pf_div);
      break;
    case CAMERA_R800x480:
      size = ((uint32_t)(800 * 480) * pf_div);
      break;
    default:
      break;
  }

  return (int32_t)size;
}

/**
  * @}
  */

/** @addtogroup _CAMERA_Private_FunctionPrototypes
  * @{
  */
#ifdef USE_CAMERA_SENSOR_S5K5CAG
/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t S5K5CAG_Probe(uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret;
  S5K5CAG_IO_t              IOCtx;
  uint32_t                 id;

  /* Configure the audio driver */
  IOCtx.Address     = CAMERA_S5K5CAG_ADDRESS;
  IOCtx.Init        = BSP_I2C1_Init;
  IOCtx.DeInit      = BSP_I2C1_DeInit;
  IOCtx.ReadReg     = BSP_I2C1_ReadReg16;
  IOCtx.WriteReg    = BSP_I2C1_WriteReg16;
  IOCtx.GetTick     = BSP_GetTick;

  ret = S5K5CAG_RegisterBusIO(&S5K5CAGObj, &IOCtx);
  if (ret != S5K5CAG_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  ret = S5K5CAG_ReadID(&S5K5CAGObj, &id);
  if (ret != S5K5CAG_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  if (id != (uint32_t)S5K5CAG_ID)
  {
    return BSP_ERROR_UNKNOWN_COMPONENT;
  }

  CameraDrv = (CAMERA_Drv_t *) &S5K5CAG_CAMERA_Driver;
  CompObj = &S5K5CAGObj;
  CameraCap = &Cap ;
  ret = CameraDrv->Init(CompObj, Resolution, PixelFormat);
  if (ret != S5K5CAG_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  ret = CameraDrv->GetCapabilities(CompObj, CameraCap);
  if (ret != S5K5CAG_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}
#endif /* USE_CAMERA_SENSOR_S5K5CAG */

#ifdef USE_CAMERA_SENSOR_OV5640
/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
/* #warning !!! On FPGA, OV5640 pixel clock must be reduced, ensure to apply patch !!! */
/*
diff --git a/ov5640.c b/ov5640.c
index 42e14ee04497..5e5d0471542d 100644
--- a/ov5640.c
+++ b/ov5640.c
@@ -145,8 +145,8 @@ int32_t OV5640_Init(OV5640_Object_t *pObj, uint32_t Resolution, uint32_t PixelFo
     {OV5640_PAD_OUTPUT_ENABLE02, 0xf3},
     {OV5640_SC_PLL_CONTRL0, 0x18},
     {OV5640_SYSTEM_CTROL0, 0x02},
-    {OV5640_SC_PLL_CONTRL1, 0x41},
-    {OV5640_SC_PLL_CONTRL2, 0x60},
+    {OV5640_SC_PLL_CONTRL1, 0x40},
+    {OV5640_SC_PLL_CONTRL2, 0x04},
     {OV5640_SC_PLL_CONTRL3, 0x13},
     {OV5640_SYSTEM_ROOT_DIVIDER, 0x01},
     {0x3630, 0x36},
 */
static int32_t OV5640_Probe(uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret;
  OV5640_IO_t              IOCtx;
  uint32_t                  id;

  /* Only perform the init if the object already exist */
  if (CompObj)
  {
    goto init;
  }

  /* Configure the i2c driver */
  IOCtx.Address     = CAMERA_OV5640_ADDRESS;
  IOCtx.Init        = BSP_I2C5_Init;
  IOCtx.DeInit      = BSP_I2C5_DeInit;
  IOCtx.ReadReg     = BSP_I2C5_ReadReg16;
  IOCtx.WriteReg    = BSP_I2C5_WriteReg16;
  IOCtx.GetTick     = BSP_GetTick;

  ret = OV5640_RegisterBusIO(&OV5640Obj, &IOCtx);
  if (ret != OV5640_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  ret = OV5640_ReadID(&OV5640Obj, &id);
  if (ret != OV5640_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  if (id != OV5640_ID)
  {
    return BSP_ERROR_UNKNOWN_COMPONENT;
  }

  CameraDrv = (CAMERA_Drv_t *) &OV5640_CAMERA_Driver;
  CompObj = &OV5640Obj;
  CameraCap = &Cap;

init:
  ret = CameraDrv->Init(CompObj, Resolution, PixelFormat);
  if (ret != OV5640_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  ret = CameraDrv->GetCapabilities(CompObj, CameraCap);
  if (ret != OV5640_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}
#endif /* USE_CAMERA_SENSOR_OV5640 */

#ifdef USE_CAMERA_SENSOR_GC2145
/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t GC2145_Probe(uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret;
  GC2145_IO_t              IOCtx;
  uint32_t                  id;

  /* Only perform the init if the object already exist */
  if (CompObj)
  {
    goto init;
  }

  /* Configure the i2c driver */
  IOCtx.Address     = CAMERA_GC2145_ADDRESS;
  IOCtx.Init        = BSP_I2C5_Init;
  IOCtx.DeInit      = BSP_I2C5_DeInit;
  IOCtx.ReadReg     = BSP_I2C5_ReadReg;
  IOCtx.WriteReg    = BSP_I2C5_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  ret = GC2145_RegisterBusIO(&GC2145Obj, &IOCtx);
  if (ret != GC2145_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  /* A delay of 1ms is given because camera sometimes
    fails to respond to ReadID when first time powered up. */
  HAL_Delay(1);

  ret = GC2145_ReadID(&GC2145Obj, &id);

  if (ret != GC2145_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  if (id != GC2145_ID)
  {
    return BSP_ERROR_UNKNOWN_COMPONENT;
  }

  CameraDrv = (CAMERA_Drv_t *) &GC2145_CAMERA_Driver;
  CompObj = &GC2145Obj;
  CameraCap = &Cap;

init:
  ret = CameraDrv->Init(CompObj, Resolution, PixelFormat);
  if (ret != GC2145_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  ret = CameraDrv->GetCapabilities(CompObj, CameraCap);
  if (ret != GC2145_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}
#endif /* USE_CAMERA_SENSOR_GC2145 */

int32_t BSP_CAMERA_SetColorbarMode(uint32_t Instance, uint32_t Mode)
{
#ifdef USE_CAMERA_SENSOR_OV5640
  if (OV5640_ColorbarModeConfig(CompObj, Mode) != OV5640_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
#else
  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
#endif /* USE_CAMERA_SENSOR_OV5640 */
}

int32_t BSP_CAMERA_SetPolarities(uint32_t Instance, uint32_t Pclk, uint32_t Href, uint32_t Vsync)
{
#ifdef USE_CAMERA_SENSOR_OV5640
  if (OV5640_SetPolarities(CompObj,
                           Pclk ? OV5640_POLARITY_PCLK_HIGH : OV5640_POLARITY_PCLK_LOW,
                           Href ? OV5640_POLARITY_HREF_HIGH : OV5640_POLARITY_HREF_LOW,
                           Vsync ? OV5640_POLARITY_VSYNC_HIGH : OV5640_POLARITY_VSYNC_LOW) != OV5640_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
#else
  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
#endif /* USE_CAMERA_SENSOR_OV5640 */
}
int32_t BSP_CAMERA_SetSynchroCodes(uint32_t Instance, uint32_t FSC, uint32_t LSC, uint32_t LEC, uint32_t FEC)
{
#ifdef USE_CAMERA_SENSOR_OV5640
  OV5640_SyncCodes_t SyncCode;

  SyncCode.FrameStartCode = FSC;
  SyncCode.LineStartCode = LSC;
  SyncCode.LineEndCode = LEC;
  SyncCode.FrameEndCode = FEC;

  if (OV5640_EmbeddedSynchroConfig(CompObj, &SyncCode) != OV5640_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
#else
  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
#endif /* USE_CAMERA_SENSOR_OV5640 */
}

/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t STMIPID02_Probe(uint8_t PixelFormat)
{
  int32_t ret;
  STMIPID02_IO_t            IOCtx;

  /* Only perform the init if the object already exist */
  if (CompObj2)
  {
    goto init;
  }

  /* Configure the i2c driver */
  IOCtx.Address     = STMIPID02_ADDRESS;
  IOCtx.Init        = BSP_I2C5_Init;
  IOCtx.DeInit      = BSP_I2C5_DeInit;
  IOCtx.ReadReg     = BSP_I2C5_ReadReg16;
  IOCtx.WriteReg    = BSP_I2C5_WriteReg16;
  IOCtx.GetTick     = BSP_GetTick;

  ret = STMIPID02_RegisterBusIO(&STMIPID02Obj, &IOCtx);
  if (ret != STMIPID02_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  DeSerializerDrv = (STMIPID02_Drv_t *) &STMIPID02_Driver;
  CompObj2 = &STMIPID02Obj;

init:
  ret = DeSerializerDrv->Init(CompObj2, PixelFormat);
  if (ret != STMIPID02_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}

static int32_t STMIPID02_ClkEnable(void)
{
  int32_t          ret = BSP_ERROR_NONE;
  GPIO_InitTypeDef gpio_init_structure;
  BSP_IO_Init_t    io_init_structure;

#if defined (MCP_IOEXPANDER)
  /* Configure IO functionalities for XSDN pin */
  if (BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  io_init_structure.Mode = IO_MODE_OUTPUT_PP;
  io_init_structure.Pin  = STMIPID02_XSDN_PIN;
  io_init_structure.Pull = IO_PULLDOWN;

  if (BSP_IO_Init(0, &io_init_structure) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else
  {
    if (BSP_IO_WritePin(0, STMIPID02_XSDN_PIN, IO_PIN_SET) != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  HAL_Delay(10);
  BSP_IO_WritePin(0, STMIPID02_XSDN_PIN, IO_PIN_RESET);
  HAL_Delay(10);
  BSP_IO_WritePin(0, STMIPID02_XSDN_PIN, IO_PIN_SET);
#endif /* MCP_IOEXPANDER */

  gpio_init_structure.Pin       = STMIPID02_CLK_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF0_MCO1;
  HAL_GPIO_Init(STMIPID02_CLK_PORT, &gpio_init_structure);

  /* Enable external clock for STMIPID02 */
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);

  return ret;
}

void HAL_DCMIPP_MspInit(DCMIPP_HandleTypeDef *pHdcmipp)
{
  GPIO_InitTypeDef gpio_init_structure;

  /*** Enable peripherals clocks and reset ***/
  __HAL_RCC_DCMIPP_CLK_ENABLE();
  __HAL_RCC_DCMIPP_CLK_SLEEP_DISABLE();
  __HAL_RCC_DCMIPP_RELEASE_RESET();

  /* Enable GPIO clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /*** Configure the GPIO ***/
  /* Configure DCMI GPIO as alternate function */
  /* D0=PA9 AF13 */
  gpio_init_structure.Pin       = GPIO_PIN_9;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMIPP;
  HAL_GPIO_Init(GPIOA, &gpio_init_structure);

  /* PIXCLK=PB7 AF14 */
  gpio_init_structure.Pin       = GPIO_PIN_7;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF14_DCMIPP;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /* D6=PB8 AF13 */
  gpio_init_structure.Pin       = GPIO_PIN_8;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMIPP;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /* D1=PD0 AF13, D5=PD3 AF13*/
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_3;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMIPP;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* D4=PD11 AF14 */
  gpio_init_structure.Pin       = GPIO_PIN_11;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF14_DCMIPP;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* D7=PE14 AF13 */
  gpio_init_structure.Pin       = GPIO_PIN_14;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMIPP;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  /* D3=PE4 AF13 */
  gpio_init_structure.Pin       = GPIO_PIN_4;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMIPP;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  /* HSYNC=PH8 AF13 */
  gpio_init_structure.Pin       = GPIO_PIN_8;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMIPP;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);

  /* VSYNC=PG9 AF13, D2=PG10 AF13 */
  gpio_init_structure.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = GPIO_AF13_DCMIPP;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
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
