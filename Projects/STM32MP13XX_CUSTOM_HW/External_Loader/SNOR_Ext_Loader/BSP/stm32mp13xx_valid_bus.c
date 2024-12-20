/**
  ******************************************************************************
  * @file    stm32mp13xx_valid_bus.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage LEDs,
  *          push-buttons, external SDRAM, external QSPI Flash, RF EEPROM,
  *          available  board from STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_valid_bus.h"
#include "stm32mp13xx_valid_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup
  * @{
  */

/** @defgroup BUS
  * @{
  */

/** @defgroup Private_Marcos BUS Private Marcos
  * @{
  */
#define DIV_ROUND_CLOSEST(x, d)  (((x) + ((d) / 2U)) / (d))
/**
  * @}
  */
/** @defgroup Private_Constants BUS Private Constants
  * @{
  */
#define I2C_ANALOG_FILTER_ENABLE         1U
#define I2C_ANALOG_FILTER_DELAY_MIN      50U  /* ns */
#define I2C_ANALOG_FILTER_DELAY_MAX      260  /* ns */
#define I2C_ANALOG_FILTER_DELAY_DEFAULT  2U   /* ns */

#define VALID_PRESC_NBR  100U
#define PRESC_MAX        16U
#define SCLDEL_MAX       16U
#define SDADEL_MAX       16U
#define SCLH_MAX         256U
#define SCLL_MAX         256U
#define I2C_DNF_MAX      16U
#define NSEC_PER_SEC     1000000000UL
/**
  * @}
  */

/** @defgroup Exported_Variables BUS Exported Variables
  * @{
  */
I2C_HandleTypeDef hI2c3;
I2C_HandleTypeDef hI2c4;
/**
  * @}
  */

/** @defgroup Private_Variables BUS Private Variables
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
static uint32_t IsI2c3MspCbValid = 0;
static uint32_t IsI2c4MspCbValid = 0;
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

/**
 * struct i2c_charac - private i2c specification timing
 * @rate: I2C bus speed (Hz)
 * @rate_min: 80% of I2C bus speed (Hz)
 * @rate_max: 100% of I2C bus speed (Hz)
 * @fall_max: Max fall time of both SDA and SCL signals (ns)
 * @rise_max: Max rise time of both SDA and SCL signals (ns)
 * @hddat_min: Min data hold time (ns)
 * @vddat_max: Max data valid time (ns)
 * @sudat_min: Min data setup time (ns)
 * @l_min: Min low period of the SCL clock (ns)
 * @h_min: Min high period of the SCL clock (ns)
 */
struct i2c_specs
{
  uint32_t rate;
  uint32_t rate_min;
  uint32_t rate_max;
  uint32_t fall_max;
  uint32_t rise_max;
  uint32_t hddat_min;
  uint32_t vddat_max;
  uint32_t sudat_min;
  uint32_t l_min;
  uint32_t h_min;
};

enum i2c_speed
{
  I2C_SPEED_STANDARD  = 0U, /* 100 kHz */
  I2C_SPEED_FAST      = 1U, /* 400 kHz */
  I2C_SPEED_FAST_PLUS = 2U, /* 1 MHz */
};

/**
 * struct i2c_setup - private I2C timing setup parameters
 * @rise_time: Rise time (ns)
 * @fall_time: Fall time (ns)
 * @dnf: Digital filter coefficient (0-16)
 * @analog_filter: Analog filter delay (On/Off)
 */
struct i2c_setup
{
  uint32_t rise_time;
  uint32_t fall_time;
  uint32_t dnf;
  uint32_t analog_filter;
};


/**
 * struct i2c_timings - private I2C output parameters
 * @node: List entry
 * @presc: Prescaler value
 * @scldel: Data setup time
 * @sdadel: Data hold time
 * @sclh: SCL high period (master mode)
 * @scll: SCL low period (master mode)
 */
struct i2c_timings
{
  uint32_t presc;
  uint32_t scldel;
  uint32_t sdadel;
  uint32_t sclh;
  uint32_t scll;
};

#if 0
/* Currently not used */
static const struct i2c_specs i2c_specs[] =
{
  [I2C_SPEED_STANDARD] =
  {
    .rate = 100000,
    .rate_min = 100000,
    .rate_max = 120000,
    .fall_max = 300,
    .rise_max = 1000,
    .hddat_min = 0,
    .vddat_max = 3450,
    .sudat_min = 250,
    .l_min = 4700,
    .h_min = 4000,
  },
  [I2C_SPEED_FAST] =
  {
    .rate = 400000,
    .rate_min = 320000,
    .rate_max = 400000,
    .fall_max = 300,
    .rise_max = 300,
    .hddat_min = 0,
    .vddat_max = 900,
    .sudat_min = 100,
    .l_min = 1300,
    .h_min = 600,
  },
  [I2C_SPEED_FAST_PLUS] =
  {
    .rate = 1000000,
    .rate_min = 800000,
    .rate_max = 1000000,
    .fall_max = 120,
    .rise_max = 120,
    .hddat_min = 0,
    .vddat_max = 450,
    .sudat_min = 50,
    .l_min = 500,
    .h_min = 260,
  },
};

static const struct i2c_setup i2c_user_setup[] =
{
  [I2C_SPEED_STANDARD] =
  {
    .rise_time = 400,
    .fall_time = 100,
    .dnf = I2C_ANALOG_FILTER_DELAY_DEFAULT,
    .analog_filter = 1,
  },
  [I2C_SPEED_FAST] =
  {
    .rise_time = 250,
    .fall_time = 100,
    .dnf = I2C_ANALOG_FILTER_DELAY_DEFAULT,
    .analog_filter = 1,
  },
  [I2C_SPEED_FAST_PLUS] =
  {
    .rise_time = 60,
    .fall_time = 100,
    .dnf = I2C_ANALOG_FILTER_DELAY_DEFAULT,
    .analog_filter = 1,
  },
};
#endif /* 0 */
/**
  * @}
  */

/** @defgroup Private_FunctionPrototypes BUS Private FunctionPrototypes
  * @{
  */
#if defined (USE_STM32MP13XX_VALID2)
static void I2C3_MspInit(I2C_HandleTypeDef *hi2c);
static void I2C3_MspDeInit(I2C_HandleTypeDef *hi2c);
static int32_t I2C3_WriteReg(uint16_t DevAddr, uint16_t MemAddSize, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t I2C3_ReadReg(uint16_t DevAddr, uint16_t MemAddSize, uint16_t Reg, uint8_t *pData, uint16_t Length);
#endif

#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
static void I2C4_MspInit(I2C_HandleTypeDef *hi2c);
static void I2C4_MspDeInit(I2C_HandleTypeDef *hi2c);
static int32_t I2C4_WriteReg(uint16_t DevAddr, uint16_t MemAddSize, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t I2C4_ReadReg(uint16_t DevAddr, uint16_t MemAddSize, uint16_t Reg, uint8_t *pData, uint16_t Length);
#endif

#if 0
/* Currenlty not used */
static uint32_t I2C_GetTiming(uint32_t clock_src_hz, uint32_t i2cfreq_hz);
#endif /* 0 */

/**
  * @}
  */

/** @defgroup Private Functions
  * @{
  */
#if defined (USE_STM32MP13XX_VALID2)
/**
  * @brief  Initializes I2C HAL.
  * @retval None
  */
int32_t BSP_I2C3_Init(void)
{
  hI2c3.Instance                  = BUS_I2C3_INSTANCE;

  if(HAL_I2C_GetState(&hI2c3) == HAL_I2C_STATE_RESET)
  {
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
    /* Init the I2C Msp */
    I2C3_MspInit(&hI2c3);
#else
    if(IsI2c3MspCbValid == 0U)
    {
      if(BSP_I2C3_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif

  if(MX_I2C3_Init(&hI2c3) != HAL_OK)

    {
      return BSP_ERROR_BUS_FAILURE;
    }
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  DeInitializes I2C HAL.
  * @retval None
  */
int32_t BSP_I2C3_DeInit(void)
{
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
  I2C3_MspDeInit(&hI2c3);
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 0) */

  /* Init the I2C */
  if (HAL_I2C_DeInit(&hI2c3) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  MX I2C3 initialization.
  * @param  phi2c I2C handle
  * @param  timing I2C timing
  * @retval HAL status
  */
__weak HAL_StatusTypeDef MX_I2C3_Init(I2C_HandleTypeDef *phi2c)
{
  phi2c->Init.Timing           = BUS_I2Cx_TIMING;
  phi2c->Init.OwnAddress1      = 0x33;
  phi2c->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  phi2c->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  phi2c->Init.OwnAddress2      = 0;
  phi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  phi2c->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  phi2c->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

  return HAL_I2C_Init(phi2c);
}

/**
  * @brief  Write a 8bit value in a register of the device through BUS.
  * @param  Addr   Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length buffer size to be written
  * @retval BSP status
  */
int32_t BSP_I2C3_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C3_WriteReg(DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hI2c3) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Read a 8bit register of the device through BUS
  * @param  DevAddr Device address on BUS
  * @param  Reg     The target register address to read
  * @param  pData   Pointer to data buffer
  * @param  Length  Length of the data
  * @retval BSP status
  */
int32_t BSP_I2C3_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C3_ReadReg(DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hI2c3) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Write a 16bit value in a register of the device through BUS.
  * @param  Addr   Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length buffer size to be written
  * @retval BSP status
  */
int32_t BSP_I2C3_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C3_WriteReg(DevAddr, Reg, I2C_MEMADD_SIZE_16BIT, pData, Length) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hI2c3) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Read a 16bit register of the device through BUS
  * @param  DevAddr Device address on BUS
  * @param  Reg     The target register address to read
  * @param  pData   Pointer to data buffer
  * @param  Length  Length of the data
  * @retval BSP status
  */
int32_t BSP_I2C3_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C3_ReadReg(DevAddr, Reg, I2C_MEMADD_SIZE_16BIT, pData, Length) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hI2c3) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Checks if target device is ready for communication.
  * @note   This function is used with Memory devices
  * @param  DevAddress  Target device address
  * @param  Trials      Number of trials
  * @retval BSP status
  */
int32_t BSP_I2C3_IsReady(uint16_t DevAddr, uint32_t Trials)
{
  int32_t ret;

  if(HAL_I2C_IsDeviceReady(&hI2c3, DevAddr, Trials, 1000) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}
#endif /* (USE_STM32MP13XX_VALID2)*/

#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
/**
  * @brief  Initialize BSP I2C4.
  * @retval BSP status.
  */
int32_t BSP_I2C4_Init(void)
{
  hI2c4.Instance                  = BUS_I2C4_INSTANCE;

  if(HAL_I2C_GetState(&hI2c4) == HAL_I2C_STATE_RESET)
  {
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
    /* Init the I2C Msp */
    I2C4_MspInit(&hI2c4);
#else
    if(IsI2c4MspCbValid == 0U)
    {
      if(BSP_I2C4_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif

  if(MX_I2C4_Init(&hI2c4) != HAL_OK)

    {
      return BSP_ERROR_BUS_FAILURE;
    }
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  DeInitializes BSP I2C4.
  * @retval BSP status.
  */
int32_t BSP_I2C4_DeInit(void)
{
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
  I2C4_MspDeInit(&hI2c4);
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 0) */

  /* Init the I2C */
  if (HAL_I2C_DeInit(&hI2c4) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  return BSP_ERROR_NONE;
}

/**
  * @brief  MX I2C4 initialization.
  * @param  phI2c I2C handle
  * @retval HAL status
  */
__weak HAL_StatusTypeDef MX_I2C4_Init(I2C_HandleTypeDef *phi2c)
{
  phi2c->Init.Timing           = BUS_I2Cx_TIMING;
  phi2c->Init.OwnAddress1      = 0x33;   //STPMU1_I2C_ADDRESS
  phi2c->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  phi2c->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  phi2c->Init.OwnAddress2      = 0;
  phi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  phi2c->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  phi2c->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

  return HAL_I2C_Init(phi2c);
}

/**
  * @brief  Write a 8bit value in a register of the device through BUS.
  * @param  Addr   Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length buffer size to be written
  * @retval BSP status
  */
int32_t BSP_I2C4_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C4_WriteReg(DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hI2c4) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Read a 8bit register of the device through BUS
  * @param  DevAddr Device address on BUS
  * @param  Reg     The target register address to read
  * @param  pData   Pointer to data buffer
  * @param  Length  Length of the data
  * @retval BSP status
  */
int32_t BSP_I2C4_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C4_ReadReg(DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hI2c4) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Write a 16bit value in a register of the device through BUS.
  * @param  Addr   Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length buffer size to be written
  * @retval BSP status
  */
int32_t BSP_I2C4_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C4_WriteReg(DevAddr, Reg, I2C_MEMADD_SIZE_16BIT, pData, Length) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hI2c4) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Read a 16bit register of the device through BUS
  * @param  DevAddr Device address on BUS
  * @param  Reg     The target register address to read
  * @param  pData   Pointer to data buffer
  * @param  Length  Length of the data
  * @retval BSP status
  */
int32_t BSP_I2C4_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C4_ReadReg(DevAddr, Reg, I2C_MEMADD_SIZE_16BIT, pData, Length) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hI2c4) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  Checks if target device is ready for communication.
  * @note   This function is used with Memory devices
  * @param  DevAddress  Target device address
  * @param  Trials      Number of trials
  * @retval BSP status
  */
int32_t BSP_I2C4_IsReady(uint16_t DevAddr, uint32_t Trials)
{
  int32_t ret;

  if(HAL_I2C_IsDeviceReady(&hI2c4, DevAddr, Trials, 1000) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

#endif /*(USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)*/


/**
  * @brief  Delay function
  * @retval Tick value
  */
int32_t BSP_GetTick(void)
{
  return (int32_t)HAL_GetTick();
}

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
#if defined (USE_STM32MP13XX_VALID2)
/**
  * @brief Register Default I2C3 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_I2C3_RegisterDefaultMspCallbacks (void)
{
  int32_t ret = BSP_ERROR_NONE;

  __HAL_I2C_RESET_HANDLE_STATE(&hI2c3);

  /* Register default MspInit/MspDeInit Callback */
  if(HAL_I2C_RegisterCallback(&hI2c3, HAL_I2C_MSPINIT_CB_ID, I2C3_MspInit) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if(HAL_I2C_RegisterCallback(&hI2c3, HAL_I2C_MSPDEINIT_CB_ID, I2C3_MspDeInit) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsI2c1MspCbValid = 1U;
  }

  /* BSP status */
  return ret;
}

/**
  * @brief Register I2C3 Bus Msp Callback registering
  * @param Callbacks     pointer to I2C3 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_I2C3_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;

  __HAL_I2C_RESET_HANDLE_STATE(&hI2c3);

  /* Register MspInit/MspDeInit Callbacks */
  if(HAL_I2C_RegisterCallback(&hI2c3, HAL_I2C_MSPINIT_CB_ID, Callback->pMspI2cInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if(HAL_I2C_RegisterCallback(&hI2c3, HAL_I2C_MSPDEINIT_CB_ID, Callback->pMspI2cDeInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsI2c1MspCbValid = 1U;
  }

  /* BSP status */
  return ret;
}
#endif /* (USE_STM32MP13XX_VALID2) */

#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
/**
  * @brief Register Default I2C4 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_I2C4_RegisterDefaultMspCallbacks (void)
{
  int32_t ret = BSP_ERROR_NONE;

  __HAL_I2C_RESET_HANDLE_STATE(&hI2c4);

  /* Register default MspInit/MspDeInit Callback */
  if(HAL_I2C_RegisterCallback(&hI2c4, HAL_I2C_MSPINIT_CB_ID, I2C4_MspInit) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if(HAL_I2C_RegisterCallback(&hI2c4, HAL_I2C_MSPDEINIT_CB_ID, I2C4_MspDeInit) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsI2c1MspCbValid = 1U;
  }

  /* BSP status */
  return ret;
}

/**
  * @brief Register I2C4 Bus Msp Callback registering
  * @param Callbacks     pointer to I2C4 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_I2C4_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;

  __HAL_I2C_RESET_HANDLE_STATE(&hI2c4);

  /* Register MspInit/MspDeInit Callbacks */
  if(HAL_I2C_RegisterCallback(&hI2c4, HAL_I2C_MSPINIT_CB_ID, Callback->pMspI2cInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if(HAL_I2C_RegisterCallback(&hI2c4, HAL_I2C_MSPDEINIT_CB_ID, Callback->pMspI2cDeInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsI2c1MspCbValid = 1U;
  }

  /* BSP status */
  return ret;
}
#endif /* (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3) */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */


/** @addtogroup Private_FunctionPrototypes
  * @{
  */

#if defined (USE_STM32MP13XX_VALID2)
/**
  * @brief  Initializes I2C MSP.
  * @param  hi2c  I2C handler
  * @retval None
  */
void I2C3_MspInit(I2C_HandleTypeDef *phi2c)
{
    GPIO_InitTypeDef  gpio_init_structure;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */

  BUS_I2C3_SCL_GPIO_CLK_ENABLE();
  BUS_I2C3_SDA_GPIO_CLK_ENABLE();

  gpio_init_structure.Mode = GPIO_MODE_AF_OD;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    /* Configure I2C SCL clock as alternate function */
  gpio_init_structure.Alternate = BUS_I2C3_SCL_AF;
  gpio_init_structure.Pin = BUS_I2C3_SCL_PIN;
  HAL_GPIO_Init(BUS_I2C3_SCL_GPIO_PORT, &gpio_init_structure);

  /* Configure I2C SDA data as alternate function */
  gpio_init_structure.Alternate = BUS_I2C3_SDA_AF;
  gpio_init_structure.Pin = BUS_I2C3_SDA_PIN;
  HAL_GPIO_Init(BUS_I2C3_SDA_GPIO_PORT, &gpio_init_structure);

  /*** Configure the I2C peripheral ***/
  /* Enable I2C clock */
  BUS_I2C3_CLK_ENABLE();

  /* Force the I2C peripheral clock reset */
  BUS_I2C3_FORCE_RESET();

  /* Release the I2C peripheral clock reset */
  BUS_I2C3_RELEASE_RESET();

  /* Enable and set I2Cx Interrupt to a lower priority */
#if defined(CORE_CA7)
  IRQ_SetPriority(BUS_I2C3_EV_IRQn, 0);
  IRQ_Enable(BUS_I2C3_EV_IRQn);
  IRQ_SetPriority(BUS_I2C3_ER_IRQn, 0);
  IRQ_Enable(BUS_I2C3_ER_IRQn);
#endif

}
/**
  * @brief  DeInitializes I2C MSP.
  * @param  hi2c  I2C handler
  * @retval None
  */
static void I2C3_MspDeInit(I2C_HandleTypeDef *phi2c)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(phi2c);

  /* Configure I2C Tx, Rx as alternate function */
  gpio_init_structure.Pin = BUS_I2C3_SCL_PIN;
  HAL_GPIO_DeInit(BUS_I2C3_SCL_GPIO_PORT, gpio_init_structure.Pin );
  gpio_init_structure.Pin = BUS_I2C3_SDA_PIN;
  HAL_GPIO_DeInit(BUS_I2C3_SDA_GPIO_PORT, gpio_init_structure.Pin);

  /* Disable GPIO clock */
  BUS_I2C3_SCL_GPIO_CLK_DISABLE();
  BUS_I2C3_SDA_GPIO_CLK_DISABLE();

  /* Disable I2C clock */
  BUS_I2C3_CLK_DISABLE();

    /* Disable IRQ for I2C */
#if defined(CORE_CA7)
  IRQ_Disable(BUS_I2C3_EV_IRQn);
  IRQ_Disable(BUS_I2C3_ER_IRQn);
#endif
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  Addr       Device address on Bus.
  * @param  MemAddSize Size of internal memory address
  * @param  Reg        The target register address to write
  * @param  pData      The target register value to be written
  * @param  Length     data length in bytes
  * @retval BSP status
  */
static int32_t I2C3_WriteReg(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length)
{
  if(HAL_I2C_Mem_Write(&hI2c3, DevAddr, Reg, MemAddSize, pData, Length, 10000) == HAL_OK)
  {
    return BSP_ERROR_NONE;
  }

  return BSP_ERROR_BUS_FAILURE;
}

/**
  * @brief  Read a register of the device through BUS
  * @param  DevAddr    Device address on BUS
  * @param  MemAddSize Size of internal memory address
  * @param  Reg        The target register address to read
  * @retval BSP status
  */
static int32_t I2C3_ReadReg(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length)
{
  if (HAL_I2C_Mem_Read(&hI2c3, DevAddr, Reg, MemAddSize, pData, Length, 10000) == HAL_OK)
  {
    return BSP_ERROR_NONE;
  }

  return BSP_ERROR_BUS_FAILURE;
}
#endif /* (USE_STM32MP13XX_VALID2) */

#if defined (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)
/**
  * @brief  Initializes I2C MSP.
  * @param  hi2c  I2C handler
  * @retval None
  */
void I2C4_MspInit(I2C_HandleTypeDef *phi2c)
{
    GPIO_InitTypeDef  gpio_init_structure;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */

  BUS_I2C4_SCL_GPIO_CLK_ENABLE();
  BUS_I2C4_SDA_GPIO_CLK_ENABLE();

  gpio_init_structure.Mode = GPIO_MODE_AF_OD;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    /* Configure I2C SCL clock as alternate function */
  gpio_init_structure.Alternate = BUS_I2C4_SCL_AF;
  gpio_init_structure.Pin = BUS_I2C4_SCL_PIN;
  HAL_GPIO_Init(BUS_I2C4_SCL_GPIO_PORT, &gpio_init_structure);

  /* Configure I2C SDA data as alternate function */
  gpio_init_structure.Alternate = BUS_I2C4_SDA_AF;
  gpio_init_structure.Pin = BUS_I2C4_SDA_PIN;
  HAL_GPIO_Init(BUS_I2C4_SDA_GPIO_PORT, &gpio_init_structure);

  /*** Configure the I2C peripheral ***/
  /* Enable I2C clock */
  BUS_I2C4_CLK_ENABLE();

  /* Force the I2C peripheral clock reset */
  BUS_I2C4_FORCE_RESET();

  /* Release the I2C peripheral clock reset */
  BUS_I2C4_RELEASE_RESET();

  /* Enable and set I2Cx Interrupt to a lower priority */
#if defined(CORE_CA7)
  IRQ_SetPriority(BUS_I2C4_EV_IRQn, 0);
  IRQ_Enable(BUS_I2C4_EV_IRQn);
  IRQ_SetPriority(BUS_I2C4_ER_IRQn, 0);
  IRQ_Enable(BUS_I2C4_ER_IRQn);
#endif
}
/**
  * @brief  DeInitializes I2C MSP.
  * @param  hi2c  I2C handler
  * @retval None
  */
static void I2C4_MspDeInit(I2C_HandleTypeDef *phi2c)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(phi2c);

  /* Configure I2C Tx, Rx as alternate function */
  gpio_init_structure.Pin = BUS_I2C4_SCL_PIN;
  HAL_GPIO_DeInit(BUS_I2C4_SCL_GPIO_PORT, gpio_init_structure.Pin );
  gpio_init_structure.Pin = BUS_I2C4_SDA_PIN;
  HAL_GPIO_DeInit(BUS_I2C4_SDA_GPIO_PORT, gpio_init_structure.Pin);

  /* Disable GPIO clock */
  BUS_I2C4_SCL_GPIO_CLK_DISABLE();
  BUS_I2C4_SDA_GPIO_CLK_DISABLE();

  /* Disable I2C clock */
  BUS_I2C4_CLK_DISABLE();

    /* Disable IRQ for I2C */
#if defined(CORE_CA7)
  IRQ_Disable(BUS_I2C4_EV_IRQn);
  IRQ_Disable(BUS_I2C4_ER_IRQn);
#endif
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  Addr       Device address on Bus.
  * @param  MemAddSize Size of internal memory address
  * @param  Reg        The target register address to write
  * @param  pData      The target register value to be written
  * @param  Length     data length in bytes
  * @retval BSP status
  */
static int32_t I2C4_WriteReg(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length)
{
  if(HAL_I2C_Mem_Write(&hI2c4, DevAddr, Reg, MemAddSize, pData, Length, 10000) == HAL_OK)
  {
    return BSP_ERROR_NONE;
  }

  return BSP_ERROR_BUS_FAILURE;
}

/**
  * @brief  Read a register of the device through BUS
  * @param  DevAddr    Device address on BUS
  * @param  MemAddSize Size of internal memory address
  * @param  Reg        The target register address to read
  * @retval BSP status
  */
static int32_t I2C4_ReadReg(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length)
{
  if (HAL_I2C_Mem_Read(&hI2c4, DevAddr, Reg, MemAddSize, pData, Length, 10000) == HAL_OK)
  {
    return BSP_ERROR_NONE;
  }

  return BSP_ERROR_BUS_FAILURE;
}

#endif /* (USE_STM32MP13XX_VALID1) || defined (USE_STM32MP13XX_VALID3)*/
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

/**
  * @}
  */