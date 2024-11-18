/**
  ******************************************************************************
  * @file    otp_interface.h
  * @author  MCD Application Team
  * @brief   Header for otp_interface.c module
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
#ifndef OTP_INTERFACE_H
#define OTP_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include "otp_util.h"
/* Exported types ------------------------------------------------------------*/
typedef Otp_TypeDef OPENBL_Otp_TypeDef;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OPENBL_OTP_Init(void);
void OPENBL_OTP_DeInit(void);
int OPENBL_OTP_Write(OPENBL_Otp_TypeDef Otp);
OPENBL_Otp_TypeDef OPENBL_OTP_Read(void);

#endif /* OTP_INTERFACE_H */
