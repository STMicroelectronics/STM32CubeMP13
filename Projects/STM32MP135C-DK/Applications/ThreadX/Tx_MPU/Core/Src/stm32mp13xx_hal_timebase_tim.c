/**
 ******************************************************************************
 * @file    stm32mp13xx_hal_timebase_tim.c
 * @brief   HAL time base based on the hardware TIM.
 *
 *          This file override the native HAL time base functions (defined as weak)
 *          the TIM time base:
 *           + Initializes the TIM peripheral generate a Period elapsed Event each 1ms
 *           + HAL_IncTick is called inside HAL_TIM_PeriodElapsedCallback ie each 1ms
 *
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
#include "stm32mp13xx_hal.h"
/** @addtogroup STM32MP13xx_HAL_Driver
 * @{
 */

#ifdef HAL_TIM_MODULE_ENABLED

/** @addtogroup HAL_TimeBase_TIM
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef        TimHandle;
/* Private function prototypes -----------------------------------------------*/
void TIM6_IRQHandler(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  This function configures the TIM6 as a time base source.
 *         The time source is configured  to have 1ms time base with a dedicated
 *         Tick interrupt priority.
 * @note   This function is called  automatically at the beginning of program after
 *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
 * @param  TickPriority: Tick interrupt priority.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
  HAL_StatusTypeDef status;

  RCC_ClkInitTypeDef    clkconfig;
  uint32_t              uwTimclock;

  uint32_t              uwPrescalerValue = 0U;
  uint32_t              pFLatency = 0U;

  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

  /* Compute TIM6 clock */
  uwTimclock = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_TIM6);

  /* Compute the prescaler value to have TIM6 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

  /* Initialize TIM6 */
  TimHandle.Instance = TIM6;

  /* Initialize TIMx peripheral as follow:
     + Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
     + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
     + ClockDivision = 0
     + Counter direction = Up
   */
  TimHandle.Init.Period = (1000000U / 1000U) - 1U;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0U;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

  __HAL_RCC_TIM6_CLK_ENABLE();

  /* Disabling the timer is useful when debugging, because the timer hardware may have not been reset between two restarts. */
  __HAL_TIM_DISABLE(&TimHandle);
  IRQ_Disable(TIM6_IRQn);
  IRQ_ClearPending(TIM6_IRQn);

  if(HAL_TIM_Base_Init(&TimHandle) == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    status = HAL_TIM_Base_Start_IT(&TimHandle);
    IRQ_SetPriority(TIM6_IRQn, 0U);
    IRQ_Enable(TIM6_IRQn);
    return status;
  }

  /* Return function status */
  return HAL_ERROR;
}

/**
 * @brief  Suspend Tick increment.
 * @note   Disable the tick increment by disabling TIM6 update interrupt.
 * @param  None
 * @retval None
 */
void HAL_SuspendTick(void)
{
  /* Disable TIM6 update Interrupt */
  __HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_UPDATE);
}

/**
 * @brief  Resume Tick increment.
 * @note   Enable the tick increment by Enabling TIM6 update interrupt.
 * @param  None
 * @retval None
 */
void HAL_ResumeTick(void)
{
  /* Enable TIM6 Update interrupt */
  __HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_UPDATE);
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  HAL_IncTick();
}

/**
 * @brief  This function handles TIM interrupt request.
 * @param  None
 * @retval None
 */
void TIM6_IRQHandler(void)
{
  if (TimHandle.Instance != NULL)
  {
    HAL_TIM_IRQHandler(&TimHandle);
  }
  else
  {
    /* Disable the TIM global Interrupt */
#if defined (CORE_CA7)
    IRQ_Disable(TIM6_IRQn);
#endif
  }
}

/**
 * @}
 */

#endif /* HAL_TIM_MODULE_ENABLED */

/**
 * @}
 */
