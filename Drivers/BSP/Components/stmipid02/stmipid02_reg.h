/**
  ******************************************************************************
  * @file    stmipid02_reg.h
  * @author  GPM Application Team
  * @brief   Header of stmipid02_reg.c
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
#ifndef STMIPID02_REG_H
#define STMIPID02_REG_H

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

/** @addtogroup STMIPID02
  * @{
  */

/** @defgroup STMIPID02_Exported_Types Exported Types
  * @{
  */
/* Exported types ------------------------------------------------------------*/
typedef int32_t (*STMIPID02_Write_Func)(void *, uint16_t, uint8_t *, uint16_t);
typedef int32_t (*STMIPID02_Read_Func)(void *, uint16_t, uint8_t *, uint16_t);

typedef struct
{
  STMIPID02_Write_Func   WriteReg;
  STMIPID02_Read_Func    ReadReg;
  void                   *handle;
} stmipid02_ctx_t;
/**
  * @}
  */

/** @defgroup STMIPID02_Exported_Functions STMIPID02 Exported Functions
  * @{
  */
/* Exported functions ------------------------------------------------------- */
int32_t STMIPID02_write_reg(stmipid02_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
int32_t STMIPID02_read_reg(stmipid02_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
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

#endif /* STMIPID02_REG_H */

