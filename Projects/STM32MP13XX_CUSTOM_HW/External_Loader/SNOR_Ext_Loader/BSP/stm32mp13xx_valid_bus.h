/**
  ******************************************************************************
  * @file    stm32mp13xx_valid_bus.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for hardware resources.
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
#ifndef STM32MP13XX_VALID_BUS_H
#define STM32MP13XX_VALID_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif


 /* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_valid_conf.h"
#include "stm32mp13xx_valid_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX_VALID_BUS
  * @{
  */

/** @defgroup STM32MP13XX_VALID_BUS_Exported_Constants BUS Exported Constants
  * @{
  */


/* Exported constant IO ------------------------------------------------------*/

 /**
   * @brief AUDIO Slave I2C address
   */
 #define AUDIO_I2C_ADDRESS                ((uint16_t)0x36)

/**
  * @}
  */
/** @defgroup Exported Constants
  * @{
  */
#if defined (USE_STM32MP13XX_VALID2)
/* Definition for I2C3 clock resources */
#define BUS_I2C3_INSTANCE                      I2C3
#define BUS_I2C3_CLK_ENABLE()                  __HAL_RCC_I2C3_CLK_ENABLE()
#define BUS_I2C3_CLK_DISABLE()                 __HAL_RCC_I2C3_CLK_DISABLE()
#define BUS_I2C3_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C3_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOH_CLK_DISABLE()
#define BUS_I2C3_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C3_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOH_CLK_DISABLE()

#define BUS_I2C3_FORCE_RESET()                 __HAL_RCC_I2C3_FORCE_RESET()
#define BUS_I2C3_RELEASE_RESET()               __HAL_RCC_I2C3_RELEASE_RESET()

/* Definition for I2C3 Pins */
#define BUS_I2C3_SCL_PIN                       GPIO_PIN_3
#define BUS_I2C3_SDA_PIN                       GPIO_PIN_7
#define BUS_I2C3_SCL_GPIO_PORT                 GPIOH
#define BUS_I2C3_SDA_GPIO_PORT                 GPIOH
#define BUS_I2C3_SCL_AF                        GPIO_AF4_I2C3
#define BUS_I2C3_SDA_AF                        GPIO_AF5_I2C3

/* I2C interrupt requests */
#define BUS_I2C3_EV_IRQn                       I2C3_EV_IRQn
#define BUS_I2C3_ER_IRQn                       I2C3_ER_IRQn

#ifndef BUS_I2C3_FREQUENCY
   #define BUS_I2C_FREQUENCY  100000U /* Frequency of I2Cn = 100 KHz*/
#endif
#endif /* VALID2*/

#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
/* Definition for I2C4 clock resources */
#define BUS_I2C4_INSTANCE                      I2C4
#define BUS_I2C4_CLK_ENABLE()                  __HAL_RCC_I2C4_CLK_ENABLE()
#define BUS_I2C4_CLK_DISABLE()                 __HAL_RCC_I2C4_CLK_DISABLE()
#define BUS_I2C4_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUS_I2C4_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define BUS_I2C4_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_I2C4_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()


#define BUS_I2C4_FORCE_RESET()                 __HAL_RCC_I2C4_FORCE_RESET()
#define BUS_I2C4_RELEASE_RESET()               __HAL_RCC_I2C4_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define BUS_I2C4_SCL_PIN                       GPIO_PIN_2
#define BUS_I2C4_SCL_GPIO_PORT                 GPIOE
#define BUS_I2C4_SDA_PIN                       GPIO_PIN_8
#define BUS_I2C4_SDA_GPIO_PORT                 GPIOA
#define BUS_I2C4_SCL_AF                        GPIO_AF4_I2C4
#define BUS_I2C4_SDA_AF                        GPIO_AF4_I2C4

/* I2C interrupt requests */
#define BUS_I2C4_EV_IRQn                       I2C4_EV_IRQn
#define BUS_I2C4_ER_IRQn                       I2C4_ER_IRQn

#ifndef BUS_I2C4_FREQUENCY
   #define BUS_I2C_FREQUENCY  100000U /* Frequency of I2Cn = 100 KHz*/
#endif
#endif /* VALID1 or VALID3*/

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from Bus clock (HSI) = 64 MHz */

#ifndef BUS_I2Cx_TIMING
#define BUS_I2Cx_TIMING                      ((uint32_t)0x10805E89)
#endif /* BUS_I2Cx_TIMING */


/**
  * @}
  */

/** @defgroup Exported Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  pI2C_CallbackTypeDef  pMspI2cInitCb;
  pI2C_CallbackTypeDef  pMspI2cDeInitCb;
}BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */

/** @addtogroup Exported_Variables
  * @{
  */
extern I2C_HandleTypeDef hbus_i2c2;
/**
  * @}
  */

/** @defgroup  Exported Functions
  * @{
  */
#if defined (USE_STM32MP13XX_VALID2)
int32_t BSP_I2C3_Init(void);
int32_t BSP_I2C3_DeInit(void);
int32_t BSP_I2C3_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_Recv(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_Send(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_IsReady(uint16_t DevAddr, uint32_t Trials);
#endif

#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
int32_t BSP_I2C4_Init(void);
int32_t BSP_I2C4_DeInit(void);
int32_t BSP_I2C4_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_Recv(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_Send(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_IsReady(uint16_t DevAddr, uint32_t Trials);
#endif

int32_t BSP_GetTick(void);

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
#if defined (USE_STM32MP13XX_VALID2)
int32_t BSP_I2C3_RegisterDefaultMspCallbacks (void);
int32_t BSP_I2C3_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback);
#endif
#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
int32_t BSP_I2C4_RegisterDefaultMspCallbacks (void);
int32_t BSP_I2C4_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback);
#endif
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

#if defined (USE_STM32MP13XX_VALID2)
__weak HAL_StatusTypeDef MX_I2C3_Init(I2C_HandleTypeDef *hI2c);
#endif
#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
__weak HAL_StatusTypeDef MX_I2C4_Init(I2C_HandleTypeDef *hI2c);
#endif
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

#endif