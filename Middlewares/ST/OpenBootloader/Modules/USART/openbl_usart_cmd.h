/**
  ******************************************************************************
  * @file    openbl_usart_cmd.h
  * @author  MCD Application Team
  * @brief   Header for openbl_usart_cmd.c module
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
#ifndef OPENBL_USART_CMD_H
#define OPENBL_USART_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"
#include "openbl_util.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_USART_VERSION                 0x40U               /* Open Bootloader USART protocol */
#define DEFAULT_ADDRESS                      0x70000000          /* External Flash Adress or NVM */
#define GPT_OFFSET     		                 0x30000             /* External Flash Adress offset for GPT */


/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern OPENBL_CommandsTypeDef OPENBL_USART_Commands;

/* Exported functions ------------------------------------------------------- */
OPENBL_CommandsTypeDef *OPENBL_USART_GetCommandsList(void);
#endif /* OPENBL_USART_CMD_H */
