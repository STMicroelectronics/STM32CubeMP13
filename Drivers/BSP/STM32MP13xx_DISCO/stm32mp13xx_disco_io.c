/**
  ******************************************************************************
  * @file    stm32mp13xx_disco_io.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the IO pins
  *          on STM32MP13xx_DISCO board.
  @verbatim
  How To use this driver:
  -----------------------
   - This driver is used to drive the IO module of the STM32MP13xx-DISCO board.
   - The MCP23x17 IO expander device component driver must be included with this
     driver in order to run the IO functionalities commanded by the IO expander (MCP)
     device mounted on the board.

  Driver description:
  -------------------
  + Initialization steps:
     o Initialize the IO module using the BSP_IO_Init() function. This
       function includes the MSP layer hardware resources initialization and the
       communication layer configuration to start the IO functionalities use.

  + IO functionalities use
     o The IO pin mode is configured when calling the function BSP_IO_ConfigPin(), you
       must specify the desired IO mode by choosing the "IO_ModeTypedef" parameter
       predefined value.
     o If an IO pin is used in interrupt mode, the function BSP_IO_ITGetStatus() is
       needed to get the interrupt status. To clear the IT pending bits, you should
       call the function BSP_IO_ITClear() with specifying the IO pending bit to clear.
     o The IT is handled using the corresponding external interrupt IRQ handler,
       the user IT callback treatment is implemented on the same external interrupt
       callback.
     o The IRQ_OUT pin (common for all functionalities: JOY, SD, LEDs, etc)  can be
       configured using the function BSP_IO_ConfigIrqOutPin()
     o To get/set an IO pin combination state you can use the functions
       BSP_IO_ReadPin()/BSP_IO_WritePin() or the function BSP_IO_TogglePin() to toggle the pin
       state.
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#if defined (MCP_IOEXPANDER)

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_disco_io.h"
#include "stm32mp13xx_disco_bus.h"
#include "stm32mp13xx_disco_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX_DISCO
  * @{
  */

/** @defgroup STM32MP13XX_DISCO_IO IO
  * @{
  */

/** @defgroup  STM32MP13XX_DISCO_IO_Exported_Variables IO Exported Variables
  * @{
  */
IOEXPANDER_Ctx_t                IOECtx[IOEXPANDER_INSTANCES_NBR] = {0};
MCP23x17_Object_t               CompObj[IOEXPANDER_INSTANCES_NBR] = {0};
/**
  * @}
  */

/** @defgroup  STM32MP13XX_DISCO_IO_Private_Variables IO Private Variables
  * @{
  */
static IO_Drv_t                 *IoDrv[IOEXPANDER_INSTANCES_NBR] = {NULL};
/**
  * @}
  */

/** @defgroup  STM32MP13XX_DISCO_IO_Private_Functions_Prototypes IO Private Functions Prototypes
  * @{
  */
static int32_t MCP23x17_Probe(uint32_t Instance);
/**
  * @}
  */

/** @defgroup  STM32MP13XX_DISCO_IO_Exported_Functions IO Exported Functions
  * @{
  */

/**
  * @brief  Initializes and start the IOExpander component
  * @param  Instance IOE instance
  * @param  Function to be initialized. Could be IOEXPANDER_IO_MODE
  * @note   IOEXPANDER_IDD_MODE and IOEXPANDER_TS_MODE are not used on STM32MP13xx-DISCO board
  * @retval BSP status
  */
int32_t BSP_IOEXPANDER_Init(uint32_t Instance, uint32_t Function)
{
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= IOEXPANDER_INSTANCES_NBR) || (Function != IOEXPANDER_IO_MODE))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (IOECtx[Instance].IsInitialized == 0U)
    {
      if (MCP23x17_Probe(Instance) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_NO_INIT;
      }
      else
      {
        IOECtx[Instance].IsInitialized = 1;
      }
    }

    if (IOECtx[Instance].IsInitialized == 1U)
    {
      if (Function == IOEXPANDER_IO_MODE)
      {
        IoDrv[Instance] = (IO_Drv_t *) &MCP23x17_IO_Driver;
      }
      else
      {
        ret = BSP_ERROR_WRONG_PARAM;
      }
    }
  }

  return ret;
}

/**
  * @brief  De-Initializes the IOExpander component
  * @param  Instance IOE instance
  * @note   The de-init is common for all IOE functions
  * @retval BSP status
  */
int32_t BSP_IOEXPANDER_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;;

  if (Instance >= IOEXPANDER_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (IOECtx[Instance].IsInitialized == 1U)
    {
      /* DeInit MCP if no more Functions on are going */
      if (IOECtx[Instance].Functions == 0U)
      {
        if (IoDrv[Instance]->DeInit(&CompObj[Instance]) < 0)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          IOECtx[Instance].IsInitialized = 0;
        }
      }
    }
  }
  return ret;
}

/**
  * @brief  Initializes the IO peripheral according to the specified parameters in the BSP_IO_Init_t.
  * @param  Instance IOE instance
  * @param  Init     pointer to a BSP_IO_Init_t structure that contains
  *         the configuration information for the specified IO pin.
  * @retval BSP status
  */
int32_t BSP_IO_Init(uint32_t Instance, BSP_IO_Init_t *Init)
{
  int32_t status = BSP_ERROR_NONE;

  if ((Instance >= IOEXPANDER_INSTANCES_NBR) || (Init == NULL))
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if ((IOECtx[Instance].Functions & IOEXPANDER_IO_MODE) != IOEXPANDER_IO_MODE)
    {
      if (BSP_IOEXPANDER_Init(Instance, IOEXPANDER_IO_MODE) != BSP_ERROR_NONE)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
      else if (IoDrv[Instance]->Start(&CompObj[Instance], IO_PIN_ALL) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        IOECtx[Instance].Functions |= IOEXPANDER_IO_MODE;
      }
    }

    if (status == BSP_ERROR_NONE)
    {
      /* If IT mode is selected, configures MCP low level interrupt */
      if (Init->Mode >= IO_MODE_IT_RISING_EDGE)
      {
        BSP_IOEXPANDER_ITConfig();
      }

      /* Initializes IO pin */
      if (IoDrv[Instance]->Init(&CompObj[Instance], Init) < 0)
      {
        status = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  return status;
}

/**
  * @brief  DeInitializes the IO peripheral
  * @param  Instance IOE instance
  * @retval BSP status
  */
int32_t BSP_IO_DeInit(uint32_t Instance)
{
  int32_t status = BSP_ERROR_NONE;

  if (Instance >= IOEXPANDER_INSTANCES_NBR)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else if ((IOECtx[Instance].Functions & IOEXPANDER_IO_MODE) != IOEXPANDER_IO_MODE)
  {
    status = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    IOECtx[Instance].Functions &= ~(IOEXPANDER_IO_MODE);
    status = BSP_IOEXPANDER_DeInit(Instance);
  }

  return status;
}

/**
  * @brief  Gets the selected pins IT status.
  * @param  Instance IOE instance
  * @param  IoPin  Selected pins to check the status.
  *          This parameter can be any combination of the IO pins.
  * @retval Pin IT status or BSP_ERROR_WRONG_PARAM
  */
int32_t BSP_IO_GetIT(uint32_t Instance, uint32_t IoPin)
{
  if ((Instance >= IOEXPANDER_INSTANCES_NBR) || ((IOECtx[Instance].Functions & IOEXPANDER_IO_MODE) == 0))
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  /* Return the IO Pin IT status */
  return (IoDrv[Instance]->ITStatus(&CompObj[Instance], IoPin));
}

/**
  * @brief  Clear only one or a selection of IO IT pending bits.
  * @param  Instance IOE instance
  * @param  IO_Pins_To_Clear   MCP IRQ status IO pin to clear (or combination of several IOs)
  * @retval BSP status
  */
int32_t BSP_IO_ClearIT(uint32_t Instance, uint32_t IO_Pins_To_Clear)
{
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= IOEXPANDER_INSTANCES_NBR) || ((IOECtx[Instance].Functions & IOEXPANDER_IO_MODE) == 0))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Clear only the selected list of IO IT pending bits */
    if (IoDrv[Instance]->ClearIT(&CompObj[Instance], IO_Pins_To_Clear) != MCP23x17_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return ret;
}


/**
  * @brief  Sets the selected pins state.
  * @param  Instance IOE instance
  * @param  IoPin  Selected pins to write.
  *          This parameter can be any combination of the IO pins.
  * @param  PinState  New pins state to write
  * @retval BSP status
  */
int32_t BSP_IO_WritePin(uint32_t Instance, uint32_t IoPin, uint32_t PinState)
{
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= IOEXPANDER_INSTANCES_NBR) || ((IOECtx[Instance].Functions & IOEXPANDER_IO_MODE) == 0))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Set the Pin state */
    if (IoDrv[Instance]->WritePin(&CompObj[Instance], IoPin, PinState) != MCP23x17_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Gets the selected pins current state.
  * @param  Instance IOE instance
  * @param  IoPin  Selected pin to read.
  * @retval The current pins state or BSP_ERROR_WRONG_PARAM
  */
int32_t BSP_IO_ReadPin(uint32_t Instance, uint32_t IoPin)
{
  if ((Instance >= IOEXPANDER_INSTANCES_NBR) || ((IOECtx[Instance].Functions & IOEXPANDER_IO_MODE) == 0))
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  return IoDrv[Instance]->ReadPin(&CompObj[Instance], IoPin);
}

/**
  * @brief  Toggles the selected pins state.
  * @param  Instance IOE instance
  * @param  IoPin  Selected pins to toggle.
  *          This parameter can be any combination of the IO pins.
  * @note   This function is only used to toggle one pin in the same time
  * @retval None
  */
int32_t BSP_IO_TogglePin(uint32_t Instance, uint32_t IoPin)
{
  int32_t ret = BSP_ERROR_NONE;

  if ((Instance >= IOEXPANDER_INSTANCES_NBR) || ((IOECtx[Instance].Functions & IOEXPANDER_IO_MODE) == 0))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Toggle the current pin state */
    if (IoDrv[Instance]->ReadPin(&CompObj[Instance], IoPin) != MCP23x17_OK) /* Set */
    {
      if (IoDrv[Instance]->WritePin(&CompObj[Instance], IoPin, 0) != MCP23x17_OK) /* Reset */
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      if (IoDrv[Instance]->WritePin(&CompObj[Instance], IoPin, 1) != MCP23x17_OK) /* Set */
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
  }

  return ret;
}

/**
  * @}
  */

/** @defgroup  STM32MP13XX_DISCO_IO_IO_Private_Functions IO Private Functions
  * @{
  */

/**
  * @brief  Configures MCP low level interrupt.
  * @retval None
  */
void BSP_IOEXPANDER_ITConfig(void)
{
  static uint32_t mcp_io_it_enabled = 0U;
  GPIO_InitTypeDef  gpio_init_structure;

  if (mcp_io_it_enabled == 0U)
  {
    mcp_io_it_enabled = 1U;
    /* Enable the GPIO EXTI clock */
    MCP_IRQOUT_GPIO_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    gpio_init_structure.Pin   = MCP_IRQOUT_PIN;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;
    gpio_init_structure.Mode  = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(MCP_IRQOUT_GPIO_PORT, &gpio_init_structure);

    /* Enable and set GPIO EXTI Interrupt to the lowest priority */
#if defined(CORE_CA7)
    IRQ_SetPriority(MCP_IRQOUT_EXTI_IRQn, 0);
    IRQ_Enable(MCP_IRQOUT_EXTI_IRQn);
#else
    HAL_NVIC_SetPriority((IRQn_Type)(MCP_IRQOUT_EXTI_IRQn), BSP_IOEXPANDER_IT_PRIORITY, 0x0F);
    HAL_NVIC_EnableIRQ((IRQn_Type)(MCP_IRQOUT_EXTI_IRQn));
#endif /* defined(CORE_CA7) */

  }
}

/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t MCP23x17_Probe(uint32_t Instance)
{
  int32_t              ret = BSP_ERROR_NONE;
  MCP23x17_IO_t    IOCtx;
  uint32_t const i2c_address = IO_I2C_ADDRESS;

  /* Configure the audio driver */
  IOCtx.Init        = BSP_I2C1_Init;
  IOCtx.DeInit      = BSP_I2C1_DeInit;
  IOCtx.ReadReg     = BSP_I2C1_ReadReg;
  IOCtx.WriteReg    = BSP_I2C1_WriteReg;
  IOCtx.GetTick     = BSP_GetTick;

  if (MCP23x17_RegisterBusIO(&CompObj[Instance], &IOCtx) != MCP23x17_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  IOCtx.Address     = (uint16_t)i2c_address;

  if (MCP23x17_RegisterBusIO(&CompObj[Instance], &IOCtx) != MCP23x17_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  if (MCP23x17_Init(&CompObj[Instance]) != MCP23x17_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }

  return ret;
}
#endif /* (MCP_IOEXPANDER) */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
