/**
  ******************************************************************************
  * @file    Examples/BSP/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stm32mp13xx_hal.h"
#include "stm32mp13xx_disco.h"
#include "stm32mp13xx_disco_ts.h"
#include "stm32mp13xx_disco_lcd.h"

#include "stm32_lcd.h"

/* Exported variables --------------------------------------------------------*/
extern unsigned char stlogo[];
extern __IO uint32_t UserButtonPressed ;



typedef struct
{
  void     (*DemoFunc)(void);
  uint8_t  DemoName[50];
  uint32_t DemoIndex;
} BSP_DemoTypedef;

#define LCD_RST_LOW()                       WRITE_REG(GPIOH->BSRR, (uint32_t)GPIO_PIN_2 << 16)
#define LCD_RST_HIGH()                      WRITE_REG(GPIOH->BSRR, GPIO_PIN_2)
#define LCD_IO_Delay HAL_Delay
/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */
void  Touchscreen_demo1(void);
void  Touchscreen_demo2(void);
void  Touchscreen_demo3(void);
void  Error_Handler(void);
uint8_t CheckForUserInput(void);
uint8_t TouchScreen_GetTouchPosition(void);
void Touchscreen_DrawBackground_Circles(uint8_t state);
#endif /* MAIN_H */
