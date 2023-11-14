/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32mp13xx_hal.h"
#include "stm32mp13xx_ll_etzpc.h"
#include "loader_XSPI_NOR.h"
#if defined (USE_STM32MP13XX_CUSTOM_HW)
#include "stm32mp13xx_valid.h"
#include "stm32mp13xx_valid_xspi.h"
#endif


/* External functions --------------------------------------------------------*/

/* For semihosting operations */
extern void initialise_monitor_handles(void);

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#endif /* __MAIN_H */

