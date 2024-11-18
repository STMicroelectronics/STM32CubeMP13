/**
  ******************************************************************************
  * @file    openbl_util.c
  * @author  MCD Application Team
  * @brief   Open Bootloader util file
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
#include "openbl_util.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t part_list_size = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
OPENBL_Flashlayout_TypeDef FlashlayoutStruct = {0};

/**
  * @brief  This function is used to parse the flashlayout options.
  * @retval Status PARSE_OK if PASS.
  */
int parse_flash_layout(uint32_t addr, uint32_t size)
{
  /* STM32CubeProgrammer sends the flashlayout info with TAB (ASCII 0x9)
   * between columns and Carriage Return ('\n') between each line.
   * Each line contains 6 columns: opt, id, name, type, ip and offset.
   */
  char *start, *last, *p;
  char opt[33], id[33], name[33], type[33], ip[33], offset[33];
  uint32_t tabcnt = 0;
  bool is_partition = true;
  uint32_t i = 0;

  start = (char *)addr;
  last = start + size;
  *last = '\0'; /* force null terminated string */

  p = start;
  while (*p && (p < last))
  {
    if (is_partition)
    {
      switch (tabcnt)
      {
      case 0:
        opt[i++] = *p++;
        opt[i] = '\0';
        break;
      case 1:
        id[i++] = *p++;
        id[i] = '\0';
        break;
      case 2:
        name[i++] = *p++;
        name[i] = '\0';
        break;
      case 3:
        type[i++] = *p++;
        type[i] = '\0';
        break;
      case 4:
        ip[i++] = *p++;
        ip[i] = '\0';
        break;
      case 5:
        offset[i++] = *p++;
        offset[i] = '\0';
        break;
      default:
        break;
      }
    }

    if (*p == 0x9) /* if TAB */
    {
      tabcnt++;
      p++;
      i = 0;
    }
    else if (*p == '\n') /* if new line */
    {
      part_list_size++;
      is_partition = true;
      if (++p < last && *p == '#')
      {
        part_list_size--;
        is_partition = false;
      }
      if (parse_option(opt, part_list_size - 1) == PARSE_ERROR)
      {
        return PARSE_ERROR;
      }
      if (parse_id(id, part_list_size - 1) == PARSE_ERROR)
      {
        return PARSE_ERROR;
      }
      if (parse_name(name, part_list_size - 1) == PARSE_ERROR)
      {
        return PARSE_ERROR;
      }
      if (parse_type(type, part_list_size - 1) == PARSE_ERROR)
      {
        return PARSE_ERROR;
      }
      if (parse_ip(ip, part_list_size - 1) == PARSE_ERROR)
      {
        return PARSE_ERROR;
      }
      if (parse_offset(offset, part_list_size - 1) == PARSE_ERROR)
      {
        return PARSE_ERROR;
      }
      tabcnt = 0;
      i = 0;
    }
  }

  if (part_list_size > PHASE_LAST_USER)
  {
    return PARSE_ERROR;
  }

  FlashlayoutStruct.partsize = part_list_size;

  return PARSE_OK;
}

/**
  * @brief  This function is used to parse the flashlayout id.
  * @retval int: return value
  */
int parse_id(char *s, uint32_t idx)
{
  uint32_t id;
  errno = 0;
  char *end_ptr = 0;

  id = strtoul(s, &end_ptr, 0);
  /* if string to number conversion is OK */
  if (end_ptr != s)
  {
    /* if the number is not valid */
    if ((id == 0 || id == ULONG_MAX) && errno == ERANGE)
    {
      return PARSE_ERROR;
    }
    else
    {
      FlashlayoutStruct.id[idx] = id;
      return PARSE_OK;
    }
  }
  else
  {
    return PARSE_ERROR;
  }
}

/**
  * @brief  This function is used to parse the flashlayout offset.
  * @retval int: return value
  */
int parse_offset(char *s, uint32_t idx)
{
  uint32_t offset;
  errno = 0;
  char *end_ptr = 0;

  offset = strtoul(s, &end_ptr, 0);
  /* if string to number conversion is OK */
  if (end_ptr != s)
  {
    /* if the number is not valid */
    if ((offset == 0 || offset == ULONG_MAX) && errno == ERANGE)
    {
      return PARSE_ERROR;
    }
    else
    {
      FlashlayoutStruct.offset[idx] = offset;
      return PARSE_OK;
    }
  }
  else
  {
    return PARSE_ERROR;
  }
}

/**
  * @brief  This function is used to parse the flashlayout name.
  * @retval int: return value
  */
int parse_name(char *s, uint32_t idx)
{
  size_t size = strlen(s) + 1;
  FlashlayoutStruct.name[idx] = (char *) malloc(size);
  strcpy(FlashlayoutStruct.name[idx], s);
  return PARSE_OK;
}

/**
  * @brief  This function is used to parse the flashlayout type.
  * @retval int: return value
  */
int parse_type(char *s, uint32_t idx)
{
  size_t size = strlen(s) + 1;
  FlashlayoutStruct.type[idx] = (char *) malloc(size);
  strcpy(FlashlayoutStruct.type[idx], s);
  return PARSE_OK;
}

/**
  * @brief  This function is used to parse the FlashlayoutStructashlayout ip.
  * @retval int: return value
  */
int parse_ip(char *s, uint32_t idx)
{
  size_t size = strlen(s) + 1;
  FlashlayoutStruct.ip[idx] = (char *) malloc(size);
  strcpy(FlashlayoutStruct.ip[idx], s);
  return PARSE_OK;
}

/**
  * @brief  This function is used to parse the flashlayout option.
  * @retval int: return value
  */
int parse_option(char *s, uint32_t idx)
{
  size_t size = strlen(s) + 1;
  FlashlayoutStruct.opt[idx] = (char *) malloc(size);
  strcpy(FlashlayoutStruct.opt[idx], s);
  return PARSE_OK;
}

/**
  * @brief  This function is used to parse the boot interface selected.
  * @retval Boot interface ID.
  */
int parse_boot_interface_selected(uint32_t addr)
{
  uint8_t *p = (uint8_t *)addr;
  uint8_t itf_sel = *p;
  if (itf_sel == BOOT_INTERFACE_SEL_SERIAL_UART)
  {
    return UART_ID;
  }
  else if (itf_sel == BOOT_INTERFACE_SEL_SERIAL_USB)
  {
    return USB_ID;
  }
  else /* Not supported interface */
  {
    return UNDEF_ID; /* Keep default interface (UART) */
  }
}
