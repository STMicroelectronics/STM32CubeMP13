/**
  ******************************************************************************
  * @file    openbl_core.c
  * @author  MCD Application Team
  * @brief   Open Bootloader core file
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
#include "openbl_core.h"
#include "openbl_util.h"
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t NumberOfInterfaces = 0U;
static OPENBL_HandleTypeDef a_InterfacesTable[INTERFACES_SUPPORTED];
static OPENBL_HandleTypeDef *p_Interface;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the registered interfaces in the Open Bootloader MW.
  * @retval None.
  */
void OPENBL_Init(void)
{
  uint32_t counter;

  for (counter = 0U; counter < NumberOfInterfaces; counter++)
  {
    if (a_InterfacesTable[counter].p_Ops->Init != NULL)
    {
      a_InterfacesTable[counter].p_Ops->Init();
    }
  }
}

/**
  * @brief  This function is used to de-initialize the registered interfaces in the Open Bootloader MW.
  * @retval None.
  */
void OPENBL_DeInit(void)
{
  uint32_t counter;

  for (counter = 0U; counter < NumberOfInterfaces; counter++)
  {
    if (a_InterfacesTable[counter].p_Ops->DeInit != NULL)
    {
      a_InterfacesTable[counter].p_Ops->DeInit();
    }
  }
}

/**
  * @brief  This function is used to register a given interface in the Open Bootloader MW.
  * @retval None.
  */
ErrorStatus OPENBL_RegisterInterface(OPENBL_HandleTypeDef *Interface)
{
  ErrorStatus status = SUCCESS;

  if (NumberOfInterfaces < INTERFACES_SUPPORTED)
  {
    a_InterfacesTable[NumberOfInterfaces].p_Ops = Interface->p_Ops;
    a_InterfacesTable[NumberOfInterfaces].p_Cmd = Interface->p_Cmd;

    NumberOfInterfaces++;
  }
  else
  {
    status = ERROR;
  }

  return status;
}

/**
  * @brief  This function is used to detect if there is any activity on a given interface.
  * @retval None.
  */
uint32_t OPENBL_InterfaceDetection(void)
{
  uint8_t detected = 0U;
#if defined(__CP_SERIAL_BOOT__)
  /*only 1 interface in serial boot, the one used by ROM Code.*/
  p_Interface = &(a_InterfacesTable[0]);
  detected = 1;
#else
  uint32_t counter;
  for (counter = 0U; counter < NumberOfInterfaces; counter++)
  {
    if (a_InterfacesTable[counter].p_Ops->Detection != NULL)
    {
      detected = a_InterfacesTable[counter].p_Ops->Detection();

      if (detected == 1U)
      {
        p_Interface = &(a_InterfacesTable[counter]);
        break;
      }
    }
  }
#endif
  return detected;
}

/**
  * @brief  This function is used to get the command opcode from the given interface and execute the right command.
  * @retval None.
  */
void OPENBL_CommandProcess(void)
{
  uint8_t command_opcode;

  /* Get the user command opcode */
  if (p_Interface->p_Ops->GetCommandOpcode != NULL)
  {
    command_opcode = p_Interface->p_Ops->GetCommandOpcode();

    switch (command_opcode)
    {
      case CMD_GET_COMMAND:
        if (p_Interface->p_Cmd->GetCommand != NULL)
        {
          p_Interface->p_Cmd->GetCommand();
        }
        break;

      case CMD_GET_VERSION:
        if (p_Interface->p_Cmd->GetVersion != NULL)
        {
          p_Interface->p_Cmd->GetVersion();
        }
        break;

      case CMD_GET_ID:
        if (p_Interface->p_Cmd->GetID != NULL)
        {
          p_Interface->p_Cmd->GetID();
        }
        break;

      case CMD_GET_PHASE:
        if (p_Interface->p_Cmd->GetPhase != NULL)
        {
          p_Interface->p_Cmd->GetPhase();
        }
        break;

      case CMD_READ_MEMORY:
        if (p_Interface->p_Cmd->ReadMemory != NULL)
        {
          p_Interface->p_Cmd->ReadMemory();
        }
        break;

      case CMD_DOWNLOAD:
        if (p_Interface->p_Cmd->Download != NULL)
        {
          p_Interface->p_Cmd->Download();
        }
        break;

      case CMD_READ_PARTITION:

        if (p_Interface->p_Cmd->ReadPartition != NULL)
        {
          p_Interface->p_Cmd->ReadPartition();
        }
        break;

      case CMD_START:
        if (p_Interface->p_Cmd->Start != NULL)
        {
          p_Interface->p_Cmd->Start();
        }
        break;

      /* Unknown command opcode */
      default:
        if (p_Interface->p_Ops->SendByte != NULL)
        {
            p_Interface->p_Ops->SendByte(NACK_BYTE);
        }
        break;
    }
  }
}
