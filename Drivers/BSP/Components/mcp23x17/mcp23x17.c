/**
  ******************************************************************************
  * @file    mcp23x17.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the MCP23x17
  *          IO Expander devices.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020(-2021) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mcp23x17.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup MCP23x17 MCP23x17
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/** @defgroup MCP23x17_Private_Types_Definitions MCP23x17 Private Types Definitions
  * @{
  */

/* IO driver structure initialization */
MCP23x17_IO_Mode_t MCP23x17_IO_Driver =
{
  MCP23x17_IO_Init,
  MCP23x17_DeInit,
  MCP23x17_ReadID,
  MCP23x17_Reset,
  MCP23x17_IO_Start,
  MCP23x17_IO_WritePin,
  MCP23x17_IO_ReadPin,
  MCP23x17_IO_EnableIT,
  MCP23x17_IO_DisableIT,
  MCP23x17_IO_ITStatus,
  MCP23x17_IO_ClearIT,
};


/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/

/** @defgroup MCP23x17_Private_Function_Prototypes MCP23x17 Private Function Prototypes
  * @{
  */
static int32_t MCP23x17_reg16_setPinValue(MCP23x17_Object_t *pObj, uint8_t RegisterAddr, uint32_t PinPosition, uint8_t PinValue );
static int32_t MCP23x17_ReadRegWrap(void *handle, uint16_t Reg, uint8_t* Data, uint16_t Length);
static int32_t MCP23x17_WriteRegWrap(void *handle, uint16_t Reg, uint8_t* Data, uint16_t Length);
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/** @defgroup MCP23x17_Exported_Functions MCP23x17 Exported Functions
  * @{
  */

/**
  * @brief  Initialize the mcp23x17 and configure the needed hardware resources
  * @param  pObj   Pointer to component object.
  * @retval Component status
  */
int32_t MCP23x17_Init(MCP23x17_Object_t *pObj)
{
  int32_t ret = MCP23x17_OK;
  uint8_t tmp;

  /* Get the current register value */
  if (mcp23x17_read_reg(&pObj->Ctx, MCP23x17_IOCONA, &tmp, 1) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }
  else
  {
    /* Reset the value */
    tmp = 0;

    /* Set the new register value */
    if (mcp23x17_write_reg(&pObj->Ctx, MCP23x17_IOCONA, &tmp, 1) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
    /* Get the current register value */
    else if (mcp23x17_read_reg(&pObj->Ctx, MCP23x17_IOCONB, &tmp, 1) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
    else
    {
      /* Reset the value */
      tmp = 0;

      /* Set the new register value */
      if (mcp23x17_write_reg(&pObj->Ctx, MCP23x17_IOCONB, &tmp, 1) != MCP23x17_OK)
      {
        ret = MCP23x17_ERROR;
      }
    }
  }

  return ret;
}

/**
  * @brief  DeInitialize the mcp23x17 and unconfigure the needed hardware resources
  * @param  pObj   Pointer to component object.
  * @retval Component status
  */
int32_t MCP23x17_DeInit(MCP23x17_Object_t *pObj)
{
  if(pObj->IsInitialized == 1U)
  {
    /* De-Initialize IO BUS layer */
    pObj->IO.DeInit();

    pObj->IsInitialized = 0U;
  }
  return MCP23x17_OK;
}


/**
  * @brief  Set the global interrupt Polarity of IRQ_OUT_PIN.
  * @param  pObj   Pointer to component object.
  * @param  Polarity: the IT mode polarity, could be one of the following values:
  *   @arg  MCP23x17_OUT_PIN_POLARITY_LOW: Interrupt output line is active Low edge
  *   @arg  MCP23x17_OUT_PIN_POLARITY_HIGH: Interrupt line output is active High edge
  * @retval Component status
  */
int32_t MCP23x17_SetIrqOutPinPolarity(MCP23x17_Object_t *pObj, uint8_t Polarity)
{
  int32_t ret = MCP23x17_OK;
  uint8_t tmp;

  /* Get the current register value */
  if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_IOCONA, &tmp, 1) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }
  else
  {
    /* Mask the polarity bits */
    tmp &= ~(uint8_t)0x01;

    /* Modify the Interrupt Output line configuration */
    tmp |= Polarity;

    /* Set the new register value */
    if(mcp23x17_write_reg(&pObj->Ctx, MCP23x17_IOCONA, &tmp, 1) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
    /* Get the current register value */
    else if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_IOCONB, &tmp, 1) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
    else
    {
      /* Mask the polarity bits */
      tmp &= ~(uint8_t)0x01;

      /* Modify the Interrupt Output line configuration */
      tmp |= Polarity;

      /* Set the new register value */
      if(mcp23x17_write_reg(&pObj->Ctx, MCP23x17_IOCONB, &tmp, 1) != MCP23x17_OK)
      {
        ret = MCP23x17_ERROR;
      }
    }
  }

  return ret;
}

/**
  * @brief  Set the pullup resistors mode for the port pin.
  * @param  pObj   Pointer to component object.
  * @param  Mode:  Pullup resistors mode, could be one of the following values:
  *   @arg  MCP23x17_GPI_WITH_PULL_RESISTOR: Open Drain output Interrupt line
  *   @arg  MCP23x17_GPI_WITHOUT_PULL_RESISTOR: Push Pull output Interrupt line
  * @retval Component status
  */
int32_t MCP23x17_IO_SetPullupResistorsMode(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t Mode)
{
  int32_t ret = MCP23x17_OK;

  if(MCP23x17_reg16_setPinValue(pObj, MCP23x17_GPPUA, IO_Pin, Mode) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  Set the global interrupt Type of IRQ_OUT_PIN.
  * @param  pObj   Pointer to component object.
  * @param  Type: Interrupt line activity type, could be one of the following values:
  *   @arg  MCP23x17_OUT_PIN_TYPE_OPENDRAIN: Open Drain output Interrupt line
  *   @arg  MCP23x17_OUT_PIN_TYPE_PUSHPULL: Push Pull output Interrupt line
  * @retval Component status
  */
int32_t MCP23x17_SetIrqOutPinType(MCP23x17_Object_t *pObj, uint8_t Type)
{
  int32_t ret = MCP23x17_OK;
  uint8_t tmp;

  /* Get the current register value */
  if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_IOCONA, &tmp, 1) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }
  else
  {
    /* Mask the type bits */
    tmp &= ~(uint8_t)0x02;

    /* Modify the Interrupt Output line configuration */
    tmp |= Type;

    /* Set the new register value */
    if(mcp23x17_write_reg(&pObj->Ctx, MCP23x17_IOCONA, &tmp, 1) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
    /* Get the current register value */
    else if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_IOCONB, &tmp, 1) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
    else
    {
      /* Mask the type bits */
      tmp &= ~(uint8_t)0x02;

      /* Modify the Interrupt Output line configuration */
      tmp |= Type;

      /* Set the new register value */
      if(mcp23x17_write_reg(&pObj->Ctx, MCP23x17_IOCONB, &tmp, 1) != MCP23x17_OK)
      {
        ret = MCP23x17_ERROR;
      }
    }
  }

  return ret;
}


/**
  * @brief  Set the interrupt pin mirror mode.
  * @param  pObj   Pointer to component object.
  * @param  Mode:  Interrupt pin mirror mode, could be one of the following values:
  *   @arg  MCP23x17_OUT_PIN_MIRROR_OFF: Mirror off
  *   @arg  MCP23x17_OUT_PIN_MIRROR_ON: Mirror on
  * @retval Component status
  */
int32_t MCP23x17_SetIrqOutPinMirrorMode(MCP23x17_Object_t *pObj, uint8_t Mode)
{
  int32_t ret = MCP23x17_OK;
  uint8_t tmp;

  /* Get the current register value */
  if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_IOCONA, &tmp, 1) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }
  else
  {
    /* Mask the mirror bits */
    tmp &= ~(uint8_t)0x06;

    /* Modify the Interrupt Output line configuration */
    tmp |= Mode;

    /* Set the new register value */
    if(mcp23x17_write_reg(&pObj->Ctx, MCP23x17_IOCONA, &tmp, 1) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
    /* Get the current register value */
    else if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_IOCONB, &tmp, 1) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
    else
    {
      /* Mask the mirror bits */
      tmp &= ~(uint8_t)0x06;

      /* Modify the Interrupt Output line configuration */
      tmp |= Mode;

      /* Set the new register value */
      if(mcp23x17_write_reg(&pObj->Ctx, MCP23x17_IOCONB, &tmp, 1) != MCP23x17_OK)
      {
        ret = MCP23x17_ERROR;
      }
    }
  }

  return ret;
}


/* ------------------------------------------------------------------ */
/* ----------------------- GPIO ------------------------------------- */
/* ------------------------------------------------------------------ */

/**
  * @brief  Start the IO functionality used
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin IO pin
  * @retval Component status
  */
int32_t MCP23x17_IO_Start(MCP23x17_Object_t *pObj, uint32_t IO_Pin) {

  int32_t ret = MCP23x17_OK;

  /* Set defaults (IO_Pin = 0xFFFF)*/
  /* Set all inputs on port A and B */
  ret += MCP23x17_IO_InitPin(pObj, IO_Pin, MCP23x17_GPIO_DIR_IN);

  /* Turn off interrupt triggers */
  ret += MCP23x17_IO_DisablePinIT(pObj, IO_Pin);

  /* Turn off pull up resistors */
  ret += MCP23x17_IO_SetPullupResistorsMode(pObj, IO_Pin, MCP23x17_GPI_WITHOUT_PULL_RESISTOR);

  return ret;
}


/**
  * @brief  Initializes the IO peripheral according to the specified parameters in the MCP23x17_IO_Init_t.
  * @param  pObj   Pointer to component object.
  * @param  IoInit Pointer to a BSP_IO_Init_t structure that contains
  *         the configuration information for the specified IO pin.
  * @retval Component status
  */
int32_t MCP23x17_IO_Init(MCP23x17_Object_t *pObj, MCP23x17_IO_Init_t *IoInit)
{

  int32_t ret = MCP23x17_OK;

  /* Set direction IN/OUT */
  if((IoInit->Mode == MCP23x17_GPIO_MODE_OUTPUT_PP) || (IoInit->Mode == MCP23x17_GPIO_MODE_OUTPUT_OD))
  {
    ret += MCP23x17_IO_InitPin(pObj, IoInit->Pin, MCP23x17_GPIO_DIR_OUT);
  }
  else
  {
    ret += MCP23x17_IO_InitPin(pObj, IoInit->Pin, MCP23x17_GPIO_DIR_IN);
  }

  /* Set Push-Pull type */
  switch(IoInit->Pull)
  {
  case MCP23x17_GPIO_NOPULL:
    ret += MCP23x17_IO_SetPullupResistorsMode(pObj, IoInit->Pin, MCP23x17_GPI_WITHOUT_PULL_RESISTOR);
    break;
  case MCP23x17_GPIO_PULLUP:
  case MCP23x17_GPIO_PULLDOWN:
    ret += MCP23x17_IO_SetPullupResistorsMode(pObj, IoInit->Pin, MCP23x17_GPI_WITH_PULL_RESISTOR);
    break;
  default:
    break;
  }

  if(IoInit->Mode == MCP23x17_GPIO_MODE_OUTPUT_PP)
  {
    ret += MCP23x17_SetIrqOutPinPolarity(pObj, MCP23x17_GPO_PUSH_PULL);
  }

  if(IoInit->Mode == MCP23x17_GPIO_MODE_OUTPUT_OD)
  {
    ret += MCP23x17_SetIrqOutPinPolarity(pObj, MCP23x17_GPO_OPEN_DRAIN);
  }

  /* Set Irq event and type mode */
  switch(IoInit->Mode)
  {
  case MCP23x17_GPIO_MODE_IT_RISING_EDGE:
    ret += MCP23x17_IO_SetIrqEvtMode(pObj, IoInit->Pin, MCP23x17_IRQ_GPI_EVT_EDGE);
    ret += MCP23x17_IO_SetIrqTypeMode(pObj, IoInit->Pin, MCP23x17_IRQ_GPI_TYPE_HLRE);
    ret += MCP23x17_IO_EnablePinIT(pObj, IoInit->Pin);  /* last to do: enable IT */
    break;
  case MCP23x17_GPIO_MODE_IT_FALLING_EDGE:
    ret += MCP23x17_IO_SetIrqEvtMode(pObj, IoInit->Pin, MCP23x17_IRQ_GPI_EVT_EDGE);
    ret += MCP23x17_IO_SetIrqTypeMode(pObj, IoInit->Pin, MCP23x17_IRQ_GPI_TYPE_LLFE);
    ret += MCP23x17_IO_EnablePinIT(pObj, IoInit->Pin);  /* last to do: enable IT */
    break;
  case MCP23x17_GPIO_MODE_IT_HIGH_LEVEL:
    ret += MCP23x17_IO_SetIrqEvtMode(pObj, IoInit->Pin, MCP23x17_IRQ_GPI_EVT_LEVEL);
    ret += MCP23x17_IO_SetIrqTypeMode(pObj, IoInit->Pin, MCP23x17_IRQ_GPI_TYPE_HLRE);
    ret += MCP23x17_IO_EnablePinIT(pObj, IoInit->Pin);  /* last to do: enable IT */
    break;
  case MCP23x17_GPIO_MODE_IT_LOW_LEVEL:
    ret += MCP23x17_IO_SetIrqEvtMode(pObj, IoInit->Pin, MCP23x17_IRQ_GPI_EVT_LEVEL);
    ret += MCP23x17_IO_SetIrqTypeMode(pObj, IoInit->Pin, MCP23x17_IRQ_GPI_TYPE_LLFE);
    ret += MCP23x17_IO_EnablePinIT(pObj, IoInit->Pin);  /* last to do: enable IT */
    break;
  default:
    break;
  }

  if(ret != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  Initialize the selected IO pin direction.
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin The IO pin to be configured. This parameter could be any
  *         combination of the following values:
  *   @arg  MCP23x17_GPIO_PIN_x: Where x can be from 0 to 16.
  * @param  Direction could be MCP23x17_GPIO_DIR_IN or MCP23x17_GPIO_DIR_OUT.
  * @retval Component status
  */
int32_t MCP23x17_IO_InitPin(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t Direction)
{
  int32_t ret = MCP23x17_OK;

  if(MCP23x17_reg16_setPinValue(pObj, MCP23x17_IODIRA, IO_Pin, Direction) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  Set the global interrupt Type.
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin The IO pin to be configured. This parameter could be any
  *         combination of the following values:
  *   @arg  MCP23x17_GPIO_PIN_x: Where x can be from 0 to 16.
  * @param  Evt: Interrupt line activity type, could be one of the following values:
  *   @arg  MCP23x17_IRQ_GPIO_EVT_LEVEL: Interrupt line is active in level model
  *   @arg  MCP23x17_IRQ_GPIO_EVT_EDGE: Interrupt line is active in edge model
  * @retval Component status
  */
int32_t MCP23x17_IO_SetIrqEvtMode(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t Evt)
{
  int32_t ret = MCP23x17_OK;

  if(MCP23x17_reg16_setPinValue(pObj, MCP23x17_INTCONA, IO_Pin, Evt) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  Configure the Edge for which a transition is detectable for the
  *         selected pin.
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin The IO pin to be configured. This parameter could be any
  *         combination of the following values:
  *   @arg  MCP23x17_GPIO_PIN_x: Where x can be from 0 to 16.
  * @param  Evt: Interrupt line activity type, could be one of the following values:
  *   @arg  MCP23x17_IRQ_GPIO_TYPE_LLFE: Interrupt line is active in Low Level or Falling Edge
  *   @arg  MCP23x17_IRQ_GPIO_TYPE_HLRE: Interrupt line is active in High Level or Rising Edge
  * @retval Component status
  */
int32_t MCP23x17_IO_SetIrqTypeMode(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t Type)
{
  int32_t ret = MCP23x17_OK;

  if(MCP23x17_reg16_setPinValue(pObj, MCP23x17_DEFVALA, IO_Pin, Type) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  When GPIO is in output mode, puts the corresponding GPO in High (1) or Low (0) level.
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin The output pin to be set or reset. This parameter can be one
  *         of the following values:
  *   @arg  MCP23x17_GPIO_PIN_x: where x can be from 0 to 16.
  * @param PinState: The new IO pin state.
  * @retval Component status
  */
int32_t MCP23x17_IO_WritePin(MCP23x17_Object_t *pObj, uint32_t IO_Pin, uint8_t PinState)
{
  int32_t ret = MCP23x17_OK;

  /* Apply the bit value to the selected pin */
  if (PinState != 0U)
  {
    /* Set the SET register */
    if(MCP23x17_reg16_setPinValue(pObj, MCP23x17_GPIOA, IO_Pin, 1) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
  }
  else
  {
    /* Set the CLEAR register */
    if(MCP23x17_reg16_setPinValue(pObj, MCP23x17_GPIOA, IO_Pin, 0) != MCP23x17_OK)
    {
      ret = MCP23x17_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Return the state of the selected IO pin(s).
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin The output pin to read its state. This parameter can be one
  *         of the following values:
  *   @arg  MCP23x17_GPIO_PIN_x: where x can be from 0 to 16.
  * @retval IO pin(s) state.
  */
int32_t MCP23x17_IO_ReadPin(MCP23x17_Object_t *pObj, uint32_t IO_Pin)
{
  uint8_t tmpreg[2];
  uint32_t tmp;

  if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_GPIOA, &tmpreg[0], 1) != MCP23x17_OK)
  {
    return MCP23x17_ERROR;
  }

  if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_GPIOB, &tmpreg[1], 1) != MCP23x17_OK)
  {
    return MCP23x17_ERROR;
  }

  tmp = (uint32_t)tmpreg[0] + ((uint32_t)tmpreg[1] << 8);
  tmp &= IO_Pin;

  return(int32_t)(tmp);
}

/**
  * @brief  Enable the global IO interrupt source.
  * @param  pObj   Pointer to component object.
  * @retval Component status
  */
int32_t MCP23x17_IO_EnableIT(MCP23x17_Object_t *pObj)
{
  int32_t ret = MCP23x17_OK;

  /* Enable global IO IT source */
  if(MCP23x17_IO_EnablePinIT(pObj, MCP23x17_IRQ_GPIO) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  Disable the global IO interrupt source.
  * @param  pObj   Pointer to component object.
  * @retval Component status
  */
int32_t MCP23x17_IO_DisableIT(MCP23x17_Object_t *pObj)
{
  int32_t ret = MCP23x17_OK;

  /* Disable global IO IT source */
  if(MCP23x17_IO_EnablePinIT(pObj, MCP23x17_IRQ_GPIO) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  Enable interrupt mode for the selected IO pin(s).
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin The IO interrupt to be enabled. This parameter could be any
  *         combination of the following values:
  *   @arg  MCP23x17_GPIO_PIN_x: where x can be from 0 to 16.
  * @retval Component status
  */
int32_t MCP23x17_IO_EnablePinIT(MCP23x17_Object_t *pObj, uint32_t IO_Pin)
{
  int32_t ret = MCP23x17_OK;

  if(MCP23x17_reg16_setPinValue(pObj, MCP23x17_GPINTENA, IO_Pin, 1) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  Disable interrupt mode for the selected IO pin(s).
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin The IO interrupt to be disabled. This parameter could be any
  *         combination of the following values:
  *   @arg  MCP23x17_GPIO_PIN_x: where x can be from 0 to 16.
  * @retval Component status
  */
int32_t MCP23x17_IO_DisablePinIT(MCP23x17_Object_t *pObj, uint32_t IO_Pin)
{
  int32_t ret = MCP23x17_OK;

  if(MCP23x17_reg16_setPinValue(pObj, MCP23x17_GPINTENA, IO_Pin, 0) != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  Check the status of the selected IO interrupt pending bit
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin The IO interrupt to be checked could be:
  *   @arg  MCP23x17_GPIO_PIN_x Where x can be from 0 to 16.
  * @retval Status of the checked IO pin(s).
  */
int32_t MCP23x17_IO_ITStatus(MCP23x17_Object_t *pObj, uint32_t IO_Pin)
{
  /* Get the Interrupt status */
  uint8_t tmpreg[2];
  uint32_t tmp;

  if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_GPINTENA, &tmpreg[0], 1) != MCP23x17_OK)
  {
    return MCP23x17_ERROR;
  }

  if(mcp23x17_read_reg(&pObj->Ctx, MCP23x17_GPINTENB, &tmpreg[1], 1) != MCP23x17_OK)
  {
    return MCP23x17_ERROR;
  }

  tmp = (uint32_t)tmpreg[0] + ((uint32_t)tmpreg[1] << 8);
  tmp &= IO_Pin;

  return(int32_t)tmp;
}

/**
  * @brief  Clear the selected IO interrupt pending bit(s).
  * @param  pObj   Pointer to component object.
  * @param  IO_Pin the IO interrupt to be cleared, could be:
  *   @arg  MCP23x17_GPIO_PIN_x: Where x can be from 0 to 16.
  * @retval Component status
  */
int32_t MCP23x17_IO_ClearIT(MCP23x17_Object_t *pObj, uint32_t IO_Pin)
{
  /* Clear the IO IT pending bit(s) by acknowledging */
  /* normally this function is called under interrupt */
  uint8_t pin_0_7, pin_8_15;

  pin_0_7   = (uint8_t)(IO_Pin & 0x0000ffU);
  pin_8_15  = (uint8_t)(IO_Pin >> 8);
  pin_8_15  = (uint8_t)(pin_8_15 & 0x00ffU);

  if (pin_0_7 != 0U)
  {
    if(mcp23x17_write_reg(&pObj->Ctx, MCP23x17_GPINTENA, &pin_0_7, 0) != MCP23x17_OK)
    {
      return MCP23x17_ERROR;
    }
  }
  if (pin_8_15 != 0U)
  {
    if(mcp23x17_write_reg(&pObj->Ctx, MCP23x17_GPINTENB, &pin_8_15, 0) != MCP23x17_OK)
    {
      return MCP23x17_ERROR;
    }
  }

  return MCP23x17_OK;
}

/**
  * @brief  Reset the mcp23x17 by Software.
  * @param  pObj   Pointer to component object.
  * @retval Component status
  */
int32_t MCP23x17_Reset(MCP23x17_Object_t *pObj)
{
  int32_t ret = MCP23x17_OK;

  /* No software reset functionality. Keep blank */

  return ret;
}

/**
  * @brief  Read the MCP23x17 IO Expander device ID.
  * @param  pObj   Pointer to component object.
  * @retval The Device ID (two bytes).
  */
int32_t MCP23x17_ReadID(MCP23x17_Object_t *pObj, uint32_t *Id)
{
  int32_t ret = MCP23x17_OK;

  /* No software read ID functionality. Keep blank */

  return ret;
}

/**
  * @brief  Register Bus Io to component
  * @param  Component object pointer
  * @retval Component status
  */
int32_t MCP23x17_RegisterBusIO(MCP23x17_Object_t *pObj, MCP23x17_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = MCP23x17_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    pObj->Ctx.ReadReg  = MCP23x17_ReadRegWrap;
    pObj->Ctx.WriteReg = MCP23x17_WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if(pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = MCP23x17_ERROR;
    }
  }
  return ret;
}
/**
  * @}
  */

/** @defgroup MCP23x17_Private_Functions MCP23x17 Private Functions
  * @{
  */
/**
  * @brief  Internal routine
  * @param  pObj   Pointer to component object.
  * @param  RegisterAddr: Register Address
  * @param  PinPosition: Pin [0:23]
  * @param  PinValue: 0/1
  * @retval Component status
  */
static int32_t MCP23x17_reg16_setPinValue(MCP23x17_Object_t *pObj, uint8_t RegisterAddr, uint32_t PinPosition, uint8_t PinValue)
{
  int32_t ret = MCP23x17_OK;
  uint8_t tmp;
  uint8_t pin_0_7, pin_8_15;

  pin_0_7   = (uint8_t)(PinPosition & 0x0000ffU);
  pin_8_15  = (uint8_t)(PinPosition >> 8);
  pin_8_15  = (uint8_t)(pin_8_15 & 0x00ffU);

  if (pin_0_7 != 0U)
  {
    /* Get the current register value */
    ret += mcp23x17_read_reg(&pObj->Ctx, RegisterAddr, &tmp, 1);

    /* Set the selected pin direction */
    if (PinValue != 0U)
    {
      tmp |= (uint8_t)pin_0_7;
    }
    else
    {
      tmp &= ~(uint8_t)pin_0_7;
    }

    /* Set the new register value */
    ret += mcp23x17_write_reg(&pObj->Ctx, RegisterAddr, &tmp, 1);
  }

  if (pin_8_15 != 0U)
  {
    /* Get the current register value */
    ret += mcp23x17_read_reg(&pObj->Ctx, ((uint16_t)RegisterAddr+1U), &tmp, 1);

    /* Set the selected pin direction */
    if (PinValue != 0U)
    {
      tmp |= (uint8_t)pin_8_15;
    }
    else
    {
      tmp &= ~(uint8_t)pin_8_15;
    }

    /* Set the new register value */
    ret += mcp23x17_write_reg(&pObj->Ctx, ((uint16_t)RegisterAddr+1U), &tmp, 1);
  }

  if(ret != MCP23x17_OK)
  {
    ret = MCP23x17_ERROR;
  }

  return ret;
}

/**
  * @brief  Wrap MCP23x17 read function to Bus IO function
  * @param  handle Component object handle
  * @param  Reg    The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length buffer size to be written
  * @retval error status
  */
static int32_t MCP23x17_ReadRegWrap(void *handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  MCP23x17_Object_t *pObj = (MCP23x17_Object_t *)handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap MCP23x17 write function to Bus IO function
  * @param  handle  Component object handle
  * @param  Reg     The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length buffer size to be written
  * @retval error status
  */
static int32_t MCP23x17_WriteRegWrap(void *handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  MCP23x17_Object_t *pObj = (MCP23x17_Object_t *)handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
