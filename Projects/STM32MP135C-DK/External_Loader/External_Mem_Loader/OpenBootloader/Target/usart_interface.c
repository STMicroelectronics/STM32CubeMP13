/**
  ******************************************************************************
  * @file    usart_interface.c
  * @author  MCD Application Team
  * @brief   Contains USART HW configuration
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

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "openbl_core.h"
#include "openbl_usart_cmd.h"
#include "usart_interface.h"
#include "interfaces_conf.h"
#include "app_openbootloader.h"
#include "external_memory_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_USART_Init(void);
static void OPENBL_USART_DeInit(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  This function is used to initialize the used USART instance.
 * @retval None.
 */
static void OPENBL_USART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct;

  USART_InitStruct.PrescalerValue      = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate            = 115200U;
  USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_9B;
  USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity              = LL_USART_PARITY_EVEN;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;

  if (IS_USART_AUTOBAUDRATE_DETECTION_INSTANCE(USARTx))
  {
    /* Configure auto-baudrate detection */
    LL_USART_EnableAutoBaudRate(USARTx);
    LL_USART_SetAutoBaudRateMode(USARTx, LL_USART_AUTOBAUD_DETECT_ON_7F_FRAME);
  }

  /* Init the usart */
  LL_USART_Init(USARTx, &USART_InitStruct);

  /* Enable fifo */
  LL_USART_EnableFIFO(USARTx);

  /* Set TX fifo threshold */
  LL_USART_SetTXFIFOThreshold(USARTx, LL_USART_FIFOTHRESHOLD_1_8);

  /* Set RX fifo threshold */
  LL_USART_SetRXFIFOThreshold(USARTx, LL_USART_FIFOTHRESHOLD_1_8);

  /* Enable the usart */
  LL_USART_Enable(USARTx);
}

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  This function is used to configure USART pins and then initialize the used USART instance.
 * @retval None.
 */
void OPENBL_USART_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
#if defined(__CP_SERIAL_BOOT__)
  uint32_t Tickstart;
  uint32_t Timeout = 200U;
  uint8_t  SendNack = 0;
#endif
  /* Enable all resources clocks --------------------------------------------*/
  /* Enable used GPIOx clocks */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Enable USART clock */
  USARTx_CLK_ENABLE();

  /* USARTx pins configuration -----------------------------------------------*/
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_ALTERNATE;
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_ALTERNATE;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /* Init the usart */
  OPENBL_USART_DeInit();
  OPENBL_USART_Init();

#if defined(__CP_SERIAL_BOOT__)
  /*Serial Boot: To avoid sync issue during ROM Code -> STM32PRGFW-UTIL transition : empty fifo and sends NACK*/
  /* Init tickstart for timeout management */
  Tickstart = HAL_GetTick();
  while ((HAL_GetTick() - Tickstart) < Timeout)
  {
     if (LL_USART_IsActiveFlag_RXNE_RXFNE(USARTx))
     {
       LL_USART_ReceiveData8(USARTx);
       SendNack =1;
     }
  }

  if (SendNack)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
#endif

}

/**
 * @brief  This function is used to De-initialize the USART pins and instance.
 * @retval None.
 */
static void OPENBL_USART_DeInit(void)
{
  /* De-init the usart */
  LL_USART_DeInit(USARTx);
  /* Enable the usart */
  LL_USART_Disable(USARTx);
}

/**
 * @brief  This function is used to detect if there is any activity on USART protocol.
 * @retval Returns 1 if interface is detected else 0.
 */
uint8_t OPENBL_USART_ProtocolDetection(void)
{
  uint8_t detected = 0;

  /* Check if the USARTx is addressed */
  if (((USARTx->ISR & LL_USART_ISR_ABRF) != 0) && ((USARTx->ISR & LL_USART_ISR_ABRE) == 0))
  {
    /* Read byte in order to flush the 0x7F synchronization byte */
    OPENBL_USART_ReadByte();

    /* Acknowledge the host */
    OPENBL_USART_SendByte(ACK_BYTE);

    detected = 1;
  }
  else
  {
    detected = 0;
  }

  return detected;
}

/**
 * @brief  This function is used to get the command opcode from the host.
 * @retval Returns the command.
 */
uint8_t OPENBL_USART_GetCommandOpcode(void)
{
  uint8_t command_opc = 0x0;

  /* Get the command opcode */
  command_opc = OPENBL_USART_ReadByte();

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_USART_ReadByte()) != 0xFF)
  {
    command_opc = ERROR_COMMAND;
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from USART pipe.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_USART_ReadByte(void)
{
  while (!LL_USART_IsActiveFlag_RXNE_RXFNE(USARTx))
  {
  }

  return LL_USART_ReceiveData8(USARTx);
}

/**
  * @brief  This function is used to read one word from USART pipe.
  * @retval Returns the read word.
  */
uint32_t OPENBL_USART_ReadWord(void)
{
  uint32_t word = 0;
  uint8_t tmp = 0;

  tmp = OPENBL_USART_ReadByte();
  word += (tmp << 24);
  tmp = OPENBL_USART_ReadByte();
  word += (tmp << 16);
  tmp = OPENBL_USART_ReadByte();
  word += (tmp << 8);
  tmp = OPENBL_USART_ReadByte();
  word += tmp;

  return word;
}

/**
  * @brief  This function is used to send one byte through USART pipe.
  * @param  Byte The byte to be sent.
  * @retval None.
  */
void OPENBL_USART_SendByte(uint8_t Byte)
{
  LL_USART_TransmitData8(USARTx, (Byte & 0xFF));

  while (!LL_USART_IsActiveFlag_TC(USARTx))
  {
  }
}

/**
  * @brief  This function is used to send one word through USART pipe.
  * @param  Word The word to be sent.
  * @retval None.
  */
void OPENBL_USART_SendWord(uint32_t Word)
{
  OPENBL_USART_SendByte(Word);
  OPENBL_USART_SendByte(Word >> 8);
  OPENBL_USART_SendByte(Word >> 16);
  OPENBL_USART_SendByte(Word >> 24);
}
