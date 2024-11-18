/**
  ******************************************************************************
  * @file    otp_util.c
  * @author  MCD Application Team
  * @brief   OTP utility functions
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
#include "otp_util.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
BSEC_HandleTypeDef handleBsec;
BSEC_HandleTypeDef * hbsec = &handleBsec;
static uint32_t otp_val_idx;
static uint32_t otp_stat_idx;
static uint32_t otp_idx;
static HAL_StatusTypeDef status;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief Init the otp
  * @param None
  * @retval None
  */
void OTP_Util_Init(void)
{
  hbsec->Instance = BSEC;
#if defined (STM32MP257Cxx)
  HAL_PWR_EnableBkUpD3Access();
#else
  HAL_PWR_EnableBkUpAccess();
#endif /* STM32MP257Cxx */
  __HAL_RCC_BSEC_CLK_ENABLE();
  HAL_BSEC_Init(hbsec);
#if !defined (STM32MP257Cxx)
  HAL_BSEC_SafMemPwrUp(hbsec, BSEC_SAFMEM_CLK_RANGE_MAX);
#endif /* !STM32MP257Cxx */
}

/**
  * @brief De-init the otp
  * @param None
  * @retval None
  */
void OTP_Util_DeInit(void)
{
  hbsec->Instance = BSEC;
#if !defined (STM32MP257Cxx)
  HAL_BSEC_SafMemPwrDown(hbsec);
#endif
  HAL_BSEC_DeInit(hbsec);
  __HAL_RCC_BSEC_CLK_DISABLE();
}

/**
  * @brief Write the otp
  * @param Otp: the otp structure
  * @retval OTP_OK: if no error
  *         other: if error
  */
int OTP_Util_Write(Otp_TypeDef Otp)
{
  uint32_t valueW;
  uint32_t otpPermWLockValue = 0;
  uint32_t stickyLockValue;
  uint32_t requestUpdateValue;

  for (otp_val_idx = 0, otp_stat_idx = otp_val_idx + 1, otp_idx = (otp_val_idx / 2); otp_val_idx < OTP_PART_SIZE; otp_val_idx += 2, otp_stat_idx = otp_val_idx + 1, otp_idx = (otp_val_idx / 2))
  {
    /* Get the request update bit */
    requestUpdateValue = (uint8_t)((Otp.OtpPart[otp_stat_idx] & OTP_REQUEST_UPDATE_MASK) >> OTP_REQUEST_UPDATE_POS);

    /* Check if there is request update */
    if (requestUpdateValue != OTP_REQUEST_UPDATE)
    {
      /* Continue to the next otp as no update on this otp */
      continue;
    }

    /*### Program the otp value ###*/
    /* Get the otp value */
    valueW = Otp.OtpPart[otp_val_idx];

    /* Skip write value if value = 0 */
    if (valueW != 0)
    {
      /* Write the otp value */
      status = HAL_BSEC_OtpProgram(hbsec, otp_idx, valueW);

      /* Check the status */
      if (status != HAL_OK) return OTP_ERROR;
    }

    /*### Program the otp status ###*/
    /* Status frame usefull values */
    /* bit0 = read error detected, 1 => invalid value */
    /* bit26 = lock error */
    /* bit27 = sticky programming lock */
    /* bit28 = shadow write sticky lock */
    /* bit29 = shadow read sticky lock */
    /* bit30 = permanent write lock */
    /* bit31 = request update */
    /*###########################*/

    /* Get the sticky lock value (1 = bit27, 2 = bit28 ,4 = bit29 */
    stickyLockValue = (uint8_t)((Otp.OtpPart[otp_stat_idx] & OTP_STICKY_LOCK_MASK) >> OTP_STICKY_LOCK_POS);

    /* Check if there is a sticky lock */
    if ((stickyLockValue & OTP_STICKY_LOCK_ALL) != 0)
    {
      /* Program the sticky lock */
      status = HAL_BSEC_SetOtpStickyLock(hbsec, otp_idx, stickyLockValue);

      /* Check the status */
      if (status != HAL_OK) return OTP_ERROR;
    }

    /* Get the permanent lock value */
    otpPermWLockValue = (uint8_t)((Otp.OtpPart[otp_stat_idx] & OTP_PERM_LOCK_MASK) >> OTP_PERM_LOCK_POS);

    /* Check if there is a permanent lock */
    if (otpPermWLockValue == OTP_PERM_LOCK)
    {
      /* Program the permanent lock */
      status = HAL_BSEC_SetOtpPermanentProgLock(hbsec, otp_idx);
    }
    /* Check the status */
    if (status != HAL_OK) return OTP_ERROR;
  }

  return OTP_OK;
}

/**
  * @brief Read the otp
  * @param None
  * @retval Otp: the otp structure
  */
Otp_TypeDef OTP_Util_Read(void)
{
  uint32_t valueR;
  uint32_t stickyLockR;
  uint32_t otpPermWLockR;
  uint32_t statusR = 0;
  BSEC_ChipSecurityTypeDef secR = {0};
  Otp_TypeDef Otp = {0};

  /* Get the otp version */
  Otp.Version = OPENBL_OTP_VERSION;

  /* Get security status */
  status = HAL_BSEC_GetSecurityStatus(hbsec, &secR);

  /* Check the status */
  if (status != HAL_OK)
  {
    /* Save otp security as invalid */
#if defined (STM32MP257Cxx)
  secR = BSEC_SEC_OTP_INVALID;
#else
    secR = BSEC_INVALID_STATE;
#endif
  }

  /* Set the otp security */
#if defined (STM32MP257Cxx)
  if (secR == BSEC_UNSECURED_CLOSE_STATE)
  {
    Otp.GlobalState = BSEC_SEC_OTP_OPEN;
  }
  else if (secR == BSEC_SECURED_CLOSE_STATE)
  {
    Otp.GlobalState = BSEC_SEC_OTP_CLOSED;
  }
  else
  {
    Otp.GlobalState = BSEC_SEC_OTP_INVALID;
  }
#else
  if (secR == BSEC_SECURED_OPEN_STATE)
  {
    Otp.GlobalState = BSEC_SEC_OTP_OPEN;
  }
  else if (secR == BSEC_SECURED_CLOSE_STATE)
  {
    Otp.GlobalState = BSEC_SEC_OTP_CLOSED;
  }
  else
  {
    Otp.GlobalState = BSEC_SEC_OTP_INVALID;
  }
#endif /* STM32MP257Cxx */

  /* Get the otp values */
  for (otp_val_idx = 0, otp_stat_idx = otp_val_idx + 1, otp_idx = (otp_val_idx / 2); otp_val_idx < OTP_PART_SIZE; otp_val_idx += 2, otp_stat_idx = otp_val_idx + 1, otp_idx = (otp_val_idx / 2))
  {
    /* Reset the read status variable */
    statusR = 0;

    /* Read the otp word */
    status = HAL_BSEC_OtpRead(hbsec, otp_idx, &valueR);

    /* Save the otp value */
    Otp.OtpPart[otp_val_idx] = valueR;

    /* Check the status */
    if (status != HAL_OK)
    {
      /* Save status as read error */
      statusR += OTP_READ_ERROR;

      /* Save the otp value */
      Otp.OtpPart[otp_stat_idx] = statusR;

      /* Update the otp value to 0 as read error */
      Otp.OtpPart[otp_val_idx] = 0;

      /* Continue to the next otp */
      continue;
    }

    /* Get the sticky lock status */
    status = HAL_BSEC_GetOtpStickyLockStatus(hbsec, otp_idx, &stickyLockR);

    /* Check the status */
    if (status != HAL_OK)
    {
      /* Save status as lock error */
      statusR += OTP_LOCK_ERROR;

      /* Save the otp value */
      Otp.OtpPart[otp_stat_idx] = statusR;

      /* Continue to the next otp */
      continue;
    }

    /* Get the permanent lock status */
    status = HAL_BSEC_GetOtpPermanentProgLockStatus(hbsec, otp_idx, &otpPermWLockR);

    /* Check the status */
    if (status != HAL_OK)
    {
      /* Save status as lock error */
      statusR += OTP_LOCK_ERROR;

      /* Save the otp value */
      Otp.OtpPart[otp_stat_idx] = statusR;

      /* Continue to the next otp */
      continue;
    }

    /* Set the otp status */
    Otp.OtpPart[otp_stat_idx] = (stickyLockR << OTP_STICKY_LOCK_POS) + (otpPermWLockR << OTP_PERM_LOCK_POS);
  }

  return Otp;
}
