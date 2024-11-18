/**
  ******************************************************************************
  * @file    app_openbootloader.c
  * @author  MCD Application Team
  * @brief   OpenBootloader application entry point
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
#include "main.h"
#include "app_openbootloader.h"
#include "usart_interface.h"
#include "ram_interface.h"
#include "iwdg_interface.h"
#include "external_memory_interface.h"
#include "openbl_usart_cmd.h"

#include "openbl_core.h"
#include "usb_interface.h"
#include "openbl_usb_cmd.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OPENBL_HandleTypeDef USART_Handle;
static OPENBL_OpsTypeDef USART_Ops =
{
  OPENBL_USART_Configuration,
  NULL,
  OPENBL_USART_ProtocolDetection,
  OPENBL_USART_GetCommandOpcode,
  OPENBL_USART_SendByte
};


static OPENBL_HandleTypeDef USB_Handle;
static OPENBL_OpsTypeDef USB_Ops =
{
  OPENBL_USB_Configuration,
  NULL,
  OPENBL_USB_ProtocolDetection,
  NULL,
  NULL
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize open Bootloader.
  * @param  None.
  * @retval None.
  */
void OpenBootloader_Init(void)
{

#if defined (__CP_SERIAL_BOOT__)

  /**********************************************************/
  /* SERIAL BOOT : Register only interface used by ROM CODE */
  /**********************************************************/

  int itx = parse_boot_interface_selected(BOOT_SEL_ITF_ADDRESS);
  if (itx == UART_ID)
  {
     /* Register USART interfaces */
	 USART_Handle.p_Ops = &USART_Ops;
	 USART_Handle.p_Cmd = OPENBL_USART_GetCommandsList();
	 OPENBL_RegisterInterface(&USART_Handle);
  }

  else if (itx == USB_ID)
  {
	 /* Register USB interfaces */
	 USB_Handle.p_Ops = &USB_Ops;
	 OPENBL_RegisterInterface(&USB_Handle);
  }


#else
  /***************************************************/
  /* NOT Serial Boot Mode : Register all interfaces  */
  /***************************************************/

  /* Register USART interfaces */
  USART_Handle.p_Ops = &USART_Ops;
  USART_Handle.p_Cmd = OPENBL_USART_GetCommandsList();
  OPENBL_RegisterInterface(&USART_Handle);


  /* Register USB interfaces */
  USB_Handle.p_Ops = &USB_Ops;
  OPENBL_RegisterInterface(&USB_Handle);
#endif

  /* Initialize interfaces */
  OPENBL_Init();

  /* Initialize memories */
  OPENBL_MEM_RegisterMemory(&RAM_Descriptor);
  OPENBL_MEM_RegisterMemory(&EXTERNAL_MEMORY_Descriptor);
}

/**
  * @brief  DeInitialize open Bootloader.
  * @param  None.
  * @retval None.
  */
void OpenBootloader_DeInit(void)
{
}

/**
  * @brief  This function is used to select which protocol will be used when communicating with the host.
  * @param  None.
  * @retval None.
  */
void OpenBootloader_ProtocolDetection(void)
{
  static uint32_t interface_detected = 0;

  if (interface_detected == 0)
  {
    interface_detected = OPENBL_InterfaceDetection();
  }

  if (interface_detected == 1)
  {
    OPENBL_CommandProcess();
  }
}



