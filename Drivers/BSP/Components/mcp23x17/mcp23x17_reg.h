/**
  ******************************************************************************
  * @file    mcp23x17_reg.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          mcp23x17_reg.c IO expander driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020(-2021) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MCP23x17_REG_H
#define MCP23x17_REG_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
#include <stdint.h>
   
/* Includes ------------------------------------------------------------------*/
/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Component
  * @{
  */
    
/** @defgroup MCP23x17
  * @{
  */    
  
/** @addtogroup MCP23x17_Exported_Constants
  * @{
  */

#define MCP23x17_IODIRA         0x00U
#define MCP23x17_IODIRB         0x01U
#define MCP23x17_IPOLA          0x02U
#define MCP23x17_IPOLB          0x03U
#define MCP23x17_GPINTENA       0x04U
#define MCP23x17_GPINTENB       0x05U
#define MCP23x17_DEFVALA        0x06U
#define MCP23x17_DEFVALB        0x07U
#define MCP23x17_INTCONA        0x08U
#define MCP23x17_INTCONB        0x09U
#define MCP23x17_IOCONA         0x0AU
#define MCP23x17_IOCONB         0x0BU
#define MCP23x17_GPPUA          0x0CU
#define MCP23x17_GPPUB          0x0DU
#define MCP23x17_NTFA           0x0EU
#define MCP23x17_NTFB           0x0FU
#define MCP23x17_INTCAPA        0x10U
#define MCP23x17_INTCAPB        0x11U
#define MCP23x17_GPIOA          0x12U
#define MCP23x17_GPIOB          0x13U
#define MCP23x17_OLATA          0x14U
#define MCP23x17_OLATB          0x15U

/** @addtogroup MCP23x17_Exported_Types
  * @{
  */

typedef int32_t (*MCP23x17_Write_Func)(void *, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*MCP23x17_Read_Func) (void *, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  MCP23x17_Write_Func   WriteReg;
  MCP23x17_Read_Func    ReadReg;
  void                  *handle;
  /* Internal resources */
} mcp23x17_ctx_t;

/**
  * @}
  */ 

/** @addtogroup MCP23x17_Exported_Functions
  * @{
  */
int32_t mcp23x17_write_reg(mcp23x17_ctx_t *ctx, uint16_t reg, uint8_t *data, uint16_t length);
int32_t mcp23x17_read_reg(mcp23x17_ctx_t *ctx, uint16_t reg, uint8_t *data, uint16_t length);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /* MCP23x17_REG_H */


/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */       
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
