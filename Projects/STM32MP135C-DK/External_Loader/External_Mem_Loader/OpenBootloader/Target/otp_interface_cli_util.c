/**
  ******************************************************************************
  * @file    otp_interface_cli_util.c
  * @author  MCD Application Team
  * @brief   This file contains common funtions used for otp interface cli
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "otp_interface_cli_util.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HAL_TIMEOUT_VALUE   HAL_MAX_DELAY

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration, used for logging */
UART_HandleTypeDef huart;
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is executed to configure in case of error occurrence.
  * @param  None
  * @retval None
  */
void UART_Config(void)
{
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
                      BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = parity none
      - BaudRate    = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  huart.Instance        = USARTx;
  huart.Init.BaudRate   = MX_UART_BAUDRATE;
  huart.Init.WordLength = MX_UART_WORDLENGTH;
  huart.Init.StopBits   = MX_UART_STOPBITS;
  huart.Init.Parity     = MX_UART_PARITY;
  huart.Init.HwFlowCtl  = MX_UART_HWFLOWCTL;
  huart.Init.Mode       = MX_UART_MODE;
  huart.Init.OverSampling = MX_UART_OVERSAMPLING;

  /* Clear UART configuration if need be */
  if(HAL_UART_DeInit(&huart) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if(HAL_UART_Init(&huart) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }


  if(HAL_UARTEx_EnableFifoMode(&huart) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if (HAL_UARTEx_SetTxFifoThreshold(&huart, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  if (HAL_UARTEx_SetRxFifoThreshold(&huart, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

}

/**
  * @brief  Gets numeric values from the Hyperterminal.
  * @param  Maximun value allowed (value)
  * @retval The character received
  */
uint32_t Serial_Scanf(uint32_t value)
{
  uint16_t tmp = 0;

#ifndef __TERMINAL_IO__
  __HAL_UART_CLEAR_OREFLAG(&huart);
  /* e.g. read a character from the EVAL_COM1 and Loop until RXNE = 1 */
  HAL_UART_Receive(&huart, (uint8_t *)&tmp, 1, HAL_TIMEOUT_VALUE);
#else
  tmp = getchar();
#endif
  if (tmp > value)
  {
    printf("\n\r  !!! Please enter valid number between 0 and %lu \n", value);
    return 0xFF;
  }
  return tmp;
}

/**
  * @brief  Gets numeric values from the Hyperterminal.
  * @param  Maximun value allowed (value)
  * @retval The character received
  */
void Serial_Putchar(char value)
{
#ifndef __TERMINAL_IO__
  /* e.g. read a character from the EVAL_COM1 and Loop until RXNE = 1 */
  HAL_UART_Transmit(&huart, (uint8_t *)&value, 1, HAL_TIMEOUT_VALUE);
#endif
}

/**
  * @brief  Gets numeric values from the Hyperterminal.
  * @param  Maximun value allowed (value)
  * @retval The character received
  */
void Serial_Printf(char *value, int len)
{
#ifndef __TERMINAL_IO__
  /* e.g. read a character from the EVAL_COM1 and Loop until RXNE = 1 */
  HAL_UART_Transmit(&huart, (uint8_t *)value, len, HAL_TIMEOUT_VALUE);
#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
