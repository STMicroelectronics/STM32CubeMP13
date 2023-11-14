/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#ifndef LX_STM32_NAND_DRIVER_H
#define LX_STM32_NAND_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"
#include "stm32mp13xx_valid_fmc_nand.h"

/* Define constants for the NAND flash. */

/* NAND structure: */
#define TOTAL_BLOCKS                        BSP_NAND_BLOCK_NBR
#define PHYSICAL_PAGES_PER_BLOCK            BSP_NAND_BLOCK_SIZE /* Min value of 2 */
#define BYTES_PER_PHYSICAL_PAGE             BSP_NAND_PAGE_SIZE /* Bytes per page */
#define WORDS_PER_PHYSICAL_PAGE             (BYTES_PER_PHYSICAL_PAGE/4) /* Words per page */
#define SPARE_BYTES_PER_PAGE                BSP_NAND_SPARE_AREA_SIZE /* Spare bytes per page */

/* Spare area layout (OpenSTLinux compatible): */
#define BAD_BLOCK_POSITION                  BSP_NAND_BAD_BLOCK_OFFSET /* Offset of bad block */
#define EXTRA_BYTE_POSITION                 ((BSP_NAND_EXTRA_OFFSET + 3) & ~0x3) /* Offset of extra bytes, needs to be 4-byte aligned otherwise an abort exception is triggered */

#define SPARE_DATA1_OFFSET                  (EXTRA_BYTE_POSITION + 0)
#define SPARE_DATA1_LENGTH                  4
#define SPARE_DATA2_OFFSET                  (SPARE_DATA1_OFFSET + SPARE_DATA1_LENGTH)
#define SPARE_DATA2_LENGTH                  2

#define NAND_INSTANCE                       0  
#define NAND_FLASH_SIZE                     (TOTAL_BLOCKS * PHYSICAL_PAGES_PER_BLOCK * BYTES_PER_PHYSICAL_PAGE)

extern ULONG fx_lx_nand_driver_buffer[(7 * TOTAL_BLOCKS + 4 + 2 * (BYTES_PER_PHYSICAL_PAGE + SPARE_BYTES_PER_PAGE)) / sizeof(ULONG)];

UINT  _lx_nand_flash_initialize_driver(LX_NAND_FLASH *nand_flash);

#ifdef __cplusplus
}
#endif

#endif /* LX_STM32_NAND_DRIVER_H */
