/**
  ******************************************************************************
  * @file    stmipid02.c
  * @author  GPM Application Team
  * @brief   This file provides the STMIPID02 camera Deserializer driver
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

/* Includes ------------------------------------------------------------------*/
#include "stmipid02.h"
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup STMIPID02
  * @brief     This file provides a set of functions needed to drive the
  *            STMIPID02 Camera module.
  * @{
  */
/** @defgroup STMIPID02_Global_Variables Global Variables
  * @{
  */
/* Global variables ----------------------------------------------------------*/
STMIPID02_Drv_t   STMIPID02_Driver =
{
  .Init = STMIPID02_Init,
  .DeInit = STMIPID02_DeInit,
};
/**
  * @}
  */

/** @defgroup STMIPID02_Private_TypesDefinitions Private Types definition
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
struct regval
{
  uint16_t addr;
  uint8_t val;
};
/**
  * @}
  */

/** @defgroup STMIPID02_Private_Macros Private Macros
  * @{
  */
/* Private macros ------------------------------------------------------------*/
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
  * @}
  */

/** @defgroup STMIPID02_Private_Constants Private Constants
  * @{
  */
/* Private Constants ---------------------------------------------------------*/
static const struct regval reg_start_rgb565[] =
{
  {0x0002, 0x21},
  {0x0004, 0x02},
  {0x0005, 0x03},
  {0x0006, 0x01},
  {0x0009, 0x01},
  {0x000a, 0x01},
  {0x0014, 0x42},
  {0x0015, 0x00},
  {0x0017, 0x22},
  {0x0019, 0x0c},
  {0x001e, 0x08},
  {0x001f, 0x08},

};

static const struct regval reg_start_rggb[] =
{
  {0x0002, 0x41},
  {0x0004, 0x02},
  {0x0005, 0x03},
  {0x0006, 0x01},
  {0x0009, 0x01},
  {0x000a, 0x01},
  {0x0014, 0x42},
  {0x0015, 0x00},
  {0x0017, 0x2a},
  {0x0019, 0x0c},
  {0x001e, 0x08},
  {0x001f, 0x08},
};

static const struct regval reg_start_yuv[] =
{
  {0x0002, 0x21},
  {0x0004, 0x02},
  {0x0005, 0x03},
  {0x0006, 0x01},
  {0x0009, 0x01},
  {0x000a, 0x01},
  {0x0014, 0x42},
  {0x0015, 0x00},
  {0x0017, 0x1e},
  {0x0019, 0x0c},
  {0x001e, 0x08},
  {0x001f, 0x08},
};


static const struct regval reg_stop_stream[] =
{
  {0x0002, 0x00},
  {0x0005, 0x00},
  {0x0009, 0x00}
};

/**
  * @}
  */

/** @defgroup STMIPID02_Private_Functions_Prototypes Private Functions Prototypes
  * @{
  */
/* Private function prototypes -----------------------------------------------*/
static int32_t STMIPID02_WriteTable(STMIPID02_Object_t *pObj, const struct regval *regs, uint32_t size);
static int32_t STMIPID02_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *Data, uint16_t Length);
static int32_t STMIPID02_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *Data, uint16_t Length);

/**
  * @}
  */

/** @defgroup STMIPID02_Private_Functions Private Functions
  * @{
  */

/* Functions Definition ------------------------------------------------------*/

static int32_t STMIPID02_WriteTable(STMIPID02_Object_t *pObj, const struct regval *regs, uint32_t size)
{
  uint32_t index;
  int32_t ret = STMIPID02_OK;

  /* Set registers */
  for (index = 0; index < size ; index++)
  {
    if (ret != STMIPID02_ERROR)
    {
      if (STMIPID02_write_reg(&pObj->Ctx, regs[index].addr, (uint8_t *) & (regs[index].val), 1) != STMIPID02_OK)
      {
        ret = STMIPID02_ERROR;
      }
    }
  }
  return ret;
}

/**
  * @brief  Wrap component ReadReg to Bus Read function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t STMIPID02_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  STMIPID02_Object_t *pObj = (STMIPID02_Object_t *)handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap component WriteReg to Bus Write function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t STMIPID02_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  STMIPID02_Object_t *pObj = (STMIPID02_Object_t *)handle;

  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @}
  */

/** @defgroup STMIPID02_Exported_Functions STMIPID02 Exported Functions
  * @{
  */
/**
  * @brief  Register component IO bus
  * @param  Component object pointer
  * @retval Component status
  */
int32_t STMIPID02_RegisterBusIO(STMIPID02_Object_t *pObj, STMIPID02_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = STMIPID02_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    pObj->Ctx.ReadReg  = STMIPID02_ReadRegWrap;
    pObj->Ctx.WriteReg = STMIPID02_WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = STMIPID02_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initializes the STMIPID02 De-Serializer component.
  * @param  pObj  pointer to component object
  * @param  PixelFormat pixel format to be configured
  * @retval Component status
  */
int32_t STMIPID02_Init(STMIPID02_Object_t *pObj, uint8_t PixelFormat)
{
  int32_t ret = STMIPID02_OK;
  struct regval *reg_list = NULL;
  uint32_t size;


  if (pObj->IsInitialized == 0U)
  {
    switch (PixelFormat)
    {

      case (STMIPID02_PF_RGB565) :
        reg_list = (struct regval *) &reg_start_rgb565;
        size = ARRAY_SIZE(reg_start_rgb565);
        break;
      case (STMIPID02_PF_RGGB8):
        reg_list = (struct regval *) &reg_start_rggb;
        size = ARRAY_SIZE(reg_start_rggb);
        break;
      case (STMIPID02_PF_YUV422):
        reg_list = (struct regval *) &reg_start_yuv;
        size = ARRAY_SIZE(reg_start_yuv);
        break;

      default:
        ret = STMIPID02_ERROR;
        break;
    }

    if (ret == STMIPID02_OK)
    {
      if (STMIPID02_WriteTable(pObj, reg_list, size) != STMIPID02_OK)
      {
        ret = STMIPID02_ERROR;
      }
      else
      {
        /* do nothing */
      }
    }

  }
  else
  {
    ret = STMIPID02_ERROR;
  }

  return ret;
}

/**
  * @brief  De-initializes the De-Serializer.
  * @param  pObj  pointer to component object
  * @retval Component status
  */
int32_t STMIPID02_DeInit(STMIPID02_Object_t *pObj)
{
  uint8_t ret = STMIPID02_OK;

  if (pObj->IsInitialized == 1U)
  {
    /* De-initialize camera sensor interface */
    if (STMIPID02_WriteTable(pObj, reg_stop_stream, ARRAY_SIZE(reg_stop_stream)) != STMIPID02_OK)
    {
      ret = STMIPID02_ERROR;
    }
    else
    {
      pObj->IsInitialized = 0U;
    }

  }

  return ret;
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
