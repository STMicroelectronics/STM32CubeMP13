/**
  ******************************************************************************
  * @file    gc2145_reg.h
  * @author  GPM Application Team
  * @brief   Header of gc2145_reg.c
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
#ifndef GC2145_REG_H
#define GC2145_REG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <cmsis_compiler.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup GC2145
  * @{
  */

/** @defgroup GC2145_Exported_Types Exported Types
  * @{
  */
/* Exported types ------------------------------------------------------------*/
typedef int32_t (*GC2145_Write_Func)(void *, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*GC2145_Read_Func)(void *, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  GC2145_Write_Func   WriteReg;
  GC2145_Read_Func    ReadReg;
  void                *handle;
} gc2145_ctx_t;

/**
  * @}
  */

/** @defgroup GC2145_Exported_Constants GC2145 Exported Constants
  * @{
  */
/* Exported constants --------------------------------------------------------*/
#define GC2145_ID                 0x2145

#define GC2145_RESET_RELATED      0xFEU
#define GC2145_ANALOG_MODE_1      0x17U
#define GC2145_SPECIAL_EFFECT     0x83U
/**
  * @}
  */

/** @defgroup GC2145_Exported_Functions GC2145 Exported Functions
  * @{
  */
/* Exported functions ------------------------------------------------------- */
int32_t gc2145_write_reg(gc2145_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
int32_t gc2145_read_reg(gc2145_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
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

#endif /* GC2145_REG_H */

