/**
  ******************************************************************************
  * @file    stm32mp13xx_valid_fmc_nand.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32mp13xx_valid_fmc_nand.c driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32MP13XX_VALID_FMC_NAND_H
#define STM32MP13XX_VALID_FMC_NAND_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_valid_errno.h"
#include "stm32mp13xx_valid_conf.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX_VALID
  * @{
  */

/** @addtogroup STM32MP13XX_VALID_FMC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup STM32MP13XX_VALID_NOR_Exported_Constants NOR Exported Constants
  * @{
  */
#define NAND_INSTANCES_NBR  1U

/** @defgroup STM32MP13XX_VALID_FMC_NAND_Exported_Types Exported Types
  * @{
  */

typedef struct
{
  uint16_t Page;   /*!< NAND memory Page address  */

  uint16_t Block;  /*!< NAND memory Block address */

} BSP_NAND_AddressTypeDef_t;



/* Exported constants --------------------------------------------------------*/
/** @defgroup STM32MP13XX_VALID_FMC_NAND_Exported_Constants FMC_NAND Exported Constants
  * @{
  */

/*
 * NAND device parameters. They can be found in the datasheet.
 */

/* Device name: MT29F8G08ABACA */
#define BSP_NAND_MANUFACTURER_CODE  0x2C
#define BSP_NAND_DEVICE_CODE        0xD3
#define BSP_NAND_THIRD_ID           0x90
#define BSP_NAND_FOURTH_ID          0xA6
#define BSP_NAND_BLOCK_NBR          4096   /* Blocks */
#define BSP_NAND_BLOCK_SIZE         64     /* Pages  */
#define BSP_NAND_PLANE_SIZE         2048   /* Blocks */
#define BSP_NAND_PAGE_SIZE          4096   /* Bytes  */
#define BSP_NAND_PLANE_NBR          2      /* Planes */
#define BSP_NAND_SPARE_AREA_SIZE    224    /* Bytes  */

/*
 * NAND ECC algorithm options.
 */

/* ECC algorithm. */
#if defined(BSP_NAND_ECC_ALGO_HAMMING)
#define BSP_NAND_ECC_ALGO            FMC_NAND_ECC_ALGO_HAMMING
#elif defined(BSP_NAND_ECC_ALGO_BCH)
#define BSP_NAND_ECC_ALGO            FMC_NAND_ECC_ALGO_BCH
#else
#error "BSP_NAND_ECC_ALGO_HAMMING or BSP_NAND_ECC_ALGO_BCH not defined"
#endif

/* When using BCH, we can choose between the 4-bit or 8-bit version. */
#if defined(BSP_NAND_ECC_BCH_MODE_BCH4)
#define BSP_NAND_ECC_BCH_MODE        FMC_NAND_BCH_4BIT
#elif defined(BSP_NAND_ECC_BCH_MODE_BCH8)
#define BSP_NAND_ECC_BCH_MODE        FMC_NAND_BCH_8BIT
#elif defined(BSP_NAND_ECC_ALGO_BCH)
#error "BSP_NAND_ECC_BCH_MODE_BCH4 or BSP_NAND_ECC_BCH_MODE_BCH8 not defined when defining BSP_NAND_ECC_ALGO_BCH"
#endif

/* Size of each sector for ECC. When using BCH, it is always 512. */
#if defined(BSP_NAND_ECC_ALGO_BCH)
#if defined(BSP_NAND_ECC_SECTOR_SIZE)
#if BSP_NAND_ECC_SECTOR_SIZE != 512
#error "BSP_NAND_ECC_SECTOR_SIZE can only be 512 when using BSP_NAND_ECC_ALGO_BCH"
#endif
#else
#define BSP_NAND_ECC_SECTOR_SIZE     512
#endif
#else
#if !defined(BSP_NAND_ECC_SECTOR_SIZE)
#error "BSP_NAND_ECC_SECTOR_SIZE not defined when defining BSP_NAND_ECC_ALGO_HAMMING"
#endif
#endif

/* ECC size per sector. With 8-bit accesses: 3 bytes for Hamming with 512-byte sector, 7 for BCH4, 13 for BCH8. */
#if defined(BSP_NAND_ECC_ALGO_HAMMING)
#if BSP_NAND_ECC_SECTOR_SIZE == 256
#define BSP_NAND_ECC_SIZE_PER_SECTOR ((22 + 7) >> 3) /* 22 bits (3 bytes) of ECC */
#elif BSP_NAND_ECC_SECTOR_SIZE == 512
#define BSP_NAND_ECC_SIZE_PER_SECTOR ((24 + 7) >> 3) /* 24 bits (3 bytes) of ECC */
#elif BSP_NAND_ECC_SECTOR_SIZE == 1024
#define BSP_NAND_ECC_SIZE_PER_SECTOR ((26 + 7) >> 3) /* 26 bits (4 bytes) of ECC */
#elif BSP_NAND_ECC_SECTOR_SIZE == 2048
#define BSP_NAND_ECC_SIZE_PER_SECTOR ((28 + 7) >> 3) /* 28 bits (4 bytes) of ECC */
#elif BSP_NAND_ECC_SECTOR_SIZE == 4096
#define BSP_NAND_ECC_SIZE_PER_SECTOR ((30 + 7) >> 3) /* 30 bits (4 bytes) of ECC */
#elif BSP_NAND_ECC_SECTOR_SIZE == 8192
#define BSP_NAND_ECC_SIZE_PER_SECTOR ((32 + 7) >> 3) /* 32 bits (4 bytes) of ECC */
#else
 #error "BSP_NAND_ECC_SECTOR_SIZE value is not supported"
#endif
#else
#if defined(BSP_NAND_ECC_BCH_MODE_BCH4)
#define BSP_NAND_ECC_SIZE_PER_SECTOR 7
#else
#define BSP_NAND_ECC_SIZE_PER_SECTOR 13
#endif
#endif

/*
 * Spare area layout (same as OpenSTLinux)
 */

#define BSP_NAND_BAD_BLOCK_OFFSET   0
#define BSP_NAND_ECC_OFFSET         2
#define BSP_NAND_EXTRA_OFFSET       (BSP_NAND_ECC_OFFSET + BSP_NAND_ECC_SIZE_PER_SECTOR * (BSP_NAND_PAGE_SIZE / BSP_NAND_ECC_SECTOR_SIZE))

/*
 * FMC NAND options
 */

/* Exported functions --------------------------------------------------------*/
/** @defgroup STM32MP13XX_VALID_FMC_NAND_Exported_Functions NAND Exported Functions
  * @{
  */
int32_t BSP_FMC_NAND_Init(uint32_t Instance);
int32_t BSP_FMC_NAND_InitWithSequencer(uint32_t Instance);
int32_t BSP_FMC_NAND_DeInit(uint32_t Instance);
int32_t BSP_FMC_NAND_ReadID(uint32_t Instance, NAND_IDTypeDef *pNAND_ID);
int32_t BSP_FMC_NAND_GetStatus(uint32_t Instance);
int32_t BSP_FMC_NAND_EraseChip(uint32_t Instance);
int32_t BSP_FMC_NAND_Erase_Block(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress);
int32_t BSP_FMC_NAND_Read(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumPageToRead);
int32_t BSP_FMC_NAND_Write(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumPageToWrite);
int32_t BSP_FMC_NAND_Read_SpareArea(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumSpareAreaToRead);
int32_t BSP_FMC_NAND_Write_SpareArea(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumSpareAreaTowrite);
int32_t BSP_FMC_NAND_ReadWithEcc(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumPageToRead, uint32_t *NumPageRead);
int32_t BSP_FMC_NAND_WriteWithEcc(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumPageToWrite, uint32_t *NumPageWritten);
int32_t BSP_FMC_NAND_ReadWithSequencer(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer);
int32_t BSP_FMC_NAND_WriteWithSequencer(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer);
int32_t BSP_FMC_NAND_ReadWithSequencerAndEcc(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer);
int32_t BSP_FMC_NAND_WriteWithSequencerAndEcc(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer);
int32_t BSP_FMC_NAND_WaitSequencerCompletion(uint32_t Instance);
int32_t BSP_FMC_NAND_GetEccStatistics(uint32_t Instance, NAND_EccStatisticsTypeDef *statistics);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */

#ifdef HAL_MDMA_MODULE_ENABLED
HAL_StatusTypeDef MX_NAND_InitWithSequencer(NAND_HandleTypeDef *hNand, MDMA_HandleTypeDef *hDmaReadData, MDMA_HandleTypeDef *hDmaWriteData, MDMA_HandleTypeDef *hDmaReadEcc, uint32_t *eccBuffer);
#endif /* HAL_MDMA_MODULE_ENABLED  */

HAL_StatusTypeDef MX_NAND_Init(NAND_HandleTypeDef *hNand);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32MP13XX_VALID_FMC_NAND_H */
