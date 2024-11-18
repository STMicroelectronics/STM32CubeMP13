/**
  ******************************************************************************
  * @file    otp_interface.c
  * @author  MCD Application Team
  * @brief   Contains common functions used by different interfaces
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

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "openbl_mem.h"
#include "app_openbootloader.h"
#include "common_interface.h"
#include "otp_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief Init the otp
  * @param None
  * @retval None
  */
void OPENBL_OTP_Init(void)
{
  OTP_Util_Init();
}

/**
  * @brief De-init the otp
  * @param None
  * @retval None
  */
void OPENBL_OTP_DeInit(void)
{
  OTP_Util_DeInit();
}

/**
  * @brief Write the otp
  * @param Otp: the otp structure
  * @retval OTP_OK: if no error
  *         other: if error
  */
int OPENBL_OTP_Write(OPENBL_Otp_TypeDef Otp)
{
  int ret = OTP_Util_Write(Otp);
  return ret;
}


/**
  * @brief Read the otp
  * @param None
  * @retval Otp: the otp structure
  */
OPENBL_Otp_TypeDef OPENBL_OTP_Read(void)
{
  OPENBL_Otp_TypeDef ret = OTP_Util_Read();
  return ret;
}
