/**
  ******************************************************************************
  * @file    openbl_core.h
  * @author  MCD Application Team
  * @brief   Header for openbl_core.c module
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
#ifndef OPENBL_CORE_H
#define OPENBL_CORE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "openbootloader_conf.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void (*Init)(void);
  void (*DeInit)(void);
  uint8_t (*Detection)(void);
  uint8_t (*GetCommandOpcode)(void);
  void (*SendByte)(uint8_t Byte);
} OPENBL_OpsTypeDef;

typedef struct
{
  void (*GetCommand)(void);
  void (*GetVersion)(void);
  void (*GetID)(void);
  void (*GetPhase)(void);
  void (*ReadMemory)(void);
  void (*Download)(void);
  void (*ReadPartition)(void);
  void (*Start)(void);
} OPENBL_CommandsTypeDef;

typedef struct
{
  OPENBL_OpsTypeDef *p_Ops;
  OPENBL_CommandsTypeDef *p_Cmd;
} OPENBL_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/
#define ERROR_COMMAND                     0xECU             /* Error command */
#define ACK_BYTE                          0x79U             /* Acknowledge Byte ID */
#define NACK_BYTE                         0x1FU             /* No Acknowledge Byte ID */
#define ABORT_BYTE                        0x5FU             /* Abort byte */
#define BUSY_BYTE                         0x76U             /* Busy Byte */
#define SYNC_BYTE                         0xA5U             /* synchronization byte */

/* ---------------------- Open Bootloader Commands ---------------------------*/
#define CMD_GET_COMMAND                   0x00U             /* Get commands command */
#define CMD_GET_VERSION                   0x01U             /* Get Version command */
#define CMD_GET_ID                        0x02U             /* Get ID command */
#define CMD_GET_PHASE                     0x03U             /* Get Phase command */
#define CMD_READ_MEMORY                   0x11U             /* Read Memory command */
#define CMD_READ_PARTITION                0x12U             /* Read Partition command */
#define CMD_DOWNLOAD                      0x31U             /* download command */
#define CMD_START                         0x21U             /* Start command */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void OPENBL_Init(void);
void OPENBL_DeInit(void);
uint32_t OPENBL_InterfaceDetection(void);
void OPENBL_CommandProcess(void);
ErrorStatus OPENBL_RegisterInterface(OPENBL_HandleTypeDef *Interface);

#endif /* OPENBL_CORE_H */
