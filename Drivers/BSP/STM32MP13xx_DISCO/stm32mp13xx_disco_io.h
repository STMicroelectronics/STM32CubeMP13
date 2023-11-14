/**
  ******************************************************************************
  * @file    stm32mp13xx_disco_io.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32mp1xx_disco_io.c driver.
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
#ifndef STM32MP13XX_DISCO_IO_H
#define STM32MP13XX_DISCO_IO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_disco_conf.h"
/* Include common IO driver */
#include "../Common/io.h"
/* Include IO component driver */
#include "../Components/mcp23x17/mcp23x17.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX_DISCO
  * @{
  */

/** @addtogroup STM32MP13XX_DISCO_IO
  * @{
  */

/** @defgroup STM32MP13XX_DISCO_IO_Exported_Types IO Exported Types
  * @{
  */
typedef struct
{
  uint32_t                    IsInitialized;     /*   IO_IsInitialized    */
  uint32_t                    Functions;         /*   Selected functions  */
}IOEXPANDER_Ctx_t;

typedef struct
{
  uint32_t Pin;       /*!< Specifies the IO pins to be configured */
  uint32_t Mode;      /*!< Specifies the operating mode for the selected pins */
  uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins */
}BSP_IO_Init_t;

/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_IO_Exported_Constants IO Exported Constants
  * @{
  */
/**
  * @brief IOExpander modes
  */
#define IOEXPANDER_IO_MODE        1U
#define IOEXPANDER_IDD_MODE       2U /* Not used */
#define IOEXPANDER_TS_MODE        4U /* Not used */

/**
  * @brief IOExpander instances number
  */
#define IOEXPANDER_INSTANCES_NBR  1U

/**
  * @brief IOExpander pins control
  */
#define IO_PIN_RESET              0U
#define IO_PIN_SET                1U

/**
  * @brief IOExpander IOs definition
  */
#define IO_PIN_0                  0x0001U
#define IO_PIN_1                  0x0002U
#define IO_PIN_2                  0x0004U
#define IO_PIN_3                  0x0008U
#define IO_PIN_4                  0x0010U
#define IO_PIN_5                  0x0020U
#define IO_PIN_6                  0x0040U
#define IO_PIN_7                  0x0080U
#define IO_PIN_8                  0x0100U
#define IO_PIN_9                  0x0200U
#define IO_PIN_10                 0x0400U
#define IO_PIN_11                 0x0800U
#define IO_PIN_12                 0x1000U
#define IO_PIN_13                 0x2000U
#define IO_PIN_14                 0x4000U
#define IO_PIN_15                 0x8000U
#define IO_PIN_ALL                0xFFFFU

/**
  * @brief IOExpander IOs pull define
  */
#define IO_NOPULL                 0U   /*!< No Pull-up or Pull-down activation  */
#define IO_PULLUP                 1U   /*!< Pull-up activation                  */
#define IO_PULLDOWN               2U   /*!< Pull-down activation                */

/**
  * @brief IOExpander IOs mode define
  */
#define IO_MODE_OFF               0U  /* when pin isn't used*/
#define IO_MODE_ANALOG            1U  /* analog mode */
#define IO_MODE_INPUT             2U  /* input floating */
#define IO_MODE_OUTPUT_OD         3U  /* Open Drain output without internal resistor */
#define IO_MODE_OUTPUT_PP         4U  /* PushPull output without internal resistor */
#define IO_MODE_IT_RISING_EDGE    5U  /* float input - irq detect on rising edge */
#define IO_MODE_IT_FALLING_EDGE   6U  /* float input - irq detect on falling edge */
#define IO_MODE_IT_LOW_LEVEL      7U  /* float input - irq detect on low level */
#define IO_MODE_IT_HIGH_LEVEL     8U  /* float input - irq detect on high level */

/**
  * @brief MCP_IRQOUT pin
  */
#define MCP_IRQOUT_PIN                    GPIO_PIN_12
#define MCP_IRQOUT_GPIO_PORT              GPIOG
#define MCP_IRQOUT_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOG_CLK_ENABLE()
#define MCP_IRQOUT_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOG_CLK_DISABLE()
#define MCP_IRQOUT_EXTI_IRQn              EXTI1_IRQn
#define MCP_IRQOUT_EXTI_IRQnHandler       EXTI1_IRQHandler

#define IO_I2C_ADDRESS                    0x42U
/**
  * @}
  */

/** @addtogroup STM32MP13XX_DISCO_IO_Exported_Functions
  * @{
  */
int32_t BSP_IO_Init(uint32_t Instance, BSP_IO_Init_t *Init);
int32_t BSP_IO_DeInit(uint32_t Instance);

int32_t BSP_IO_GetIT(uint32_t Instance, uint32_t Pins);
int32_t BSP_IO_ClearIT(uint32_t Instance, uint32_t Pins);

int32_t BSP_IO_WritePin(uint32_t Instance, uint32_t Pin, uint32_t PinState);
int32_t BSP_IO_ReadPin(uint32_t Instance, uint32_t Pin);
int32_t BSP_IO_TogglePin(uint32_t Instance, uint32_t Pin);


int32_t BSP_IOEXPANDER_Init(uint32_t Instance, uint32_t Functions);
int32_t BSP_IOEXPANDER_DeInit(uint32_t Instance);
void BSP_IOEXPANDER_ITConfig(void);

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

#endif /* STM32MP13XX_DISCO_IO_H */