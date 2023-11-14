/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         mmu_for_threadx.c
  * @brief        MMU support for ThreadX Module Manager
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
/* USER CODE END Header */
#include "txm_module.h"

#ifdef TXM_MODULE_MEMORY_PROTECTION_ENABLED

#include "mmu_stm32mp13xx_azurertos.h"
#include "stm32mp13xx.h"
#include <stdio.h>

extern char __TEXT_START__; // Start of code section
extern char __TEXT_END__; // End of code section (4096 bytes aligned)
extern char __RO_START__; // Start of data read-only section
extern char __RO_END__; // End of RO section (4096 bytes aligned)
extern char __RW_START__; // Start of data read/write section
extern char __RW_END__; // End of data read/write section (4096 bytes aligned)

/*********************************************************************************
 * Page tables.
 *********************************************************************************/
#define LEVEL1_PAGES __attribute__ ((aligned(16384), section(".mmu")))
#define LEVEL2_PAGES __attribute__ ((aligned(1024), section(".mmu")))

/*-------------------------------------------------------------------------------
 * Specific page tables.
 *-------------------------------------------------------------------------------*/
static LEVEL2_PAGES uint32_t mmu_level2_page_table_SYSRAM[TXM_LEVEL_2_PAGE_TABLE_ENTRIES];
static LEVEL2_PAGES uint32_t mmu_level2_page_table_SRAM[TXM_LEVEL_2_PAGE_TABLE_ENTRIES];
static LEVEL2_PAGES uint32_t mmu_level2_page_table_GIC[TXM_LEVEL_2_PAGE_TABLE_ENTRIES];

/*-------------------------------------------------------------------------------
 * ThreadX page tables (from txm_module_manager_mm_initialize.c).
 *-------------------------------------------------------------------------------*/
/* ASID table, index is ASID number and contents hold pointer to module.  */
TXM_MODULE_INSTANCE *_txm_asid_table[TXM_ASID_TABLE_LENGTH];

/* Master page table, 2^14 (16kB) alignment.
 * First table is the master level 1 table, the rest are for each module. */
LEVEL1_PAGES ULONG _txm_ttbr1_page_table[TXM_MAXIMUM_MODULES][TXM_MASTER_PAGE_TABLE_ENTRIES];

/* Module start and end level 2 page tables, 2^10 (1kB) alignment.
 * First set of 4 tables are the master level 2 tables, the rest are for each module.
 * Each module needs two L2 tables for code and two L2 tables for data. */
LEVEL2_PAGES ULONG _txm_level2_module_page_table[TXM_MAXIMUM_MODULES * 4][TXM_LEVEL_2_PAGE_TABLE_ENTRIES];

/* Module external memory level 2 page tables, 2^10 (1kB) alignment. */
LEVEL2_PAGES ULONG _txm_level2_external_page_pool[TXM_LEVEL2_EXTERNAL_POOL_PAGES][TXM_LEVEL_2_PAGE_TABLE_ENTRIES];

/*********************************************************************************
 * Section and page descriptors.
 *********************************************************************************/
static uint32_t Sect_Periph_SO; // strongly ordered, shareable, non-executable, rw
static uint32_t Sect_Device_R; // device, non-shareable, non-executable, ro
static uint32_t Sect_Device_RW; // device, non-shareable, non-executable, rw
static uint32_t Sect_Kernel_RW; // global, outer & inner wb/wa, shareable, non-executable, rw
static uint32_t Sect_Kernel_Data_R; // global, outer & inner wb/wa, shareable, non-executable, ro
static uint32_t Sect_Kernel_Data_RW; // global, outer & inner wb/wa, shareable, non-executable, rw
static uint32_t Sect_Kernel_Code; // global, outer & inner wb/wa, shareable, executable, ro
static uint32_t Sect_User_Data_R; // non-global, outer & inner wb/wa, shareable, non-executable, ro
static uint32_t Sect_User_Data_RW; // non-global, outer & inner wb/wa, shareable, non-executable, rw
static uint32_t Sect_User_Code; // non-global, outer & inner wb/wa, shareable, executable, ro

static uint32_t Page_L1_4k; // generic
static uint32_t Page_4k_Device_RW; // global, device, shareable, non-executable, rw
static uint32_t Page_4k_SRAM_RW; // global, non-cacheable, shareable, non-executable, rw
static uint32_t Page_4k_Kernel_Data_R; // global, outer & inner wb/wa, shareable, non-executable, ro
static uint32_t Page_4k_Kernel_Data_RW; // global, outer & inner wb/wa, shareable, non-executable, rw
static uint32_t Page_4k_Kernel_Code; // global, outer & inner wb/wa, shareable, executable, ro
static uint32_t Page_4k_User_Data_R; // non-global, outer & inner wb/wa, shareable, non-executable, ro
static uint32_t Page_4k_User_Data_RW; // non-global, outer & inner wb/wa, shareable, non-executable, rw
static uint32_t Page_4k_User_Code; // non-global, outer & inner wb/wa, shareable, executable, ro

typedef int (*mmu_canModifyLevel2TableType)(TXM_MODULE_INSTANCE *module, uint32_t *ttbl2);
typedef uint32_t* (*mmu_allocateLevel2TableType)(TXM_MODULE_INSTANCE *module);

static int mmu_setupPages(uint32_t s, uint32_t e, uint32_t *ttbl1, uint32_t descl1section, uint32_t descl1, uint32_t descl2, mmu_canModifyLevel2TableType canModifyLevel2Table, mmu_allocateLevel2TableType allocateTable, TXM_MODULE_INSTANCE *module)
{
  uint32_t sectionShift = 20, sectionSize = 0x1UL << sectionShift, sectionMask = sectionSize - 1, sectionEnd;
  uint32_t pageShift = 12, pageSize = 0x1UL << pageShift, pageMask = pageSize - 1, pageCount;
  uint32_t descl1Current, *ttbl2Current, *ttbl2;
  s = s & ~pageMask; /* Round down the start address its page start. */
  e = (e + pageMask) & ~pageMask; /* Round up the end address to its page end. */
  do
  {
    sectionEnd = (s + sectionSize) & ~sectionMask;
    if (sectionEnd > e)
    {
      sectionEnd = e;
    }

    if (((s | sectionEnd) & sectionMask) == 0)
    {
      /* Both start and end are section aligned, we can use a full section, we do not need a level 2 table. */

      MMU_TTSection(ttbl1, s, 1, descl1section);
    }
    else
    {
      /* Only a part of a section is used, we need the level 2 table. */

      descl1Current = ttbl1[s >> sectionShift];
      if (descl1Current & PAGE_L1_DESCRIPTOR)
      {
        /* There is already an L2 table, try to use it. */
        ttbl2Current = (uint32_t*)(descl1Current & 0xFFFFFC00);
        if (!canModifyLevel2Table(module, ttbl2Current))
        {
          /* We cannot modify this level 2 table, allocate a new one and copy the original into it. */
          ttbl2 = allocateTable(module);
          if (!ttbl2)
          {
            /* We run out of L2 tables. */
            return 1;
          }

          /* Copy the original table to the new one. */
          memcpy(ttbl2, ttbl2Current, TXM_LEVEL_2_PAGE_TABLE_ENTRIES * sizeof(uint32_t));

          ttbl2Current = ttbl2;
        }
      }
      else
      {
        /* No L2 table is currently used for this section, use a new one. */

        /* Check if there is still an L2 table available. */
        ttbl2 = allocateTable(module);
        if (!ttbl2)
        {
          /* We run out of L2 tables. */
          return 1;
        }

        ttbl2Current = ttbl2;
      }

      pageCount = (sectionEnd - s) / pageSize;
      MMU_TTPage4k(ttbl1, s, pageCount, descl1, ttbl2Current, descl2);
    }

    s = sectionEnd;
  } while (s < e);

  return 0;
}

/*********************************************************************************
 * Target specific page setup.
 *********************************************************************************/
static int mmu_canModifyLevel2Table(TXM_MODULE_INSTANCE *module, uint32_t *ttbl2)
{
  ULONG asid;
  if (!module)
  {
    return 1;
  }
  asid = module->txm_module_instance_asid;
  if (
      (ttbl2 >= _txm_level2_module_page_table[asid * 4] && ttbl2 < _txm_level2_module_page_table[asid * 4 + 4]) ||
      (ttbl2 >= _txm_level2_external_page_pool[0] && ttbl2 < _txm_level2_external_page_pool[TXM_LEVEL2_EXTERNAL_POOL_PAGES])
      )
  {
    return 1;
  }
  return 0;
}

static uint32_t* mmu_allocateLevel2TableInternal(TXM_MODULE_INSTANCE *module)
{
  ULONG asid = module ? module->txm_module_instance_asid : 0;
  uint32_t* ttbl2 = (uint32_t*)_txm_level2_module_page_table[asid * 4];
  int tableIndex;
  for (tableIndex = 0; tableIndex < 4; tableIndex++)
  {
    if (ttbl2[0] == 0)
    {
      return ttbl2;
    }
    ttbl2 += TXM_LEVEL_2_PAGE_TABLE_ENTRIES;
  }
  return NULL;
}

static uint32_t* mmu_allocateLevel2TableExternal(TXM_MODULE_INSTANCE *module)
{
  uint32_t *table = NULL;
  INT table_index = -1, pool_index = -1, i;

  /* Find first free table slot in module control block. */
  for (i = 0; i < TXM_MODULE_LEVEL2_EXTERNAL_PAGES; i++)
  {
    if (module->txm_external_page_table[i] == TX_NULL)
    {
      table_index = i;
      break;
    }
  }
  if (table_index >= 0)
  {
    /* Find first free table in pool. */
    for (i = 0; i < TXM_LEVEL2_EXTERNAL_POOL_PAGES; i++)
    {
      if (_txm_level2_external_page_pool[i][0] == (ULONG) TX_NULL)
      {
        pool_index = i;
        break;
      }
    }
  }
  if (table_index >= 0 && pool_index >= 0)
  {
    table = _txm_level2_external_page_pool[pool_index];
    module->txm_external_page_table[table_index] = (ULONG*)table;
    memset(table, 0, TXM_LEVEL_2_PAGE_TABLE_ENTRIES * sizeof(ULONG));
  }

  return table;
}

static int mmu_initialize_target()
{
  mmu_region_attributes_Type region;
  uint32_t *ttbl1 = (uint32_t*) _txm_ttbr1_page_table[TXM_MASTER_PAGE_TABLE_INDEX], *ttbl2;
  int shared = 0; /* Set to 1 to enable sharing. Sharing is not needed on a MP1 since there is only one care. Also this can cause performance issues. */
  int codeWritable = 1; /* Set to 1 to allow write access for code pages. This may be necessary for debugging. */

   /* Create faulting entries for full 4GB address space to ensure any access to incorrect memory location will be trapped
      Further sections and pages setup will overwrite these default table entries. */
  MMU_TTSection(ttbl1, 0, TXM_MASTER_PAGE_TABLE_ENTRIES, DESCRIPTOR_FAULT);

  /* Clear all Level 2 tables, this also set all pages as faulting by default. */
  memset(_txm_level2_module_page_table, 0, sizeof(_txm_level2_module_page_table));
  memset(_txm_level2_external_page_pool, 0, sizeof(_txm_level2_external_page_pool));
  memset(mmu_level2_page_table_SYSRAM, 0, sizeof(mmu_level2_page_table_SYSRAM));
  memset(mmu_level2_page_table_SRAM, 0, sizeof(mmu_level2_page_table_SRAM));
  memset(mmu_level2_page_table_GIC, 0, sizeof(mmu_level2_page_table_GIC));

  /*****************************************
   * Generate descriptors. Refer to core_ca.h to get information about attributes
   *****************************************/
  /* Common to all descriptors. */
  region.domain = 0x0;
  region.e_t = ECC_DISABLED;

  /* Warning: MMU_GetSectionDescriptor() and MMU_GetPageDescriptor() call MMU_APSection() and MMU_APPage() with afe=1, so that all combinations of priv_t and user_t are not supported.
     For non supported combinations, this results in no access for both privileged and user mode. */

  /*----------------------------------------
   * Section descriptors
   *----------------------------------------*/
  /* Common to all sections. */
  region.rg_t = SECTION;

  /*
   * Peripheral, device, and kernel sections.
   */

  /* Common to all peripheral, device and kernel sections. */
  region.g_t = GLOBAL;

  /* Common to peripheral and device sections. */
  region.inner_norm_t = NON_CACHEABLE;
  region.outer_norm_t = NON_CACHEABLE;
  region.mem_t = STRONGLY_ORDERED;
  region.sec_t = SECURE;
  region.xn_t = NON_EXECUTE;
#if 0
  /* Temporarily enabled for debugging. */
  region.user_t = RW;
#else
  region.user_t = NO_ACCESS;
#endif
  /* Peripheral strongly ordered section. */
  region.priv_t = RW;
  region.sh_t = shared ? SHARED : NON_SHARED;
  MMU_GetSectionDescriptor(&Sect_Periph_SO, region);
  /* Device strongly ordered read only section. */
  region.priv_t = READ;
  region.sh_t = NON_SHARED; // Never shared.
  MMU_GetSectionDescriptor(&Sect_Device_R, region);
  /* Device strongly ordered read only section. */
  region.priv_t = RW;
  region.sh_t = NON_SHARED; // Never shared.
  MMU_GetSectionDescriptor(&Sect_Device_RW, region);

  /* Common to kernel sections. */
  region.inner_norm_t = WB_WA;
  region.outer_norm_t = WB_WA;
  region.mem_t = NORMAL;
  region.sec_t = SECURE;
  region.sh_t = shared ? SHARED : NON_SHARED;
  /* Common to kernel data sections. */
  region.xn_t = NON_EXECUTE;
  region.user_t = NO_ACCESS;
  /* Kernel read-write section. */
  region.priv_t = RW;
  MMU_GetSectionDescriptor(&Sect_Kernel_RW, region);
  /* Kernel data read only section. */
  region.priv_t = READ;
  MMU_GetSectionDescriptor(&Sect_Kernel_Data_R, region);
  /* Kernel data read-write section. */
  region.priv_t = RW;
  MMU_GetSectionDescriptor(&Sect_Kernel_Data_RW, region);
  /* Kernel code section. */
  region.xn_t = EXECUTE;
  region.priv_t = codeWritable ? RW : READ; /* This should be read-only but the debugger is not able to step in a read-only code section. */
  region.user_t = codeWritable ? RW : READ; /* Necessary for ThreadX module calls. */
  MMU_GetSectionDescriptor(&Sect_Kernel_Code, region);

  /*
   * User sections.
   */
  /* Common to all user sections. */
  region.g_t = NON_GLOBAL;
  region.inner_norm_t = WB_WA;
  region.outer_norm_t = WB_WA;
  region.mem_t = NORMAL;
  region.sec_t = SECURE;
  region.sh_t = shared ? SHARED : NON_SHARED;
  /* User data read only section. */
  region.xn_t = NON_EXECUTE;
  region.priv_t = READ;
  region.user_t = READ;
  MMU_GetSectionDescriptor(&Sect_User_Data_R, region);
  /* User data read-write section. */
  region.xn_t = NON_EXECUTE;
  region.priv_t = RW;
  region.user_t = RW;
  MMU_GetSectionDescriptor(&Sect_User_Data_RW, region);
  /* User code section. */
  region.xn_t = EXECUTE;
  region.priv_t = codeWritable ? RW : READ; /* This should be read-only but the debugger is not able to step in a read-only code section. */
  region.user_t = codeWritable ? RW : READ; /* This should be read-only but the debugger is not able to step in a read-only code section. */
  MMU_GetSectionDescriptor(&Sect_User_Code, region);

  /*----------------------------------------
   * Page descriptors
   *----------------------------------------*/
  /* Common to all pages. */
  region.rg_t = PAGE_4k;

  /*
   * Kernel pages.
   */

  /* Common to all peripheral, device and kernel pages. */
  region.g_t = GLOBAL;

  /* Common to device and SRAM pages. */
  region.inner_norm_t = NON_CACHEABLE;
  region.outer_norm_t = NON_CACHEABLE;
  region.sec_t = SECURE;
  region.user_t = NO_ACCESS;
  /* Device read-write page. */
  region.mem_t = SHARED_DEVICE;
  region.xn_t = NON_EXECUTE;
  region.priv_t = RW;
  region.sh_t = NON_SHARED; /* Never shared. */
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Device_RW, region);
  /* SRAM read-write, executable, non-cacheable page. */
  region.mem_t = NORMAL;
  region.xn_t = EXECUTE;
  region.priv_t = RW;
  region.sh_t = shared ? SHARED : NON_SHARED;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_SRAM_RW, region);

  /* Common to kernel pages. */
  region.inner_norm_t = WB_WA;
  region.outer_norm_t = WB_WA;
  region.mem_t = NORMAL;
  region.sec_t = SECURE;
  region.sh_t = shared ? SHARED : NON_SHARED;
  /* Common to kernel data pages. */
  region.xn_t = NON_EXECUTE;
  region.user_t = NO_ACCESS;
  /* Kernel data read only page. */
  region.priv_t = READ;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Kernel_Data_R, region);
  /* Kernel data read-write page. */
  region.priv_t = RW;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Kernel_Data_RW, region);
  /* Kernel code page. */
  region.xn_t = EXECUTE;
  region.priv_t = codeWritable ? RW : READ; /* This should be read-only but the debugger is not able to step in a read-only code section. */
  region.user_t = codeWritable ? RW : READ; /* Necessary for ThreadX module calls. */
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Kernel_Code, region);

  /*
   * User pages.
   */

  /* Common to all user pages. */
  region.g_t = NON_GLOBAL;
  region.inner_norm_t = WB_WA;
  region.outer_norm_t = WB_WA;
  region.mem_t = NORMAL;
  region.sec_t = SECURE;
  region.sh_t = shared ? SHARED : NON_SHARED;
  /* User data read only page. */
  region.xn_t = NON_EXECUTE;
  region.priv_t = READ;
  region.user_t = READ;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_User_Data_R, region);
  /* User data read-write page. */
  region.xn_t = NON_EXECUTE;
  region.priv_t = RW;
  region.user_t = RW;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_User_Data_RW, region);
  /* User code page. */
  region.xn_t = EXECUTE;
  region.priv_t = codeWritable ? RW : READ; /* This should be read-only but the debugger is not able to step in a read-only code section. */
  region.user_t = codeWritable ? RW : READ; /* This should be read-only but the debugger is not able to step in a read-only code section. */
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_User_Code, region);

  /*****************************************
   * Generate page tables.
   *****************************************/
  /*
   * Device specific areas.
   */
  // Define 256M FMC NOR memory as device, non-shareable, non-executable, ro
  MMU_TTSection(ttbl1, (uint32_t) AXI_BUS_MEMORY_BASE, ((QSPI_MEM_BASE - AXI_BUS_MEMORY_BASE) >> 20), Sect_Device_R);
  // Define 256 QSPI memory as device, non-shareable, non-executable, ro
  MMU_TTSection(ttbl1, (uint32_t) QSPI_MEM_BASE, ((FMC_NAND_MEM_BASE - QSPI_MEM_BASE) >> 20), Sect_Device_R);
  // Define 256M FMC NAND memory as device, non-shareable, non-executable, ro
  MMU_TTSection(ttbl1, (uint32_t) FMC_NAND_MEM_BASE, 256U, Sect_Device_RW);

  /*
   * DDR area.
   */
  // All DDR (1GB) non-executable (except code pages), cacheable, rw
  MMU_TTSection(ttbl1, (uint32_t) DRAM_MEM_BASE, 1024U, Sect_Kernel_RW);

  /*
   * SYSRAM area.
   */
  ttbl2 = mmu_level2_page_table_SYSRAM;
  // Create (256 * 4k)=1MB faulting entries to cover SYSRAM 1M aligned range
  MMU_TTPage4k(ttbl1, SYSRAM_BASE & 0xFFF00000, 1024U / 4U, Page_L1_4k, ttbl2, DESCRIPTOR_FAULT);
  // Create (32 * 4k)=128k RW non-executable entries to cover full SYSRAM
  MMU_TTPage4k(ttbl1, SYSRAM_BASE, 128U / 4U, Page_L1_4k, ttbl2, Page_4k_SRAM_RW);

  /*
   * SRAM area.
   */
  ttbl2 = mmu_level2_page_table_SRAM;
  // Create (256 * 4k)=1MB faulting entries to cover SRAM 1M aligned range
  MMU_TTPage4k(ttbl1, AHB_SRAM & 0xFFF00000, 1024U / 4U, Page_L1_4k, ttbl2, DESCRIPTOR_FAULT);
  // Create (8 * 4k)=32k Normal entries to cover full SRAM
  MMU_TTPage4k(ttbl1, AHB_SRAM, 32U / 4U, Page_L1_4k, ttbl2, Page_4k_SRAM_RW);

  /*
   * Peripherals.
   */
  // Define peripheral memory as strongly ordered, shareable, read/write, non cacheable
  MMU_TTSection(ttbl1, (uint32_t) PERIPH_BASE, ((AXI_BUS_MEMORY_BASE - PERIPH_BASE) >> 20), Sect_Periph_SO);

  /*
   * GIC.
   */
  ttbl2 = mmu_level2_page_table_GIC;
  // Create (256 * 4k)=1MB faulting entries to cover GIC 1M aligned range
  MMU_TTPage4k(ttbl1, GIC_BASE & 0xFFF00000, 1024U / 4U, Page_L1_4k, ttbl2, DESCRIPTOR_FAULT);
  // Create (7 * 4k)=28k Device entries to cover full GIC
  MMU_TTPage4k(ttbl1, GIC_BASE, 28U / 4U, Page_L1_4k, ttbl2, Page_4k_Device_RW);

  /*
   * Code and data.
   */
  ttbl2 = _txm_level2_module_page_table[TXM_MASTER_PAGE_TABLE_INDEX];
  // Create normal, executable and read-only (read/write in reality for debugging) entries to cover code
  if (mmu_setupPages((uint32_t) &__TEXT_START__, (uint32_t) &__TEXT_END__, ttbl1, Sect_Kernel_Code, Page_L1_4k, Page_4k_Kernel_Code, mmu_canModifyLevel2Table, mmu_allocateLevel2TableInternal, NULL))
  {
    printf("Cannot setup application text pages.\n");
    return 1;
  }
  // Create normal, non-executable and read-only entries to cover read-only data
  if (mmu_setupPages((uint32_t) &__RO_START__, (uint32_t) &__RO_END__, ttbl1, Sect_Kernel_Data_R, Page_L1_4k, Page_4k_Kernel_Data_R, mmu_canModifyLevel2Table, mmu_allocateLevel2TableInternal, NULL))
  {
    printf("Cannot setup application read-only pages.\n");
    return 1;
  }
  // Create normal, non-executable and read/write entries to cover writable data
  if (mmu_setupPages((uint32_t) &__RW_START__, (uint32_t) &__RW_END__, ttbl1, Sect_Kernel_Data_RW, Page_L1_4k, Page_4k_Kernel_Data_RW, mmu_canModifyLevel2Table, mmu_allocateLevel2TableInternal, NULL))
  {
    printf("Cannot setup application read/write pages.\n");
    return 1;
  }
  // The remaining 4 KB pages of the 1 MB section must also be initialized, otherwise there will be a crash when accessing this area (because of heap allocation).
  if (mmu_setupPages((uint32_t) &__RW_END__, ((uint32_t) &__RW_END__ + 0x100000UL) & ~(0x100000UL - 1), ttbl1, Sect_Kernel_Data_RW, Page_L1_4k, Page_4k_Kernel_Data_RW, mmu_canModifyLevel2Table, mmu_allocateLevel2TableInternal, NULL))
  {
    printf("Cannot setup application read/write pages.\n");
    return 1;
  }

  return 0;
}

/*********************************************************************************
 * MMU initialization.
 *********************************************************************************/
static void mmu_clearAsidTable()
{
  UINT i;
  for (i = 0; i < TXM_ASID_TABLE_LENGTH; i++)
  {
    _txm_asid_table[i] = 0;
  }
  _txm_asid_table[0] = (TXM_MODULE_INSTANCE*) TXM_ASID_RESERVED;
}

static void mmu_clearAsid()
{
  ULONG cp15reg = 0;
  __asm volatile ("mcr p15, 0, %0, c13, c0, 1" : : "r"(cp15reg) : );
  __ISB();
}

/* Set the domain to client mode. */
static void mmu_setClientMode()
{
  __set_DACR(DACR_Dn_CLIENT << DACR_D_Pos_(0));
  __ISB();
}

static void mmu_setLevel1PageTable(ULONG *pageTable)
{
  /* Set location of level 1 page table in TTBR
   ; 31:14 - Translation table base addr (31:14-TTBCR.N, TTBCR.N is 0 out of reset)
   ; 13:7  - 0x0
   ; 6     - IRGN[0] (Inner WB WA)
   ; 5     - NOS     (Non-shared)
   ; 4:3   - RGN     (Outer WB WA)
   ; 2     - IMP     (Implementation Defined)
   ; 1     - S       (Non-shared)
   ; 0     - IRGN[1] (Inner WB WA) */
  __set_TTBR0((uint32_t)pageTable | 0x9U);
  __ISB();
}

int mmu_initialize()
{
  /* Disable MMU. */
  MMU_Disable();

  mmu_clearAsidTable();
  mmu_clearAsid();
  mmu_setLevel1PageTable(_txm_ttbr1_page_table[TXM_MASTER_PAGE_TABLE_INDEX]);
  mmu_setClientMode();
  if (mmu_initialize_target())
  {
    printf("Cannot initialize target MMU.");
    return 1;
  }

  /* Clean and invalidate caches */
  L1C_CleanInvalidateDCacheAll();
  L1C_InvalidateICacheAll();
  L1C_InvalidateBTAC();
  MMU_InvalidateTLB();

  /* Enables MMU, simplified access permissions model and access flag. */
  MMU_Enable();

  return 0;
}

/*********************************************************************************
 * ThreadX MMU functions.
 *********************************************************************************/

/*-------------------------------------------------------------------------------
 * From txm_module_manager_mm_register_setup.c.
 *-------------------------------------------------------------------------------*/
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _txm_module_manager_inside_data_check           Cortex-A7/MMU/GNU   */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function determines if pointer is within the module's data or  */
/*    shared memory.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    pointer                           Data pointer                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    TXM_MODULE_MANAGER_DATA_POINTER_CHECK                               */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx      Scott Larson            Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
UINT _txm_module_manager_inside_data_check(ULONG pointer)
{
  ULONG translation;

  /* ATS1CUR operation on address supplied in pointer, Stage 1 unprivileged read. */
  __asm volatile ("MCR p15, 0, %0, c7, c8, 2" : : "r"(pointer) : );
  __asm volatile ("ISB");
  /* Ensure completion of the MCR write to CP15. */
  __asm volatile ("MRC p15, 0, %0, c7, c4, 0" : "=r"(translation) : : );
  /* Read result from 32-bit PAR into translation. */
  if (translation & TXM_ADDRESS_TRANSLATION_FAULT_BIT)
  {
    return (TX_FALSE);
  }

  return (TX_TRUE);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _txm_module_manager_assign_asid                 Cortex-A7/MMU/GNU   */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns an Application Specific ID (ASID) to a        */
/*    module.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    module_instance                   Pointer to module instance        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _txm_module_manager_mm_register_setup                               */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx      Scott Larson            Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
UINT _txm_module_manager_assign_asid(TXM_MODULE_INSTANCE *module_instance)
{
  UINT i = 1;

  /* Find first non-zero ASID, starting at index 1. */
  while (i < TXM_ASID_TABLE_LENGTH)
  {
    if (_txm_asid_table[i] != 0)
    {
      i++;
    }
    else
    {
      module_instance->txm_module_instance_asid = i;
      _txm_asid_table[i] = module_instance;
      return (TX_SUCCESS);
    }
  }

  return (TXM_MODULE_ASID_ERROR);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _txm_module_manager_remove_asid                 Cortex-A7/MMU/GNU   */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function removes a module from the ASID list.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    module_instance                   Pointer to module instance        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    TXM_MODULE_MANAGER_MODULE_UNLOAD                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx      Scott Larson            Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
VOID _txm_module_manager_remove_asid(TXM_MODULE_INSTANCE *module_instance)
{
  if (module_instance->txm_module_instance_asid)
  {
    _txm_asid_table[module_instance->txm_module_instance_asid] = 0;
  }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _txm_module_manager_mm_register_setup           Cortex-A7/MMU/GNU   */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets up the Cortex-A7 MMU register definitions based  */
/*    on the module's memory characteristics.                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    module_instance                   Pointer to module instance        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _txm_module_manager_assign_asid                                     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    TXM_MODULE_MANAGER_MODULE_SETUP                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx      Scott Larson            Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
VOID _txm_module_manager_mm_register_setup(TXM_MODULE_INSTANCE *module)
{
  ULONG asid;
  uint32_t *ttbl1, *ttbl1Master, *ttbl2;
  uint32_t codeStart, codeEnd, dataStart, dataEnd;

  /* Assign an ASID to this module. */
  _txm_module_manager_assign_asid(module);
  asid = module->txm_module_instance_asid;

  /* Copy master level 1 page table to module's page table. */
  ttbl1 = (uint32_t *)_txm_ttbr1_page_table[asid];
  ttbl1Master = (uint32_t *)_txm_ttbr1_page_table[TXM_MASTER_PAGE_TABLE_INDEX];
  memcpy(ttbl1, ttbl1Master, TXM_MASTER_PAGE_TABLE_ENTRIES * sizeof(uint32_t));
  /* Clear level 2 tables. This makes each page as a fault page. */
  ttbl2 = (uint32_t *)_txm_level2_module_page_table[asid * 4];
  memset(ttbl2, 0, TXM_LEVEL_2_PAGE_TABLE_ENTRIES * 4 * sizeof(ULONG));

  /* Get code start and end addresses. */
  codeStart = (uint32_t) module->txm_module_instance_code_start;
  /* Extend end address to end of page (TXM_MODULE_MEMORY_ALIGNMENT-1). */
  codeEnd = ((((uint32_t) module->txm_module_instance_code_end) + TXM_MODULE_MEMORY_ALIGNMENT - 1) & ~((uint32_t) TXM_MODULE_MEMORY_ALIGNMENT - 1)) - 1;

  /* Get relocatable data start and end addresses. */
  dataStart = (uint32_t) module->txm_module_instance_data_start;
  /* Extend end address to end of page (TXM_MODULE_MEMORY_ALIGNMENT-1). */
  dataEnd = ((((uint32_t) module->txm_module_instance_data_end) + TXM_MODULE_MEMORY_ALIGNMENT - 1) & ~((uint32_t) TXM_MODULE_MEMORY_ALIGNMENT - 1)) - 1;

  // Create normal, executable and read-only (read/write in reality for debugging) entries to cover code.
  if (mmu_setupPages(codeStart, codeEnd, ttbl1, Sect_User_Code, Page_L1_4k, Page_4k_User_Code, mmu_canModifyLevel2Table, mmu_allocateLevel2TableInternal, module))
  {
    printf("Cannot setup module code pages.\n");
    assert(0);
  }
  // Create normal, non-executable and read/write entries to cover writable data.
  if (mmu_setupPages(dataStart, dataEnd, ttbl1, Sect_User_Data_RW, Page_L1_4k, Page_4k_User_Data_RW, mmu_canModifyLevel2Table, mmu_allocateLevel2TableInternal, module))
  {
    printf("Cannot setup module data pages.\n");
    assert(0);
  }

  // Ensure the module code has been written to main memory so that the instruction cache sees it.
#if !defined(NO_CACHE_USE)
  L1C_CleanInvalidateDCacheAll();
  L1C_InvalidateICacheAll();
  L1C_InvalidateBTAC();
#endif
}

/*-------------------------------------------------------------------------------
 * From txm_module_manager_external_memory_enable.c.
 *-------------------------------------------------------------------------------*/
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _txm_level2_page_clear                          Cortex-A7/MMU/GNU   */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function clears the first entry in a L2 page table and clears  */
/*    the table entry from the module external page table list.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    module_instance                       Module instance pointer       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    TXM_MODULE_MANAGER_MODULE_UNLOAD                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx      Scott Larson            Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
VOID _txm_level2_page_clear(TXM_MODULE_INSTANCE *module_instance)
{
  UINT i;
  for (i = 0; i < TXM_MODULE_LEVEL2_EXTERNAL_PAGES; i++)
  {
    if (module_instance->txm_external_page_table[i])
    {
      *(ULONG *)module_instance->txm_external_page_table[i] = (ULONG)TX_NULL;
      module_instance->txm_external_page_table[i] = TX_NULL;
    }
  }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _txm_module_manager_external_memory_enable      Cortex-A7/MMU/GNU   */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Scott Larson, Microsoft Corporation                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates an entry in the MMU table for a shared        */
/*    memory space.                                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    module                                Module instance pointer       */
/*    start_address                         Start address of memory       */
/*    length                                Length of external memory     */
/*    attributes                            Memory attributes             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tx_mutex_get                         Get protection mutex          */
/*    _tx_mutex_put                         Release protection mutex      */
/*    TX_MEMSET                             Fill memory with constant     */
/*    _txm_level2_page_get                  Get L2 page table             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx      Scott Larson            Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
UINT _txm_module_manager_external_memory_enable(TXM_MODULE_INSTANCE *module,
    VOID *start_address,
    ULONG length,
    UINT attributes)
{
  mmu_region_attributes_Type region;
  uint32_t *ttbl1, sectionDesc, pageDesc;
  uint32_t areaStart = (uint32_t)start_address, areaEnd = (uint32_t)(start_address + length);
  ULONG asid;

  /* Determine if the module manager has not been initialized yet.  */
  if (_txm_module_manager_ready != TX_TRUE)
  {
    /* Module manager has not been initialized.  */
    return(TX_NOT_AVAILABLE);
  }

  /* Determine if the module is valid. */
  if (module == TX_NULL)
  {
    /* Invalid module pointer. */
    return(TX_PTR_ERROR);
  }

  /* Get module manager protection mutex. */
  _tx_mutex_get(&_txm_module_manager_mutex, TX_WAIT_FOREVER);

  /* Determine if the module instance is valid.  */
  if (module -> txm_module_instance_id != TXM_MODULE_ID)
  {
    /* Release the protection mutex. */
    _tx_mutex_put(&_txm_module_manager_mutex);

    /* Invalid module pointer. */
    return(TX_PTR_ERROR);
  }

  /* Determine if the module instance is in the loaded state. */
  if (module -> txm_module_instance_state != TXM_MODULE_LOADED)
  {
    /* Release the protection mutex. */
    _tx_mutex_put(&_txm_module_manager_mutex);

    /* Return error if the module is not ready. */
    return(TX_START_ERROR);
  }

  /* Determine if the module instance is memory protected. */
  if (module -> txm_module_instance_asid == 0)
  {
    /* Release the protection mutex. */
    _tx_mutex_put(&_txm_module_manager_mutex);

    /* Return error if the module is not protected. */
    return(TXM_MODULE_EXTERNAL_MEMORY_ENABLE_ERROR);
  }

  /* Start address must be aligned to MMU block size (4 kB).
     Length will be rounded up to 4 kB alignment. */
  if (areaStart & (TXM_MODULE_MEMORY_ALIGNMENT - 1))
  {
    /* Release the protection mutex. */
    _tx_mutex_put(&_txm_module_manager_mutex);

    /* Return alignment error. */
    return(TXM_MODULE_ALIGNMENT_ERROR);
  }

  asid = module -> txm_module_instance_asid;

  /* Page tables. */
  ttbl1 = (uint32_t *)_txm_ttbr1_page_table[asid];

  /* Create page entries. */
  if (attributes == mmu_peripheral)
  {
    memset(&region, 0, sizeof(region));
    region.mem_t = STRONGLY_ORDERED;
    region.domain = 0;
    region.inner_norm_t = NON_CACHEABLE;
    region.outer_norm_t = NON_CACHEABLE;
    region.e_t = ECC_DISABLED;
    region.xn_t = NON_EXECUTE;
    region.g_t = NON_GLOBAL;
    region.sec_t = SECURE;
    region.priv_t = RW;
    region.user_t = RW;
    region.sh_t = SHARED; // shared ? SHARED : NON_SHARED;

    region.rg_t = SECTION;
    MMU_GetSectionDescriptor(&sectionDesc, region);
    region.rg_t = PAGE_4k;
    MMU_GetPageDescriptor(&Page_L1_4k, &pageDesc, region);
  }
  else
  {
    sectionDesc = attributes == mmu_readWrite ? Sect_User_Data_RW : Sect_User_Data_R;
    pageDesc = attributes == mmu_readWrite ? Page_4k_User_Data_RW : Page_4k_User_Data_R;
  }
  if (mmu_setupPages(areaStart, areaEnd, ttbl1, sectionDesc, Page_L1_4k, pageDesc, mmu_canModifyLevel2Table, mmu_allocateLevel2TableExternal, module))
  {
    /* Release the protection mutex. */
    _tx_mutex_put(&_txm_module_manager_mutex);

    /* Return external memory enable. */
    return(TXM_MODULE_EXTERNAL_MEMORY_ENABLE_ERROR);
  }

  L1C_CleanInvalidateDCacheAll();
  L1C_InvalidateICacheAll();
  L1C_InvalidateBTAC();
  MMU_InvalidateTLB();

  /* Release the protection mutex. */
  _tx_mutex_put(&_txm_module_manager_mutex);

  return(TX_SUCCESS);
}

#endif
