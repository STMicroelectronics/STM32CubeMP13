/**
  ******************************************************************************
  * @file    stmipid02.h
  * @author  GPM Application Team
  * @brief   This file contains all the functions prototypes for the stmipid02.c
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
#ifndef STMIPID02_H
#define STMIPID02_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stmipid02_reg.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup stmipid02
  * @{
  */

/** @defgroup STMIPID02_Exported_Types Exported Types
  * @{
  */
/* Exported types ------------------------------------------------------------*/
typedef int32_t (*STMIPID02_Init_Func)(void);
typedef int32_t (*STMIPID02_DeInit_Func)(void);
typedef int32_t (*STMIPID02_GetTick_Func)(void);
typedef int32_t (*STMIPID02_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*STMIPID02_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  STMIPID02_Init_Func          Init;
  STMIPID02_DeInit_Func        DeInit;
  uint16_t                     Address;
  STMIPID02_WriteReg_Func      WriteReg;
  STMIPID02_ReadReg_Func       ReadReg;
  STMIPID02_GetTick_Func       GetTick;
} STMIPID02_IO_t;

typedef struct
{
  STMIPID02_IO_t         IO;
  stmipid02_ctx_t        Ctx;
  uint8_t                IsInitialized;
} STMIPID02_Object_t;

typedef struct
{
  int32_t (*Init)(STMIPID02_Object_t *, uint8_t);
  int32_t (*DeInit)(STMIPID02_Object_t *);

} STMIPID02_Drv_t;
/**
  * @}
  */

/** @defgroup STMIPID02_Exported_Constants
  * @{
  */
/* Exported constants --------------------------------------------------------*/
#define STMIPID02_OK                      (0)
#define STMIPID02_ERROR                   (-1)
/**
  * @brief  STMIPID02 Features Parameters
  */
/* Camera Pixel Format */
#define STMIPID02_PF_RGB565                0U     /* Pixel Format RGB565         */
#define STMIPID02_PF_YUV422                2U     /* Pixel Format YUV422         */
#define STMIPID02_PF_RGGB8                 6U     /* Pixel Format RAW_RGGB8      */

/**
  * @}
  */
/** @defgroup STMIPID02_Exported_Variables STMIPID02 Exported Variables
  * @{
  */
/* Exported variables --------------------------------------------------------*/
extern STMIPID02_Drv_t   STMIPID02_Driver;
/**
  * @}
  */
/** @defgroup STMIPID02_Exported_Functions STMIPID02 Exported Functions
  * @{
  */
/* Exported functions ------------------------------------------------------- */
int32_t STMIPID02_RegisterBusIO(STMIPID02_Object_t *pObj, STMIPID02_IO_t *pIO);
int32_t STMIPID02_Init(STMIPID02_Object_t *pObj, uint8_t PixelFormat);
int32_t STMIPID02_DeInit(STMIPID02_Object_t *pObj);

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

#endif /* STMIPID02_H */
