/**
  ******************************************************************************
  * @file    platform.h
  * @author  MCD Application Team
  * @brief   Header for patterns
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
#ifndef PLATFORM_H
#define PLATFORM_H

/* Includes ------------------------------------------------------------------*/
#if defined (STM32MP257Cxx)
#include "stm32mp2xx_hal.h"
#include "stm32mp2xx_ll_usart.h"
#include "stm32mp2xx_ll_bsec.h"
#include "stm32mp2xx_hal_conf.h"
#elif defined (STM32MP157Cxx)
#include "stm32mp1xx_hal.h"
#include "stm32mp1xx_ll_usart.h"
#include "stm32mp1xx_hal_conf.h"
#else
#include "stm32mp13xx_hal.h"
#include "stm32mp13xx_ll_usart.h"
#include "stm32mp13xx_hal_conf.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* PLATFORM_H */
