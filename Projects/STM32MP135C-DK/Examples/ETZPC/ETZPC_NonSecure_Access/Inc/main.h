/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
//#include "lock_resource.h"
#include "stm32mp13xx_hal.h"

/* Private includes ----------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32mp13xx_disco.h"
#include "stm32mp13xx_disco_stpmic1.h"
#include "stm32mp13xx_disco_io.h"

#include <stdbool.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

extern char SYSRAM_SECURE;
extern char SYSRAM_UNSECURE;
extern char SRAM1_READ_WRITE_SECURE;
extern char SRAM2_READ_UNSECURE_WRITE_SECURE;
extern char SRAM3_READ_WRITE_UNSECURE;

#define MEMORY_BLOCK_SIZE 4096 /* Size of the memory blocks used for each transfer */
#define SRC_SECURE_CHAR 'S' /* Character used to fill memory of a secure source */
#define SRC_UNSECURE_CHAR 'U' /* Character used to fill memory of an unsecure source */
#define DST_SECURE_CHAR 's' /* Character used to fill memory of a secure destination */
#define DST_UNSECURE_CHAR 'u' /* Character used to fill memory of an unsecure destination */

extern DMA_HandleTypeDef hdma;

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

