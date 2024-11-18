/**
  ******************************************************************************
  * @file    Dev_Inf.c
  * @author  MCD Application Team
  * @brief   This file defines the structure containing informations about the 
  *          external flash memory MX25LM51245G used by STM32CubeProgramer in 
  *          programming/erasing the device.
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
/*
 * Dev_Inf.c
 *
 */
#include "Dev_Inf.h"
#include "main.h"

/* This structure contains information used by ST-LINK Utility to program and erase the device */
#if defined (__ICCARM__)
__root struct StorageInfo const StorageInfo  =  {
#else
struct StorageInfo const StorageInfo = {
#endif
  "XSPI_flashloader_MP13",             // Device Name + version number
  NOR_FLASH,                           // Device Type
  0x70000000,                          // Device Start Address
  MX25L512_FLASH_SIZE,                 // Device Size in Bytes
  MX25L512_PAGE_SIZE,                  // Programming Page Size
  0xFF,                                // Initial Content of Erased Memory

  // Specify Size and Address of Sectors (view example below)
  { {
      (MX25L512_FLASH_SIZE / MX25L512_SECTOR_SIZE),  // Sector Numbers,
      (uint32_t) MX25L512_FLASH_SIZE
    },  //Sector Size

    { 0x00000000, 0x00000000 }
  }
};

