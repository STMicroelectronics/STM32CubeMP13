/**
  ******************************************************************************
  * @file    common_interface.h
  * @author  MCD Application Team
  * @brief   Header for common_interface.c module
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
#ifndef COMMON_INTERFACE_H
#define COMMON_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef void (*Function_Pointer)(void);
typedef void (Send_BusyByte_Func)(void);

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Common_SetMsp(uint32_t TopOfMainStack);
void Common_EnableIrq(void);
void Common_DisableIrq(void);

#endif /* COMMON_INTERFACE_H */
