/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines of the application
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stm32mp13xx_disco.h"
#include "stm32mp13xx_disco_stpmic1.h"
#include "stm32mp13xx_disco_io.h"
#include "string.h"
/* USER CODE END Includes */

/* Private defines -----------------------------------------------------------*/
#define DEFAULT_IRQ_PRIO      1U

/* MDMA channels to CRYP IP */
#define MDMA_Channel_CRYP_IN    MDMA_Channel28
#define MDMA_Channel_CRYP_OUT   MDMA_Channel29

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
