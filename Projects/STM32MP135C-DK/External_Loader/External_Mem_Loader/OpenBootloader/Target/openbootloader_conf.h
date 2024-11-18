/**
  ******************************************************************************
  * @file    openbootloader_conf.h
  * @author  MCD Application Team
  * @brief   Contains Open Bootloader configuration
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
#ifndef OPENBOOTLOADER_CONF_H
#define OPENBOOTLOADER_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "platform.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* -------------------------------- Device ID ------------------------------- */
#define DEVICE_ID_MSB                     0x05U  /* MSB byte of device ID */
#if defined (STM32MP157Cxx)
#define DEVICE_ID_LSB                     0x00U  /* LSB byte of device ID */
#elif defined (STM32MP135Fxx)
#define DEVICE_ID_LSB                     0x01U  /* LSB byte of device ID */
#elif defined (STM32MP257Cxx)
#define DEVICE_ID_LSB                     0x05U  /* LSB byte of device ID */
#endif

/* -------------------------- Definitions for Memories ---------------------- */
#if defined (STM32MP157Cxx)
#define RAM_SIZE                          (704U * 1024U)                       /* Size of RAM 704 KB for OpenBL application use  */
#define RAM_START_ADDRESS                 0x2FFC0000                           /* Start of SRAM  */
#define BOOT_SEL_ITF_ADDRESS              0x2FFC0078                           /* Boot selected interface Address */
#define OPENBL_RAM_SIZE                   (89U * 1024U)                        /* Start downloading after this project memory space used */
#elif defined (STM32MP135Fxx)
#define RAM_SIZE                          (160U * 1024U)                       /* Size of RAM 160 KB for OpenBL application use  */
#define RAM_START_ADDRESS                 0x2FFE0000                           /* Start of SRAM  */
#define BOOT_SEL_ITF_ADDRESS              0x30004078                           /* Boot selected interface Address */
#if (defined SNOR_FLASH) || (defined SDCARD_FLASH)
#define OPENBL_RAM_SIZE                   (96U * 1024U)                        /* Start downloading after this project memory space used : 96KB reserved for OpenBL binary */
#else
#define OPENBL_RAM_SIZE                   (89U * 1024U)                        /* Start downloading after this project memory space used */
#endif
#elif defined (STM32MP257Cxx)
#define RAM_START_ADDRESS                 0x20006000                           /* Start of RAM for OpenBL application use */
#define RAM_SIZE                          (232U * 1024U)                       /* Size of RAM */
#define BOOT_SEL_ITF_ADDRESS              0x20000044                           /* Boot selected interface Address */
#define OPENBL_RAM_SIZE                   (160U * 1024U)                        /* Start downloading after this project memory space used */
#endif

#define RAM_END_ADDRESS                   (RAM_START_ADDRESS + RAM_SIZE)       /* End of SRAM  */
#define BASE_WRITE_ADDRESS                RAM_START_ADDRESS + OPENBL_RAM_SIZE  /* Start downloading here */
#define FLASHLAYOUT_RAM_ADDRESS           BASE_WRITE_ADDRESS                   /* Space reserved for flashlayout */
#if (defined SNOR_FLASH) || (defined SDCARD_FLASH)
#define RAM_WRITE_ADDRESS                 RAM_START_ADDRESS + OPENBL_RAM_SIZE
#else
#define RAM_WRITE_ADDRESS                 BASE_WRITE_ADDRESS + 1024            /* Free space for application downloading */
#endif
#define SD_WRITE_ADDRESS                  BASE_WRITE_ADDRESS + 1024            /* Fake address, SD not supported */
#define UNDEF_ADDRESS                     0xFFFFFFFF                           /* Undefined Address */

#define AREA_ERROR                        0x0U  /* Error Address Area */
#define RAM_AREA                          0x1U  /* RAM Address area */
#define EXTERNAL_MEMORY_AREA              0x2U  /* External memory Address area */

#if defined(__CP_SERIAL_BOOT__)
/* only 1 interface registered/detected at runtime, the one used by ROM CODE*/
#define INTERFACES_SUPPORTED              1U
#else
#define INTERFACES_SUPPORTED              2U
#endif
/* -------------------------- Definitions for External Memories ---------------------- */
#if defined SNOR_FLASH
#define EXT_MEMORY_START_ADDRESS          0x70000000 /* (uint32_t)pStorageInfo->DeviceStartAddress */
#define EXT_MEMORY_END_ADDRESS            0x74000000 /* (uint32_t)(pStorageInfo->DeviceStartAddress + pStorageInfo->DeviceSize) */
#define EXT_MEMORY_SIZE                   0x04000000 /* (uint32_t)pStorageInfo->DeviceSize */

#define SECTOR_SIZE                       0x1000     /* (pStorageInfo->sectors[0].SectorSize) */
#define SECTOR_MAX_NUMBER                 0x4000     /* (pStorageInfo->sectors[0].SectorNum) */
#elif defined SDCARD_FLASH
#if defined EMMC_FLASH
#define EXT_MEMORY_START_ADDRESS          0x00       /* 0 LBA */
#else
#define EXT_MEMORY_START_ADDRESS          0x00       /* 128 LBA */
#endif
#define EXT_MEMORY_END_ADDRESS            0x5000     /* 20480 LBA, i.e = 10MB of data starting from 128 LBA, SDCard Address will be 20480 X 512 = 0xA00000 */
#define EXT_MEMORY_SIZE                   0x01000000 /* (uint32_t)pStorageInfo->DeviceSize */
#define SECTOR_SIZE                       0x200      /* Block Size = 512 Bytes */
#define SECTOR_MAX_NUMBER                 0x4000     /* (pStorageInfo->sectors[0].SectorNum) */

#endif

/* Exported macro ------------------------------------------------------------*/
#define BLOCKSIZE_SD                      ((uint32_t)512U) /* SDCard Block size is 512 bytes */

/* Exported functions ------------------------------------------------------- */

#endif /* OPENBOOTLOADER_CONF_H */
