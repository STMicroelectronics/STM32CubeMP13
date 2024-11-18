/**
  ******************************************************************************
  * @file    openbl_mem.h
  * @author  MCD Application Team
  * @brief   Header for openbl_mem.c module
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
#ifndef OPENBL_MEM_H
#define OPENBL_MEM_H

/* Includes ------------------------------------------------------------------*/
#include "openbootloader_conf.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t StartAddress;
  uint32_t EndAddress;
  uint32_t Size;
  uint32_t Type;
  int32_t (*Init)(uint32_t Address);
  uint8_t (*Read)(uint32_t Address);
  void (*Write)(uint32_t Address, uint8_t *Data, uint32_t DataLength);
  void (*JumpToAddress)(uint32_t Address);
  void (*MassErase)(uint32_t Address);
  void (*SectorErase)(uint32_t EraseStartAddress, uint32_t EraseEndAddress);
  uint64_t (*Verify)(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t missalignement);
  void (*configure)(void *);
} OPENBL_MemoryTypeDef;

typedef enum {
	CONFIGURE_ENABLE_BOOT1_PARTITION = 1,
	CONFIGURE_DISABLE_BOOT1_PARTITION = 2,
} conf_boot_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint8_t prog_USB;
/* Exported functions ------------------------------------------------------- */
void OPENBL_MEM_JumpToAddress(uint32_t Address);
void OPENBL_MEM_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength);

int32_t OPENBL_MEM_Init(uint32_t Address);
void OPENBL_MEM_Configure(uint32_t Address, void *ConfPtr);
uint8_t OPENBL_MEM_Read(uint32_t Address, uint32_t MemoryIndex);
uint32_t OPENBL_MEM_GetAddressArea(uint32_t Address);
uint32_t OPENBL_MEM_GetMemoryIndex(uint32_t Address);
uint8_t OPENBL_MEM_CheckJumpAddress(uint32_t Address);
uint64_t OPENBL_MEM_Verify(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t missalignement);
void OPENBL_MEM_MassErase(uint32_t Address);
void OPENBL_MEM_SectorErase(uint32_t Address, uint32_t EraseStartAddress, uint32_t EraseEndAddress);

ErrorStatus OPENBL_MEM_RegisterMemory(OPENBL_MemoryTypeDef *Memory);

#endif /* OPENBL_MEM_H */
