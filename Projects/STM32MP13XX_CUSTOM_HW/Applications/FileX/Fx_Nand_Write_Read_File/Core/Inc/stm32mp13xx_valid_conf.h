/** 
  ******************************************************************************
  * @file    stm32mp13xx_valid_conf.h
  * @author  MCD Application Team
  * @brief   configuration file.
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
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32MP13XX_VALID_CONFIG_H
#define __STM32MP13XX_VALID_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32mp13xx_hal.h"


/* Activation of Critical Section */
#define USE_BSP_CRITICAL_SECTION           0U

/* Activation of PMIC */
#define USE_STPMIC1x                        0U

/* Usage of COM feature */
#define USE_BSP_COM_FEATURE                 1U
/* Activation of COM port for log output */
#define USE_COM_LOG                         1U

#if (USE_BSP_CRITICAL_SECTION == 1)
#include "lock_resource.h"
#define BSP_ENTER_CRITICAL_SECTION       PERIPH_LOCK
#define BSP_EXIT_CRITICAL_SECTION        PERIPH_UNLOCK
#else
#define BSP_ENTER_CRITICAL_SECTION(periph)       ((void)0)
#define BSP_EXIT_CRITICAL_SECTION(periph)        ((void)0)
#endif

/* IRQ priorities */
#define BSP_PMU1_IT_PRIORITY                   0x03UL
#define BSP_BUTTON_WAKEUP_IT_PRIORITY          0x0FUL
#define BSP_BUTTON_USER_IT_PRIORITY            0x0FUL
#define BSP_BUTTON_USER2_IT_PRIORITY           0x0FUL

/*
 * Algorithm used for NAND ECC
 * Only one define must be uncommented
 */
/* If defined, Hamming algorithm is used for NAND ECC */
/* #define BSP_NAND_ECC_ALGO_HAMMING */
/* If defined, BCH algorithm is used for NAND ECC */
#define BSP_NAND_ECC_ALGO_BCH

/*
 * BCH algorithm used for NAND ECC
 * It is only effective if BSP_NAND_ECC_ALGO_BCH is defined
 * Only one define must be uncommented
 */
/* #define BSP_NAND_ECC_BCH_MODE_BCH4 */
#define BSP_NAND_ECC_BCH_MODE_BCH8

/*
 * Sector size used to compute the NAND ECC.
 * It must only be defined if BSP_NAND_ECC_ALGO_HAMMING is defined too, because BCH algorithm uses only 512-byte sectors
 */
/* #define BSP_NAND_ECC_SECTOR_SIZE */

#ifdef __cplusplus
}
#endif

#endif /* __STM32MP13XX_VALID_CONFIG_H */

