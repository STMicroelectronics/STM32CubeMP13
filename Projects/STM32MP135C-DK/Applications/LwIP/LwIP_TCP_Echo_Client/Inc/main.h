/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Private includes ----------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32mp13xx_disco.h"
#include "stm32mp13xx_disco_stpmic1.h"
#include "stm32mp13xx_disco_io.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* TCP remote connection port */
#define DEST_PORT    7

/* Static DEST IP ADDRESS: DEST_IP_ADDR0.DEST_IP_ADDR1.DEST_IP_ADDR2.DEST_IP_ADDR3
 *  of the Server connected to the MPU*/
#define DEST_IP_ADDR0   ((uint8_t)10U)
#define DEST_IP_ADDR1   ((uint8_t)199U)
#define DEST_IP_ADDR2   ((uint8_t)170U)
#define DEST_IP_ADDR3   ((uint8_t)75U)

/* Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 of the TCP Client which
 * is connected to the Server i.e. MPU connected to the server */
#define IP_ADDR0   			((uint8_t) 10U)
#define IP_ADDR1   			((uint8_t) 199U)
#define IP_ADDR2   			((uint8_t) 86U)
#define IP_ADDR3   			((uint8_t) 143U)

/*NETMASK*/
#define NETMASK_ADDR0   ((uint8_t) 255U)
#define NETMASK_ADDR1   ((uint8_t) 255U)
#define NETMASK_ADDR2   ((uint8_t) 240U)
#define NETMASK_ADDR3   ((uint8_t) 0U)

/*Gateway Address*/
#define GW_ADDR0   			((uint8_t) 10U)
#define GW_ADDR1   			((uint8_t) 199U)
#define GW_ADDR2   			((uint8_t) 175U)
#define GW_ADDR3   			((uint8_t) 254U)

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

