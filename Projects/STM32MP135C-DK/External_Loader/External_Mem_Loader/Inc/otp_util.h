/**
  ******************************************************************************
  * @file    otp_util.h
  * @author  MCD Application Team
  * @brief   Header for otp_util.c module
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
#ifndef OTP_UTIL_H
#define OTP_UTIL_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t Version;
  uint32_t GlobalState;
#if defined (STM32MP257Cxx)
  uint32_t OtpPart[768];
#else
  uint32_t OtpPart[192];
#endif
} Otp_TypeDef;

/* Exported constants --------------------------------------------------------*/
#if defined (STM32MP257Cxx)
#define OTP_PART_SIZE                   (2 * 384)
#define OTP_VALUE_SIZE                  384
#else
#define OTP_PART_SIZE                   (2 * 96)
#define OTP_VALUE_SIZE                  96
#endif
#define OPENBL_OTP_VERSION              2
#define OTP_STICKY_PROG_LOCK_MASK       (1 << 27)
#define OTP_STICKY_WRITE_LOCK_MASK      (1 << 28)
#define OTP_STICKY_READ_LOCK_MASK       (1 << 29)
#define OTP_STICKY_LOCK_MASK            0x38000000
#define OTP_STICKY_LOCK_POS             27
#define OTP_STICKY_LOCK_ALL             7
#define OTP_PERM_LOCK_MASK              0x40000000
#define OTP_PERM_LOCK_POS               30
#define OTP_PERM_LOCK                   1
#define OTP_REQUEST_UPDATE_MASK         0x80000000
#define OTP_REQUEST_UPDATE_POS          31
#define OTP_REQUEST_UPDATE              1
#define OTP_LOCK_ERROR                  (1 << 26)
#define OTP_READ_ERROR                  (1 << 0)
#define BSEC_SEC_OTP_OPEN               0
#define BSEC_SEC_OTP_CLOSED             1
#define BSEC_SEC_OTP_INVALID            3
#define OTP_ENCRYPTED_DATA              1
#define OTP_HARDWARE_KEY_SET            1
#define OTP_ENCRYPTED_DATA_POS          8
#define OTP_HARDWARE_KEY_SET_POS        31
#define OTP_ERROR                       -1
#define OTP_OK                          0


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OTP_Util_Init(void);
void OTP_Util_DeInit(void);
int OTP_Util_Write(Otp_TypeDef Otp);
Otp_TypeDef OTP_Util_Read(void);

#endif /* OTP_UTIL_H */
