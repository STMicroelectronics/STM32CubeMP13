/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    : USB_Host/CDC_Standalone/USB_Host/Target/usbh_conf.h
  * @author  : MCD Application Team
  * @brief   : General low level driver configuration
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
#ifndef __USBH_CONF__H__
#define __USBH_CONF__H__
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Exported types ------------------------------------------------------------*/

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_HOST_LIBRARY
  * @{
  */

/** @defgroup USBH_CONF
  * @brief usb host low level driver configuration file
  * @{
  */

/** @defgroup USBH_CONF_Exported_Variables USBH_CONF_Exported_Variables
  * @brief Public variables.
  * @{
  */

/**
  * @}
  */
/** @defgroup USBH_CONF_Exported_Defines USBH_CONF_Exported_Defines
  * @brief Defines for configuration of the Usb host.
  * @{
  */

/*----------   -----------*/
#define USBH_MAX_NUM_ENDPOINTS      2U

/*----------   -----------*/
#define USBH_MAX_NUM_INTERFACES      2U

/*----------   -----------*/
#define USBH_MAX_NUM_CONFIGURATION      1U

/*----------   -----------*/
#define USBH_MAX_NUM_SUPPORTED_CLASS          1U

/*----------   -----------*/
#define USBH_KEEP_CFG_DESCRIPTOR              1U

/*----------   -----------*/
#define USBH_MAX_SIZE_CONFIGURATION           0x200

/*----------   -----------*/
#define USBH_MAX_DATA_BUFFER                  0x200

/*----------   -----------*/
#define USBH_DEBUG_LEVEL                      2U

/*----------   -----------*/
#define USBH_USE_OS                           0U
/****************************************/
/* #define for FS and HS identification */
#define HOST_HS 		0

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* CMSIS OS macros */
#if (USBH_USE_OS == 1)
  #include "cmsis_os.h"
  #define   USBH_PROCESS_PRIO    osPriorityNormal
#endif

/**
  * @}
  */

/** @defgroup USBH_CONF_Exported_Macros USBH_CONF_Exported_Macros
  * @brief Aliases.
  * @{
  */
/* Memory management macros */

/** Alias for memory allocation. */
#define USBH_malloc         malloc

/** Alias for memory release. */
#define USBH_free           free

/** Alias for memory set. */
#define USBH_memset         memset

/** Alias for memory copy. */
#define USBH_memcpy         memcpy

/* DEBUG macros */
#if (USBH_DEBUG_LEVEL > 0)
#define USBH_UsrLog(...)   printf(__VA_ARGS__);\
                           printf("\n");
#else
#define USBH_UsrLog(...)
#endif


#if (USBH_DEBUG_LEVEL > 1)

#define USBH_ErrLog(...)   printf("ERROR: ") ;\
                           printf(__VA_ARGS__);\
                           printf("\n");
#else
#define USBH_ErrLog(...)
#endif

#if (USBH_DEBUG_LEVEL > 2)
#define USBH_DbgLog(...)   printf("DEBUG : ") ;\
                           printf(__VA_ARGS__);\
                           printf("\n");
#else
#define USBH_DbgLog(...)
#endif

/**
  * @}
  */

/** @defgroup USBH_CONF_Exported_Types USBH_CONF_Exported_Types
  * @brief Types.
  * @{
  */

/**
  * @}
  */

/** @defgroup USBH_CONF_Exported_FunctionsPrototype USBH_CONF_Exported_FunctionsPrototype
  * @brief Declaration of public functions for Usb host.
  * @{
  */

/* Exported functions -------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_CONF__H__ */

