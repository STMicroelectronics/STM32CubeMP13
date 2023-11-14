/**
  ******************************************************************************
  * @file    mcp23x17.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the
  *          mcp23x17.c IO expander driver.
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
#ifndef MCP23x17_H
#define MCP23x17_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mcp23x17_reg.h"
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup MCP23x17
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup MCP23x17_Exported_Types MCP23x17 Exported Types
  * @{
  */
typedef struct
{
  uint32_t Pin;       /*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_pins_define */

  uint32_t Mode;      /*!< Specifies the operating mode for the selected pins.
                           This parameter can be a value of @ref IO_ModeTypedef */

  uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                           This parameter can be a value of @ref GPIO_pull_define */
}MCP23x17_IO_Init_t;


typedef int32_t (*MCP23x17_Init_Func)    (void);
typedef int32_t (*MCP23x17_DeInit_Func)  (void);
typedef int32_t (*MCP23x17_GetTick_Func) (void);
typedef int32_t (*MCP23x17_Delay_Func)   (uint32_t);
typedef int32_t (*MCP23x17_WriteReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*MCP23x17_ReadReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  MCP23x17_Init_Func          Init;
  MCP23x17_DeInit_Func        DeInit;
  uint16_t                    Address;
  MCP23x17_WriteReg_Func      WriteReg;
  MCP23x17_ReadReg_Func       ReadReg;
  MCP23x17_GetTick_Func       GetTick;
} MCP23x17_IO_t;


typedef struct
{
  MCP23x17_IO_t         IO;
  mcp23x17_ctx_t        Ctx;
  uint8_t               IsInitialized;
} MCP23x17_Object_t;



/* IO driver structure initialization */
typedef struct
{
  int32_t ( *Init               )(MCP23x17_Object_t *, MCP23x17_IO_Init_t *);
  int32_t ( *DeInit             )(MCP23x17_Object_t *);
  int32_t ( *ReadID             )(MCP23x17_Object_t *, uint32_t*);
  int32_t ( *Reset              )(MCP23x17_Object_t *);
  int32_t ( *IO_Start           )(MCP23x17_Object_t *, uint32_t);
  int32_t ( *IO_WritePin        )(MCP23x17_Object_t *, uint32_t, uint8_t);
  int32_t ( *IO_ReadPin         )(MCP23x17_Object_t *, uint32_t);
  int32_t ( *IO_EnableIT        )(MCP23x17_Object_t *);
  int32_t ( *IO_DisableIT       )(MCP23x17_Object_t *);
  int32_t ( *IO_ITStatus        )(MCP23x17_Object_t *, uint32_t);
  int32_t ( *IO_ClearIT         )(MCP23x17_Object_t *, uint32_t);
}MCP23x17_IO_Mode_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup MCP23x17_Exported_Constants MCP23x17 Exported Constants
  * @{
  */
#define MCP23x17_OK                      (0)
#define MCP23x17_ERROR                   (-1)


/* MCP23x17_REG_ADR_SYS_CTRL values */
#define MCP23x17_SWRST                    0x80U




/* MCP23x17_REG_ADR_MFX_IRQ_OUT values */
#define MCP23x17_OUT_PIN_TYPE_OPENDRAIN   0x00U


/* GPIO: IO Pins definition */
#define MCP23x17_GPIO_PIN_0                0x0001U
#define MCP23x17_GPIO_PIN_1                0x0002U
#define MCP23x17_GPIO_PIN_2                0x0004U
#define MCP23x17_GPIO_PIN_3                0x0008U
#define MCP23x17_GPIO_PIN_4                0x0010U
#define MCP23x17_GPIO_PIN_5                0x0020U
#define MCP23x17_GPIO_PIN_6                0x0040U
#define MCP23x17_GPIO_PIN_7                0x0080U

#define MCP23x17_GPIO_PIN_8                0x0100U
#define MCP23x17_GPIO_PIN_9                0x0200U
#define MCP23x17_GPIO_PIN_10               0x0400U
#define MCP23x17_GPIO_PIN_11               0x0800U
#define MCP23x17_GPIO_PIN_12               0x1000U
#define MCP23x17_GPIO_PIN_13               0x2000U
#define MCP23x17_GPIO_PIN_14               0x4000U
#define MCP23x17_GPIO_PIN_15               0x8000U
#define MCP23x17_GPIO_ALL                  0xFFFFU


/* GPIO: constant */
#define MCP23x17_GPIO_DIR_IN                0x1U
#define MCP23x17_GPIO_DIR_OUT               0x0U
#define MCP23x17_IRQ_GPI_EVT_LEVEL          0x0U
#define MCP23x17_IRQ_GPI_EVT_EDGE           0x1U
#define MCP23x17_IRQ_GPI_TYPE_LLFE          0x0U  /* Low Level Falling Edge */
#define MCP23x17_IRQ_GPI_TYPE_HLRE          0x1U  /*High Level Raising Edge */
#define MCP23x17_GPI_WITHOUT_PULL_RESISTOR  0x0U
#define MCP23x17_GPI_WITH_PULL_RESISTOR     0x1U
#define MCP23x17_GPO_PUSH_PULL              0x0U
#define MCP23x17_GPO_OPEN_DRAIN             0x1U
#define MCP23x17_GPIO_PULL_DOWN             0x0U
#define MCP23x17_GPIO_PULL_UP               0x1U

#define MCP23x17_GPIO_NOPULL                0x0U   /*!< No Pull-up or Pull-down activation  */
#define MCP23x17_GPIO_PULLUP                0x1U   /*!< Pull-up activation                  */
#define MCP23x17_GPIO_PULLDOWN              0x2U   /*!< Pull-down activation                */

#define MCP23x17_GPIO_MODE_OFF                  0x0U  /* when pin isn't used*/
#define MCP23x17_GPIO_MODE_ANALOG               0x1U  /* analog mode */
#define MCP23x17_GPIO_MODE_INPUT                0x2U  /* input floating */
#define MCP23x17_GPIO_MODE_OUTPUT_OD            0x3U  /* Open Drain output without internal resistor */
#define MCP23x17_GPIO_MODE_OUTPUT_PP            0x4U  /* PushPull output without internal resistor */
#define MCP23x17_GPIO_MODE_IT_RISING_EDGE       0x5U  /* float input - irq detect on rising edge */
#define MCP23x17_GPIO_MODE_IT_FALLING_EDGE      0x6U  /* float input - irq detect on falling edge */
#define MCP23x17_GPIO_MODE_IT_LOW_LEVEL         0x7U  /* float input - irq detect on low level */
#define MCP23x17_GPIO_MODE_IT_HIGH_LEVEL        0x8U  /* float input - irq detect on high level */

#define MCP23x17_IRQ_GPIO                 0xFFFFU  /* General GPIO irq (only for SRC_EN and PENDING) */

/* MCP23x17_REG_ADR_MFX_IRQ_OUT values */
#define MCP23x17_OUT_PIN_TYPE_OPENDRAIN   0x00U
#define MCP23x17_OUT_PIN_TYPE_PUSHPULL    0x01U
#define MCP23x17_OUT_PIN_POLARITY_LOW     0x00U
#define MCP23x17_OUT_PIN_POLARITY_HIGH    0x01U
#define MCP23x17_OUT_PIN_MIRROR_OFF       0x00U
#define MCP23x17_OUT_PIN_MIRROR_ON        0x01U


/**
  * @}
  */

/** @addtogroup MCP23x17_Exported_Functions
  * @{
  */

/* MCP23x17 Control functions */
int32_t MCP23x17_RegisterBusIO (MCP23x17_Object_t *pObj, MCP23x17_IO_t *pIO);
int32_t MCP23x17_Init(MCP23x17_Object_t *pObj);
int32_t MCP23x17_DeInit(MCP23x17_Object_t *pObj);
int32_t MCP23x17_Reset(MCP23x17_Object_t *pObj);
int32_t MCP23x17_ReadID(MCP23x17_Object_t *pObj, uint32_t *Id);
int32_t MCP23x17_SetIrqOutPinPolarity(MCP23x17_Object_t *pObj, uint8_t Polarity);
int32_t MCP23x17_SetIrqOutPinType(MCP23x17_Object_t *pObj, uint8_t Type);
int32_t MCP23x17_SetIrqOutPinMirrorMode(MCP23x17_Object_t *pObj, uint8_t Mode);


/* MCP23x17 IO functionalities functions */
int32_t MCP23x17_IO_Init(MCP23x17_Object_t *pObj, MCP23x17_IO_Init_t *IoInit);
int32_t MCP23x17_IO_Start(MCP23x17_Object_t *pObj, uint32_t IO_Pin);
int32_t MCP23x17_IO_WritePin(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t PinState);
int32_t MCP23x17_IO_ReadPin(MCP23x17_Object_t *pObj, uint32_t IO_Pin);
int32_t MCP23x17_IO_EnableIT(MCP23x17_Object_t *pObj);
int32_t MCP23x17_IO_DisableIT(MCP23x17_Object_t *pObj);
int32_t MCP23x17_IO_ITStatus(MCP23x17_Object_t *pObj, uint32_t IO_Pin);
int32_t MCP23x17_IO_ClearIT(MCP23x17_Object_t *pObj, uint32_t IO_Pin);
int32_t MCP23x17_IO_InitPin(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t Direction);
int32_t MCP23x17_IO_SetIrqTypeMode(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t Type);
int32_t MCP23x17_IO_SetIrqEvtMode(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t Evt);
int32_t MCP23x17_IO_EnablePinIT(MCP23x17_Object_t *pObj, uint32_t IO_Pin);
int32_t MCP23x17_IO_DisablePinIT(MCP23x17_Object_t *pObj, uint32_t IO_Pin);
int32_t MCP23x17_IO_SetPullupResistorsMode(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t Mode);


/**
  * @}
  */

/* IO driver structure */
extern MCP23x17_IO_Mode_t MCP23x17_IO_Driver;


#ifdef __cplusplus
}
#endif
#endif /* MCP23x17_H */


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
