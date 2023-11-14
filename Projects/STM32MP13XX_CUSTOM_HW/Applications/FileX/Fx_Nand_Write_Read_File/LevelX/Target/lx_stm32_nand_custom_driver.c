/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#include "lx_stm32_nand_custom_driver.h"

/* Enable reading and writing with ECC. This should be enabled for normal operation. Disable it only for debugging. */
#define ECC_ENABLED 1

static UINT  _lx_nand_flash_block_erase_driver(ULONG block, ULONG erase_count);
static UINT  _lx_nand_flash_block_erased_verify_driver(ULONG block);
static UINT  _lx_nand_flash_page_erased_verify_driver(ULONG block, ULONG page);
static UINT  _lx_nand_flash_block_status_get_driver(ULONG block, UCHAR *bad_block_byte);
static UINT  _lx_nand_flash_block_status_set_driver(ULONG block, UCHAR bad_block_byte);
static UINT  _lx_nand_flash_system_error_driver(UINT error_code, ULONG block, ULONG page);

static UINT  _lx_nand_flash_pages_read(ULONG block, ULONG page, UCHAR* main_buffer, UCHAR* spare_buffer, ULONG pages);
static UINT  _lx_nand_flash_pages_write(ULONG block, ULONG page, UCHAR* main_buffer, UCHAR* spare_buffer, ULONG pages);
static UINT  _lx_nand_flash_pages_copy(ULONG source_block, ULONG source_page, ULONG destination_block, ULONG destination_page, ULONG pages, UCHAR* data_buffer);

/* Define NAND flash buffers for LevelX.  */
static ULONG  nand_flash_rw_buffer[(BYTES_PER_PHYSICAL_PAGE + SPARE_BYTES_PER_PAGE + sizeof(ULONG) - 1) / sizeof(ULONG)] = {0};

static uint8_t flash_is_initialized = 0;

/* Initialize nand flash function*/
UINT  _lx_nand_flash_initialize_driver(LX_NAND_FLASH *nand_flash)
{
  NAND_IDTypeDef pNAND_ID;

  if (!flash_is_initialized)
  {
    /* Initialize FMC interface */
    if (BSP_FMC_NAND_Init(NAND_INSTANCE) != BSP_ERROR_NONE)
    {
      return(LX_ERROR);
    }

    /*Read & check the NAND device IDs*/
    /* Initialize the ID structure */
    pNAND_ID.Maker_Id = 0x00;
    pNAND_ID.Device_Id = 0x00;
    pNAND_ID.Third_Id = 0x00;
    pNAND_ID.Fourth_Id = 0x00;

    /* Read the NAND memory ID */
    BSP_FMC_NAND_ReadID(NAND_INSTANCE, &pNAND_ID);

    /* Test the NAND ID correctness */
    if(pNAND_ID.Maker_Id  != BSP_NAND_MANUFACTURER_CODE)
      return(LX_ERROR);
    else if (pNAND_ID.Third_Id != BSP_NAND_THIRD_ID)
      return(LX_ERROR);
    else if (pNAND_ID.Fourth_Id != BSP_NAND_FOURTH_ID)
      return(LX_ERROR);

    flash_is_initialized = 1;
  }

  /* Setup geometry of the NAND flash.  */
  nand_flash->lx_nand_flash_total_blocks =                  TOTAL_BLOCKS;
  nand_flash->lx_nand_flash_pages_per_block =               PHYSICAL_PAGES_PER_BLOCK;
  nand_flash->lx_nand_flash_bytes_per_page =                BYTES_PER_PHYSICAL_PAGE;

  /* Setup function pointers for the NAND flash services.  */
  nand_flash->lx_nand_flash_driver_read =                   NULL;
  nand_flash->lx_nand_flash_driver_write =                  NULL;
  nand_flash->lx_nand_flash_driver_block_erase =            _lx_nand_flash_block_erase_driver;
  nand_flash->lx_nand_flash_driver_block_erased_verify =    _lx_nand_flash_block_erased_verify_driver;
  nand_flash->lx_nand_flash_driver_page_erased_verify =     _lx_nand_flash_page_erased_verify_driver;
  nand_flash->lx_nand_flash_driver_block_status_get =       _lx_nand_flash_block_status_get_driver;
  nand_flash->lx_nand_flash_driver_block_status_set =       _lx_nand_flash_block_status_set_driver;
  nand_flash->lx_nand_flash_driver_extra_bytes_get =        NULL;
  nand_flash->lx_nand_flash_driver_extra_bytes_set =        NULL;
  nand_flash->lx_nand_flash_driver_system_error =           _lx_nand_flash_system_error_driver;

  nand_flash->lx_nand_flash_driver_pages_read =             _lx_nand_flash_pages_read;
  nand_flash->lx_nand_flash_driver_pages_write =            _lx_nand_flash_pages_write;
  nand_flash->lx_nand_flash_driver_pages_copy =             _lx_nand_flash_pages_copy;

  nand_flash->lx_nand_flash_spare_data1_offset =            SPARE_DATA1_OFFSET;
  nand_flash->lx_nand_flash_spare_data1_length =            SPARE_DATA1_LENGTH;

  nand_flash->lx_nand_flash_spare_data2_offset =            SPARE_DATA2_OFFSET;
  nand_flash->lx_nand_flash_spare_data2_length =            SPARE_DATA2_LENGTH;

  nand_flash->lx_nand_flash_spare_total_length =            SPARE_BYTES_PER_PAGE;

  /* Return success.  */
  return(LX_SUCCESS);
}

/* Erase block function*/
static UINT  _lx_nand_flash_block_erase_driver(ULONG block, ULONG erase_count)
{
  LX_PARAMETER_NOT_USED(erase_count);

  BSP_NAND_AddressTypeDef_t BspAddress = {0};

  BspAddress.Block = (uint16_t) block;

  if (BSP_FMC_NAND_Erase_Block(NAND_INSTANCE, &BspAddress) != BSP_ERROR_NONE)
  {
    return(LX_ERROR);
  }

  return(LX_SUCCESS);
}

/* Erase chip function*/
UINT  _lx_nand_flash_erase_all_driver(VOID)
{
  if (BSP_FMC_NAND_EraseChip(NAND_INSTANCE) != BSP_ERROR_NONE)
  {
    return(LX_ERROR);
  }

  return(LX_SUCCESS);
}

/* Verify block erased function*/
static UINT  _lx_nand_flash_block_erased_verify_driver(ULONG block)
{
  uint32_t index1 = 0;
  BSP_NAND_AddressTypeDef_t Bsp_Address = {0};

  Bsp_Address.Block = (uint16_t) block;

  for (index1 = 0; index1 < PHYSICAL_PAGES_PER_BLOCK ; index1++)
  {
    if (_lx_nand_flash_page_erased_verify_driver(Bsp_Address.Block, Bsp_Address.Page) != LX_SUCCESS)
      return(LX_ERROR);

    Bsp_Address.Page++;
  }

  /* Return success.  */
  return(LX_SUCCESS);
}

/* Verify page erased function*/
static UINT  _lx_nand_flash_page_erased_verify_driver(ULONG block, ULONG page)
{
  ULONG *word_ptr;
  ULONG words;
  BSP_NAND_AddressTypeDef_t Bsp_Address = {0};

  memset(nand_flash_rw_buffer, 0, sizeof(nand_flash_rw_buffer));

  Bsp_Address.Block = (uint16_t) block;
  Bsp_Address.Page = (uint16_t) page;

  if (BSP_FMC_NAND_Read(NAND_INSTANCE, &Bsp_Address, (uint8_t *) nand_flash_rw_buffer, 1)!= BSP_ERROR_NONE)
  {
    return(LX_ERROR);
  }

  word_ptr = (ULONG *) &(nand_flash_rw_buffer[0]);

  /* Calculate the number of words in a page.  */
  words =  WORDS_PER_PHYSICAL_PAGE;

  /* Loop to read flash.  */
  while (words--)
  {
    /* Is this word erased?  */
    if (*word_ptr++ != 0xFFFFFFFF)
      return(LX_ERROR);
  }

  /* Return success.  */
  return(LX_SUCCESS);
}

/* Get block status function*/
static UINT _lx_nand_flash_block_status_get_driver(ULONG block, UCHAR *bad_block_byte)
{
  BSP_NAND_AddressTypeDef_t Bsp_Address = {0};

  Bsp_Address.Block = (uint16_t) block;

  if (BSP_FMC_NAND_Read_SpareArea(NAND_INSTANCE, &Bsp_Address, (uint8_t *) nand_flash_rw_buffer, 1) != BSP_ERROR_NONE)
  {
    return(LX_ERROR);
  }

  *bad_block_byte = (UCHAR) nand_flash_rw_buffer[BAD_BLOCK_POSITION];

  /* Return success.  */
  return(LX_SUCCESS);
}

/* Set block status function*/
static UINT _lx_nand_flash_block_status_set_driver(ULONG block, UCHAR bad_block_byte)
{
  BSP_NAND_AddressTypeDef_t Bsp_Address = {0};

  Bsp_Address.Block = (uint16_t) block;

  if (BSP_FMC_NAND_Read_SpareArea(NAND_INSTANCE, &Bsp_Address, (uint8_t *) nand_flash_rw_buffer, 1) != BSP_ERROR_NONE)
  {
    return(LX_ERROR);
  }

  nand_flash_rw_buffer[BAD_BLOCK_POSITION] = bad_block_byte;

  if (BSP_FMC_NAND_Write_SpareArea(NAND_INSTANCE, &Bsp_Address, (uint8_t *) nand_flash_rw_buffer, 1) != BSP_ERROR_NONE)
  {
    return(LX_ERROR);
  }

  /* Return success.  */
  return(LX_SUCCESS);
}

/* System error function */
static UINT  _lx_nand_flash_system_error_driver(UINT error_code, ULONG block, ULONG page)
{
  LX_PARAMETER_NOT_USED(error_code);
  LX_PARAMETER_NOT_USED(block);
  LX_PARAMETER_NOT_USED(page);

  /* Custom processing goes here...  all errors except for LX_NAND_ERROR_CORRECTED are fatal.  */
  return(LX_ERROR);
}

static UINT _lx_nand_flash_pages_read(ULONG block, ULONG page, UCHAR* main_buffer, UCHAR* spare_buffer, ULONG pages)
{
  UINT status = LX_SUCCESS;
  BSP_NAND_AddressTypeDef_t Bsp_Address = {0};
  ULONG pageIndex;
#if ECC_ENABLED
  uint32_t numPageRead = 0;
#endif

  for (pageIndex = 0; pageIndex < pages; pageIndex++)
  {
    Bsp_Address.Block = (uint16_t) block;
    Bsp_Address.Page = (uint16_t) (page + pageIndex);

    if (main_buffer)
    {
#if ECC_ENABLED
      if (BSP_FMC_NAND_ReadWithEcc(NAND_INSTANCE, &Bsp_Address, (uint8_t *) main_buffer, 1, &numPageRead) != BSP_ERROR_NONE)
      {
        status = LX_ERROR;
        break;
      }
      if (numPageRead != 1)
      {
        if (_lx_nand_flash_page_erased_verify_driver(Bsp_Address.Block, Bsp_Address.Page) == LX_SUCCESS)
        {
          /* The page was erased and never written before. So this is not an error, the error correction information is just not initialized. */
        }
        else
        {
          /* The page has been written before so this is an ECC error */
          status = LX_NAND_ERROR_NOT_CORRECTED;
          break;
        }
      }
      else
      {
        /* 
        if (???)
        {
          status = LX_NAND_ERROR_CORRECTED;
        }
        */
      }
#else
      if (BSP_FMC_NAND_Read(NAND_INSTANCE, &Bsp_Address, (uint8_t *) main_buffer, 1)!= BSP_ERROR_NONE)
      {
        status = LX_ERROR;
        break;
      }
#endif
    }

    if (spare_buffer)
    {
      if (BSP_FMC_NAND_Read_SpareArea(NAND_INSTANCE, &Bsp_Address, (uint8_t *) spare_buffer, 1) != BSP_ERROR_NONE)
      {
        status = LX_ERROR;
        break;
      }
    }

    main_buffer += BSP_NAND_PAGE_SIZE;
    spare_buffer += BSP_NAND_SPARE_AREA_SIZE;
  }

  return status;
}

static UINT _lx_nand_flash_pages_write(ULONG block, ULONG page, UCHAR* main_buffer, UCHAR* spare_buffer, ULONG pages)
{
  UINT status = LX_SUCCESS;
  BSP_NAND_AddressTypeDef_t Bsp_Address = {0};
  ULONG pageIndex;
#if ECC_ENABLED
  uint32_t numPageWritten = 0;
#endif

  for (pageIndex = 0; pageIndex < pages; pageIndex++)
  {
    Bsp_Address.Block = (uint16_t) block;
    Bsp_Address.Page = (uint16_t) (page + pageIndex);

    if (main_buffer)
    {
#if ECC_ENABLED
      if (BSP_FMC_NAND_WriteWithEcc(NAND_INSTANCE, &Bsp_Address, (uint8_t *) main_buffer, 1, &numPageWritten) != BSP_ERROR_NONE)
      {
        status = LX_ERROR;
        break;
      }
      if (numPageWritten != 1)
      {
        status = LX_ERROR;
        break;
      }
#else
      if (BSP_FMC_NAND_Write(NAND_INSTANCE, &Bsp_Address, (uint8_t *) main_buffer, 1) != BSP_ERROR_NONE)
      {
        status = LX_ERROR;
        break;
      }
#endif
    }

    if (spare_buffer)
    {
      if (BSP_FMC_NAND_Write_SpareArea(NAND_INSTANCE, &Bsp_Address, (uint8_t *) spare_buffer, 1) != BSP_ERROR_NONE)
      {
        status = LX_ERROR;
        break;
      }
    }

    main_buffer += BSP_NAND_PAGE_SIZE;
    spare_buffer += BSP_NAND_SPARE_AREA_SIZE;
  }

  return status;
}

static UINT  _lx_nand_flash_pages_copy(ULONG source_block, ULONG source_page, ULONG destination_block, ULONG destination_page, ULONG pages, UCHAR *data_buffer)
{
  UINT status = LX_SUCCESS;
  BSP_NAND_AddressTypeDef_t Bsp_Address_Src = {0};
  BSP_NAND_AddressTypeDef_t Bsp_Address_Dst = {0};
  ULONG pageIndex;
#if ECC_ENABLED
  uint32_t numPageRead = 0;
  uint32_t numPageWritten = 0;
#endif

  for (pageIndex = 0; pageIndex < pages; pageIndex++)
  {
    Bsp_Address_Src.Block = (uint16_t) source_block;
    Bsp_Address_Src.Page = (uint16_t) (source_page + pageIndex);
    Bsp_Address_Dst.Block = (uint16_t) destination_block;
    Bsp_Address_Dst.Page = (uint16_t) (destination_page + pageIndex);

#if ECC_ENABLED
    if (BSP_FMC_NAND_ReadWithEcc(NAND_INSTANCE, &Bsp_Address_Src, (uint8_t *) nand_flash_rw_buffer, 1, &numPageRead) != BSP_ERROR_NONE)
    {
      status = LX_ERROR;
      break;
    }
    if (numPageRead != 1)
    {
      if (_lx_nand_flash_page_erased_verify_driver(Bsp_Address_Src.Block, Bsp_Address_Src.Page) == LX_SUCCESS)
      {
        /* The page was erased and never written before. So this is not an error, the error correction information is just not initialized. */
      }
      else
      {
        /* The page has been written before so this is an ECC error */
//      status = LX_NAND_ERROR_NOT_CORRECTED;
        status = LX_ERROR;
        break;
      }
    }
    if (BSP_FMC_NAND_WriteWithEcc(NAND_INSTANCE, &Bsp_Address_Dst, (uint8_t *) nand_flash_rw_buffer, 1, &numPageWritten) != BSP_ERROR_NONE)
    {
      status = LX_ERROR;
      break;
    }
    if (numPageWritten != 1)
    {
      status = LX_ERROR;
      break;
    }

#else
    if (BSP_FMC_NAND_Read(NAND_INSTANCE, &Bsp_Address_Src, (uint8_t *) nand_flash_rw_buffer, 1) != BSP_ERROR_NONE)
    {
      status = LX_ERROR;
      break;
    }
    if (BSP_FMC_NAND_Write(NAND_INSTANCE, &Bsp_Address_Dst, (uint8_t *) nand_flash_rw_buffer, 1) != BSP_ERROR_NONE)
    {
      status = LX_ERROR;
      break;
    }
#endif

    if (BSP_FMC_NAND_Read_SpareArea(NAND_INSTANCE, &Bsp_Address_Src, (uint8_t *) nand_flash_rw_buffer, 1) != BSP_ERROR_NONE)
    {
      status = LX_ERROR;
      break;
    }
    if (BSP_FMC_NAND_Write_SpareArea(NAND_INSTANCE, &Bsp_Address_Dst, (uint8_t *) nand_flash_rw_buffer, 1) != BSP_ERROR_NONE)
    {
      status = LX_ERROR;
      break;
    }
  }

  return status;
}
