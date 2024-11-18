/**
  ******************************************************************************
  * @file    openbl_util.h
  * @author  MCD Application Team
  * @brief   Header for openbl_util.c module
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
#ifndef OPENBL_UTIL_H
#define OPENBL_UTIL_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "string.h"
#include "stdlib.h"
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  char*     opt[0xF];
  uint32_t  id[0xF];
  char*     name[0xF];
  char*     type[0xF];
  char*     ip[0xF];
  uint32_t  offset[0xF];
  uint32_t  partsize;
} OPENBL_Flashlayout_TypeDef;

typedef struct
{
  uint32_t  id;
  char     name[40];
  uint32_t  offset;
} OPENBL_Flashlayout_Storage_TypeDef;

/* Exported constants --------------------------------------------------------*/
#define PHASE_FLASHLAYOUT                    0x00                /* Flashlayout phase */
#define PHASE_0x1                            0x01                /* Phase ID 0x1  */
#define PHASE_0x3                            0x03                /* Phase ID 0x3 */
#define PHASE_0x4                            0x04                /* Phase ID 0x4 */
#define PHASE_0x5                            0x05                /* Phase ID 0x5 */
#define PHASE_END                            0xFE                /* End phasse */
#define PHASE_FIRST_USER                     0x01                /* Phase first user */
#define PHASE_LAST_USER                      0x0F                /* Phase last user */
#define PARSE_ERROR                          -1
#define PARSE_OK                             0
#define GETPHASE_SIZE                        9
#define PHASE_CMD                            0xF1

#define BOOT_INTERFACE_SEL_SERIAL_UART       0x5U                /* Boot occurred on UART */
#define BOOT_INTERFACE_SEL_SERIAL_USB        0x6U                /* Boot occurred on USB */
#define UART_ID                              0                   /* USB registered as interface ID 0 */
#define USB_ID                               1                   /* USB registered as interface ID 1 */
#define UNDEF_ID                             -2
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int parse_flash_layout(uint32_t addr, uint32_t size);
int parse_name(char *s, uint32_t idx);
int parse_ip(char *s, uint32_t idx);
int parse_id(char *s, uint32_t idx);
int parse_offset(char *s, uint32_t idx);
int parse_type(char *s, uint32_t idx);
int parse_option(char *s, uint32_t idx);
int parse_boot_interface_selected(uint32_t addr);
#endif /* OPENBL_UTIL_H */
