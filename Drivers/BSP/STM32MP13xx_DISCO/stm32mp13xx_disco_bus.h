/**
  ******************************************************************************
  * @file    stm32mp13xx_disco_bus.h
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
#ifndef STM32MP13XX_DISCO_BUS_H
#define STM32MP13XX_DISCO_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif


 /* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_disco_conf.h"
#include "stm32mp13xx_disco_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX_DISCO_BUS
  * @{
  */

/** @defgroup STM32MP13XX_DISCO_BUS_Exported_Constants BUS Exported Constants
  * @{
  */


/* Exported constant IO ------------------------------------------------------*/

 /**
   * @brief AUDIO Slave I2C address
   */
 #define AUDIO_I2C_ADDRESS                ((uint16_t)0x36U)

/**
  * @brief STPMU1 I2C address
  * (NVM_SECTOR3_REGISTER_7 & 0x7F) << 1
  */
#define STPMU1_I2C_ADDRESS               ((0x33U & 0x7FU) << 1 )

/* I2C clock speed configuration (in Hz)
   WARNING:
   Make sure that this define is not already declared in other files (ie.
   stm32mp13xx_disco.h file). It can be used in parallel by other modules. */
#ifndef I2C_SPEED
 #define I2C_SPEED                        ((uint32_t)100000U)
#endif /* I2C_SPEED */

/* Definition for I2C5 clock resources */
#define BUS_I2C5_INSTANCE                      I2C5
#define BUS_I2C5_CLK_ENABLE()                  __HAL_RCC_I2C5_CLK_ENABLE()
#define BUS_I2C5_CLK_DISABLE()                 __HAL_RCC_I2C5_CLK_DISABLE()
#define BUS_I2C5_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUS_I2C5_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOD_CLK_DISABLE()
#define BUS_I2C5_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOH_CLK_ENABLE()
#define BUS_I2C5_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOH_CLK_DISABLE()

#define BUS_I2C5_FORCE_RESET()                 __HAL_RCC_I2C5_FORCE_RESET()
#define BUS_I2C5_RELEASE_RESET()               __HAL_RCC_I2C5_RELEASE_RESET()

/* Definition for I2C5 Pins */
#define BUS_I2C5_SCL_PIN                       GPIO_PIN_1
#define BUS_I2C5_SDA_PIN                       GPIO_PIN_6
#define BUS_I2C5_SCL_GPIO_PORT                 GPIOD
#define BUS_I2C5_SDA_GPIO_PORT                 GPIOH
#define BUS_I2C5_SCL_AF                        GPIO_AF4_I2C5
#define BUS_I2C5_SDA_AF                        GPIO_AF4_I2C5

/* I2C interrupt requests */
#define BUS_I2C5_EV_IRQn                       I2C5_EV_IRQn
#define BUS_I2C5_ER_IRQn                       I2C5_ER_IRQn

#ifndef BUS_I2C5_FREQUENCY
   #define BUS_I2C_FREQUENCY  100000U /* Frequency of I2Cn = 100 KHz*/
#endif

/* Definition for I2C4 clock resources */
#define BUS_I2C4_INSTANCE                      I2C4
#define BUS_I2C4_CLK_ENABLE()                  __HAL_RCC_I2C4_CLK_ENABLE()
#define BUS_I2C4_CLK_DISABLE()                 __HAL_RCC_I2C4_CLK_DISABLE()
#define BUS_I2C4_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUS_I2C4_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define BUS_I2C4_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C4_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()


#define BUS_I2C4_FORCE_RESET()                 __HAL_RCC_I2C4_FORCE_RESET()
#define BUS_I2C4_RELEASE_RESET()               __HAL_RCC_I2C4_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define BUS_I2C4_SCL_PIN                       GPIO_PIN_15
#define BUS_I2C4_SCL_GPIO_PORT                 GPIOE
#define BUS_I2C4_SDA_PIN                       GPIO_PIN_9
#define BUS_I2C4_SDA_GPIO_PORT                 GPIOB
#define BUS_I2C4_SCL_AF                        GPIO_AF6_I2C4
#define BUS_I2C4_SDA_AF                        GPIO_AF6_I2C4

/* I2C interrupt requests */
#define BUS_I2C4_EV_IRQn                       I2C4_EV_IRQn
#define BUS_I2C4_ER_IRQn                       I2C4_ER_IRQn

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from Bus clock (HSI) = 64 MHz */

#ifndef BUS_I2Cx_TIMING
#define BUS_I2Cx_TIMING                      ((uint32_t)0x10805E89U)
#endif /* BUS_I2Cx_TIMING */

/* Definition for I2C1 clock resources */
#define BUS_I2C1_INSTANCE                      I2C1
#define BUS_I2C1_CLK_ENABLE()                  __HAL_RCC_I2C1_CLK_ENABLE()
#define BUS_I2C1_CLK_DISABLE()                 __HAL_RCC_I2C1_CLK_DISABLE()
#define BUS_I2C1_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUS_I2C1_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOD_CLK_DISABLE()
#define BUS_I2C1_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUS_I2C1_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()


#define BUS_I2C1_FORCE_RESET()                 __HAL_RCC_I2C1_FORCE_RESET()
#define BUS_I2C1_RELEASE_RESET()               __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define BUS_I2C1_SCL_PIN                       GPIO_PIN_12
#define BUS_I2C1_SCL_GPIO_PORT                 GPIOD
#define BUS_I2C1_SDA_PIN                       GPIO_PIN_8
#define BUS_I2C1_SDA_GPIO_PORT                 GPIOE
#define BUS_I2C1_SCL_AF                        GPIO_AF5_I2C1
#define BUS_I2C1_SDA_AF                        GPIO_AF5_I2C1

/* I2C interrupt requests */
#define BUS_I2C1_EV_IRQn                       I2C1_EV_IRQn
#define BUS_I2C1_ER_IRQn                       I2C1_ER_IRQn


/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_BUS_Exported_Types BUS Exported Types
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

/** @addtogroup STM32MP13XX_DISCO_BUS_Exported_Variables BUS Exported Variables
  * @{
  */
extern I2C_HandleTypeDef hbus_i2c2;
/**
  * @}
  */

/** @defgroup  STM32MP13XX_DISCO_BUS_Exported_Functions BUS Exported Functions
  * @{
  */
int32_t BSP_I2C5_Init(void);
int32_t BSP_I2C5_DeInit(void);
int32_t BSP_I2C5_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_Recv(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_Send(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C5_IsReady(uint16_t DevAddr, uint32_t Trials);

int32_t BSP_I2C4_Init(void);
int32_t BSP_I2C4_DeInit(void);
int32_t BSP_I2C4_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_Recv(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_Send(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C4_IsReady(uint16_t DevAddr, uint32_t Trials);

int32_t BSP_I2C1_Init(void);
int32_t BSP_I2C1_DeInit(void);
int32_t BSP_I2C1_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_Recv(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_Send(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C1_IsReady(uint16_t DevAddr, uint32_t Trials);

int32_t BSP_GetTick(void);

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_I2C5_RegisterDefaultMspCallbacks (void);
int32_t BSP_I2C5_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback);
int32_t BSP_I2C4_RegisterDefaultMspCallbacks (void);
int32_t BSP_I2C4_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback);
int32_t BSP_I2C1_RegisterDefaultMspCallbacks (void);
int32_t BSP_I2C1_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
__weak HAL_StatusTypeDef MX_I2C5_Init(I2C_HandleTypeDef *phI2c);
__weak HAL_StatusTypeDef MX_I2C4_Init(I2C_HandleTypeDef *phI2c);
__weak HAL_StatusTypeDef MX_I2C1_Init(I2C_HandleTypeDef *phI2c);

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