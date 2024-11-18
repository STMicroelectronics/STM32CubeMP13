/**
  ******************************************************************************
  * @file    stm32mp13xx_valid.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32MP13xx VALIDATION  LEDs,
  *          push-buttons and COM ports hardware resources.
  *          VALID1 / VALID2 or VALID3 have to be selected
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
#ifndef __STM32MP13XX_VALID_H
#define __STM32MP13XX_VALID_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID2) || defined (USE_STM32MP13XX_VALID3)
#include "stm32mp13xx_hal.h"

#include "stm32mp13xx_valid_conf.h"
#include "stm32mp13xx_valid_errno.h"
#else
#error "Need to select a VALID board :USE_STM32MP13XX_VALID1 , or USE_STM32MP13XX_VALID2, or USE_STM32MP13XX_VALID3"
#endif
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX_VALID STM32MP13XX_VALID
  * @{
  */

/** @defgroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Types Exported Types
  * @{
  */

typedef enum
{
#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
LED2 = 0U,
LED_GREEN = LED2,
LED3 = 1U,
LED_RED = LED3,
LED4 = 2U,
LED_ORANGE = LED4,
LED5 = 3U,
LED_BLUE = LED5,
#endif
#if defined (USE_STM32MP13XX_VALID2)
LED1 = 0U,
LED_RED = LED1,
LED2 = 1U,
LED_GREEN = LED2,
LED3 = 2U,
LED_BLUE = LED3,
LED4 = 3U,
LED_ORANGE = LED4,
#endif
LEDn
}Led_TypeDef;


typedef enum
{
  BUTTON_WAKEUP = 0U,
  BUTTON_USER = 1U,
  BUTTON_USER2 = 2U,
  BUTTONn
}Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE == 1)
typedef enum
{
  COM1 = 0,
  COMn
}COM_TypeDef;

#ifdef HAL_UART_MODULE_ENABLED
typedef enum
{
 COM_STOPBITS_1     =   UART_STOPBITS_1,
 COM_STOPBITS_2     =   UART_STOPBITS_2,
}COM_StopBitsTypeDef;

typedef enum
{
 COM_PARITY_NONE     =  UART_PARITY_NONE,
 COM_PARITY_EVEN     =  UART_PARITY_EVEN,
 COM_PARITY_ODD      =  UART_PARITY_ODD,
}COM_ParityTypeDef;


typedef enum
{
 COM_HWCONTROL_NONE    =  UART_HWCONTROL_NONE,
 COM_HWCONTROL_RTS     =  UART_HWCONTROL_RTS,
 COM_HWCONTROL_CTS     =  UART_HWCONTROL_CTS,
 COM_HWCONTROL_RTS_CTS =  UART_HWCONTROL_RTS_CTS,
}COM_HwFlowCtlTypeDef;

typedef struct
{
  uint32_t             BaudRate;
  uint32_t             WordLength;
  COM_StopBitsTypeDef  StopBits;
  COM_ParityTypeDef    Parity;
  COM_HwFlowCtlTypeDef HwFlowCtl;
}COM_InitTypeDef;
#endif

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  pUART_CallbackTypeDef  pMspInitCb;
  pUART_CallbackTypeDef  pMspDeInitCb;
}BSP_COM_Cb_t;
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1) */
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/** @defgroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Constants Exported Constants
  * @{
  */

/**
  * @brief  Define for STM32MP13XX_VALID board
  */
#if !defined (USE_STM32MP13XX_VALID)
 #define USE_STM32MP13XX_VALID
#endif


/**
  * @}
  */

  /** @addtogroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Constants Exported Constants
  * @{
  */

/** @addtogroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Constants_Group1 LED Constants
  * @{
  */

#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
#define LED2_GPIO_PORT                   GPIOA
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define LED2_PIN                         GPIO_PIN_14

#define LED3_GPIO_PORT                   GPIOA
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define LED3_PIN                         GPIO_PIN_13

#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()
#define LED4_PIN                         GPIO_PIN_3

#define LED5_GPIO_PORT                   GPIOB
#define LED5_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()
#define LED5_PIN                         GPIO_PIN_10
#endif

#if defined (USE_STM32MP13XX_VALID2)
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define LED1_PIN                         GPIO_PIN_13

#define LED2_GPIO_PORT                   GPIOA
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define LED2_PIN                         GPIO_PIN_11

#define LED3_GPIO_PORT                   GPIOA
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define LED3_PIN                         GPIO_PIN_7

#define LED4_GPIO_PORT                   GPIOA
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define LED4_PIN                         GPIO_PIN_2
#endif

/**
  * @}
  */

/** @addtogroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Constants_Group2 BUTTON Constants
  * @{
  */

/* Button state */
#define BUTTON_RELEASED                     1U
#define BUTTON_PRESSED                      0U

/**
  * @brief Wakeup push-button
  */

#define WAKEUP_BUTTON_PIN                   GPIO_PIN_3
#define WAKEUP_BUTTON_GPIO_PORT             GPIOI
#define WAKEUP_BUTTON_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOI_CLK_ENABLE()
#define WAKEUP_BUTTON_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOI_CLK_DISABLE()
#define WAKEUP_BUTTON_EXTI_LINE             EXTI_LINE_3
#define WAKEUP_BUTTON_EXTI_IRQn             EXTI3_IRQn

/**
  * @brief User push-button
  * Button 'B2' for all Validation board
  */
#define USER2_BUTTON_PIN                       GPIO_PIN_13
#define USER2_BUTTON_GPIO_PORT                 GPIOA
#define USER2_BUTTON_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER2_BUTTON_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()
#define USER2_BUTTON_EXTI_LINE                 EXTI_LINE_13
#define USER2_BUTTON_EXTI_IRQn                 EXTI13_IRQn

/**
  * @brief User2 push-button
  * Button 'B3' for all Validation board
  */
#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
#define USER_BUTTON_PIN                       GPIO_PIN_14
#define USER_BUTTON_GPIO_PORT                 GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()
#define USER_BUTTON_EXTI_LINE                 EXTI_LINE_14
#define USER_BUTTON_EXTI_IRQn                 EXTI14_IRQn
#endif
#if defined (USE_STM32MP13XX_VALID2)
#define USER_BUTTON_PIN                       GPIO_PIN_11
#define USER_BUTTON_GPIO_PORT                 GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()
#define USER_BUTTON_EXTI_LINE                 EXTI_LINE_11
#define USER_BUTTON_EXTI_IRQn                 EXTI11_IRQn
#endif

/**
  * @}
  */

/** @addtogroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Constants_Group3 COM Constants
  * @{
  */

/**
 * @brief Definition for COM port1, connected to UART4
 */
#if (USE_BSP_COM_FEATURE == 1)
#define COM1_UART                     UART4
#define COM1_CLK_ENABLE()             __HAL_RCC_UART4_CLK_ENABLE()
#define COM1_CLK_DISABLE()            __HAL_RCC_UART4_CLK_DISABLE()

#define COM1_TX_PIN                   GPIO_PIN_6
#define COM1_TX_GPIO_PORT             GPIOD
#define COM1_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define COM1_TX_AF                    GPIO_AF8_UART4

#define COM1_RX_PIN                   GPIO_PIN_8
#define COM1_RX_GPIO_PORT             GPIOD
#define COM1_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()
#define COM1_RX_AF                    GPIO_AF8_UART4

#define COM1_POLL_TIMEOUT             1000U

#define COM1_IRQn                     UART4_IRQn
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Functions Exported Functions
  * @{
  */
uint32_t         BSP_GetVersion(void);
void             BSP_Error_Handler(void);

/** @addtogroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Functions_Group1 LED Functions
  * @{
  */
int32_t         BSP_LED_Init(Led_TypeDef Led);
int32_t         BSP_LED_DeInit(Led_TypeDef Led);
int32_t         BSP_LED_On(Led_TypeDef Led);
int32_t         BSP_LED_Off(Led_TypeDef Led);
int32_t         BSP_LED_Toggle(Led_TypeDef Led);
int32_t         BSP_LED_GetState (Led_TypeDef Led);
/**
  * @}
  */

/** @addtogroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Functions_Group2 BUTTON Functions
  * @{
  */
int32_t         BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t         BSP_PB_DeInit(Button_TypeDef Button);
uint32_t         BSP_PB_GetState(Button_TypeDef Button);
void             BSP_PB_Callback(Button_TypeDef Button);
void             BSP_PB_IRQHandler(Button_TypeDef Button);
void             BSP_PB_WAKEUP_EXTI_LINE_0_IRQHandler(void);
void             BSP_PB_USER_EXTI_LINE_14_IRQHandler(void);
void             BSP_PB_USER2_EXTI_LINE_13_IRQHandler(void);
/**
  * @}
  */

/** @addtogroup STM32MP13XX_VALID_LOW_LEVEL_Exported_Functions_Group3 COM Functions
  * @{
  */
#if (USE_BSP_COM_FEATURE == 1)
#ifdef HAL_UART_MODULE_ENABLED
int32_t         BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t         BSP_COM_DeInit(COM_TypeDef COM);
#if( USE_COM_LOG == 1)
int32_t  BSP_COM_SelectLogPort (COM_TypeDef COM);
#endif
HAL_StatusTypeDef MX_UART4_Init(UART_HandleTypeDef *huart, COM_InitTypeDef *COM_Init) ;
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#endif
#endif /* (USE_BSP_COM_FEATURE == 1) */

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

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32MP13XX_VALID_H */
