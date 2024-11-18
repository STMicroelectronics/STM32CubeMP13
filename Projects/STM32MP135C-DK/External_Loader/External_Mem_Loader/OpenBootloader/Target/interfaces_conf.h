/**
  ******************************************************************************
  * @file    interfaces_conf.h
  * @author  MCD Application Team
  * @brief   Contains Interfaces configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERFACES_CONF_H
#define INTERFACES_CONF_H

#define MEMORIES_SUPPORTED                2U

/* ------------------------- Definitions for USART -------------------------- */
#if defined (STM32MP257Cxx)
#define USARTx                            USART2
#define USARTx_CLK_ENABLE()               __HAL_RCC_USART2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_TX_PIN                     GPIO_PIN_4
#define USARTx_TX_GPIO_PORT               GPIOA
#define USARTx_TX_ALTERNATE               GPIO_AF6_USART2
#define USARTx_RX_PIN                     GPIO_PIN_8
#define USARTx_RX_GPIO_PORT               GPIOA
#define USARTx_RX_ALTERNATE               GPIO_AF8_USART2
#elif defined (STM32MP157Cxx)
#define USARTx                            UART4
#define USARTx_CLK_ENABLE()               __HAL_RCC_UART4_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()

#define USARTx_TX_PIN                     GPIO_PIN_11
#define USARTx_TX_GPIO_PORT               GPIOG
#define USARTx_TX_ALTERNATE               GPIO_AF6_UART4
#define USARTx_RX_PIN                     GPIO_PIN_2
#define USARTx_RX_GPIO_PORT               GPIOB
#define USARTx_RX_ALTERNATE               GPIO_AF8_UART4
#else
#define USARTx                            UART4
#define USARTx_CLK_ENABLE()               __HAL_RCC_UART4_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_TX_PIN                     GPIO_PIN_6
#define USARTx_TX_GPIO_PORT               GPIOD
#define USARTx_TX_ALTERNATE               GPIO_AF8_UART4
#define USARTx_RX_PIN                     GPIO_PIN_8
#define USARTx_RX_GPIO_PORT               GPIOD
#define USARTx_RX_ALTERNATE               GPIO_AF8_UART4
#endif


#endif /* INTERFACES_CONF_H */
