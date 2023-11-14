/**
  ******************************************************************************
  * @file    loader_XSPI_NOR.h
  * @author  MCD Application Team
  * @brief   Header file of loader_XSPI_NOR.c
  ******************************************************************************
  *
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOADER_XSPI_NOR_H
#define __LOADER_XSPI_NOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_hal.h"
#if defined (USE_STM32MP13XX_CUSTOM_HW)
#include "stm32mp13xx_valid_xspi.h"
#endif

#define TIMEOUT 5000U

/* Private function prototypes -----------------------------------------------*/
int Init_XSPI ();
int Write_XSPI(uint8_t* pData, uint32_t Address, uint32_t Size);
int Read_XSPI(uint8_t* pData, uint32_t Address, uint32_t Size);
int SectorErase_XSPI(uint32_t EraseStartAddress, uint32_t EraseEndAddress);
int MassErase_XSPI(void);

#endif /* __LOADER_XSPI_NOR_H */

