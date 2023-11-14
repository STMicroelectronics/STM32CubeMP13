/**
  ******************************************************************************
  * @file    mmu_stm32mp13xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-A7 Device Peripheral Access Layer MMU Setup source file.
  *
  *   This file provides one function called from SystemInit
  *      - MMU_CreateTranslationTable() : This function is called at startup
  *                      during system initialization to setup MMU Translation tables
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

/* Memory map description STM32MP135Cxx
                                                     Memory Type
0xFFFFFFFF |--------------------------| ---------   ------------
           |           DDR            |  1G            Normal
0xC0000000 |--------------------------| ---------   ------------
           |          Fault           | 512M-160k      Fault
0xA0028000 |--------------------------| ---------   ------------
           |           GIC            | 28k            Device
0xA0021000 |--------------------------| ---------   ------------
           |          Fault           | 132k           Fault
0xA0000000 |--------------------------| ---------   ------------
           |          Fault           | 256M           Fault
0x90000000 |--------------------------| ---------   ------------
           |        FMC NAND          | 256M          Device RW
0x80000000 |--------------------------| ---------   ------------
           |       QSPI NAND          | 256M          Device RO
0x70000000 |--------------------------| ---------   ------------
           |        FMC NOR           | 256M          Device RO
0x60000000 |--------------------------| ---------   ------------
           |       Peripherals        | 512M        Device RW/RO
0x40000000 |--------------------------| ---------   ------------
           |          Fault           | 256M-32k       Fault
0x30008000 |--------------------------| ---------   ------------
           |             SRAM         | 32k           Normal
0x30000000 |--------------------------| ---------   ------------
           |    SYSRAM - Stacks       |              Normal RW
           |--------------------------|             ------------
           |    SYSRAM - TTB          |              Normal RW
           |--------------------------|             ------------
           |    SYSRAM - RW ZI Data   |              Normal RW
           |--------------------------| 128k        ------------
           |    SYSRAM - RW Data      |              Normal RW
           |--------------------------|             ------------
           |    SYSRAM - RO Data      |            Normal Exec RO
           |--------------------------|             ------------
           |    SYSRAM - RO Code      |            Normal Exec RO
0x2FFE0000 |--------------------------| ---------   ------------
           |          Fault           | 256M-128k      Fault
0x20000000 |--------------------------| ---------   ------------
           |          Fault           | 512M           Fault
0x00000000 |--------------------------| ---------   ------------
*/

// L1 Cache info and restrictions about architecture of the caches (CCSIR register):
// Write-Through support *not* available
// Write-Back support available.
// Read allocation support available.
// Write allocation support available.

// Note: You should use the Shareable attribute carefully.
// For cores without coherency logic (such as SCU) marking a region as shareable forces the processor to not cache that region regardless of the inner cache settings.
// Cortex-A versions of RTX use LDREX/STREX instructions relying on Local monitors. Local monitors will be used only when the region gets cached, regions that are not cached will use the Global Monitor.
// Some Cortex-A implementations do not include Global Monitors, so wrongly setting the attribute Shareable may cause STREX to fail.
   
// Recall: When the Shareable attribute is applied to a memory region that is not Write-Back, Normal memory, data held in this region is treated as Non-cacheable.
// When SMP bit = 0, Inner WB/WA Cacheable Shareable attributes are treated as Non-cacheable.
// When SMP bit = 1, Inner WB/WA Cacheable Shareable attributes are treated as Cacheable.
   
// Following MMU configuration is expected
// SCTLR.AFE == 1 (Simplified access permissions model - AP[2:1] define access permissions, AP[0] is an access flag)
// SCTLR.TRE == 0 (TEX remap disabled, so memory type and attributes are described directly by bits in the descriptor)
// Domain 0 is always the Client domain
// Descriptors should place all memory in domain 0

#include "stm32mp13xx.h"

//--------------------- TTB sizes  -------------------
#define TTB_L1_SIZE                     0x4000
#define TTB_L2_1M_SIZE                  0x0400

#define PAGE_1MB_SIZE (1 << 20)
#define PAGE_1MB_MASK (PAGE_1MB_SIZE - 1)
#define PAGE_1MB_ALIGN_MASK (~PAGE_1MB_MASK)

//Import symbols from linker
#if defined ( __ICCARM__ )
#pragma section = ".intvec"
#pragma section = "RO_CODE"
#pragma section = "RO_DATA"
#pragma section = "TTB"
#else
extern uint32_t __TEXT_START__;     // Start of code section (RO+Executable)
extern uint32_t __TEXT_END__;       // End of code section (4096 bytes aligned)
extern uint32_t __RO_START__;       // Start of data RO section (RO + Non-Executable)
extern uint32_t __RO_END__;         // End of RO section (4096 bytes aligned)
extern uint32_t TTB;
#endif

// Level 2 table pointers
//-----------------------------------------------------

/* Define global descriptors */
static uint32_t Sect_SO;                // Strongly ordered, shareable by default
static uint32_t Sect_Normal;            // outer & inner wb/wa, non-shareable, executable, rw, domain 0, base addr 0
static uint32_t Sect_Normal_Shared;     // outer & inner wb/wa, shareable, executable, rw, domain 0, base addr 0
static uint32_t Sect_Device_RO;         // device, non-shareable, non-executable, ro, domain 0, base addr 0
static uint32_t Sect_Device_RW;         // as Sect_Device_RO, but writeable
static uint32_t Sect_Device_RW_Shared;  // as Sect_Device_RO, but writeable, shareable

static uint32_t Page_L1_4k  = 0x0;                  // generic
static uint32_t Page_4k_Normal_Cod;                 // outer & inner wb/wa        , shareable, executable,     ro, domain 0
static uint32_t Page_4k_Normal_Cod_NonCacheable;    // outer & inner non-cacheable, shareable, executable,     ro, domain 0
static uint32_t Page_4k_Normal_RO;                  // outer & inner wb/wa        , shareable, non-executable, ro, domain 0
static uint32_t Page_4k_Normal_RO_NonCacheable;     // outer & inner non-cacheable, shareable, non-executable, ro, domain 0
static uint32_t Page_4k_Normal_RW;                  // outer & inner wb/wa        , shareable, non-executable, rw, domain 0
static uint32_t Page_4k_Normal_RW_NonCacheable;     // outer & inner non-cacheable, shareable, non-executable, rw, domain 0
static uint32_t Page_4k_Device_RW;                  // shared device

void MMU_CreateTranslationTable(void)
{
  mmu_region_attributes_Type region;
  uint32_t pageNum;

#if defined ( __ICCARM__ ) //For compatibility with IAR
  uint32_t *text_start_addr = __section_begin(".intvec");
  uint32_t *text_end_addr = __section_end("RO_CODE");
  uint32_t *rodata_start_addr = __section_begin("RO_DATA");
  uint32_t *rodata_end_addr = __section_end("RO_DATA");
  uint32_t *ttb_addr = __section_begin("TTB");
#else
  uint32_t *ttb_addr = &TTB;
  uint32_t *text_start_addr = &__TEXT_START__;
  uint32_t *text_end_addr = &__TEXT_END__;
  uint32_t *rodata_start_addr = &__RO_START__;
  uint32_t *rodata_end_addr = &__RO_END__;
#endif
  volatile uint32_t code_and_data_table_l2_base_4k = ((uint32_t)ttb_addr + TTB_L1_SIZE);
  volatile uint32_t sram_table_l2_base_4k = (code_and_data_table_l2_base_4k + TTB_L2_1M_SIZE);
  volatile uint32_t gic_table_l2_base_4k = (sram_table_l2_base_4k + TTB_L2_1M_SIZE);
  volatile uint32_t sysram_table_l2_base_4k = (gic_table_l2_base_4k + TTB_L2_1M_SIZE);


  // Create faulting entries for full 4GB address space to ensure any access to incorrect memory location will be trapped
  // Further sections and pages setup will overwrite these default table entries
  MMU_TTSection (ttb_addr, 0, 4096U, DESCRIPTOR_FAULT);

  /*
   * Generate descriptors. Refer to core_ca.h to get information about attributes
   *
   */
  // Create descriptors for Vectors, RO, RW, ZI sections
  section_so(Sect_SO, region);
  section_normal(Sect_Normal, region);

  Sect_Normal_Shared = Sect_Normal;
  MMU_SharedSection(&Sect_Normal_Shared, SHARED);

  // Create descriptors for peripherals
  section_device_ro(Sect_Device_RO, region);
  section_device_rw(Sect_Device_RW, region);

  Sect_Device_RW_Shared = Sect_Device_RW;
  MMU_SharedSection(&Sect_Device_RW_Shared, SHARED);

  // Create descriptors for 4k pages
  page4k_device_rw(Page_L1_4k, Page_4k_Device_RW, region);

  /* Common to all L2 pages. */
  region.rg_t = PAGE_4k;
  region.domain = 0x0;
  region.e_t = ECC_DISABLED;
  region.g_t = GLOBAL;
  region.mem_t = NORMAL;
  region.sec_t = SECURE;
  region.sh_t = SHARED;

  /* Code L2 pages. */
  region.inner_norm_t = WB_WA;
  region.outer_norm_t = WB_WA;
  region.xn_t = EXECUTE;
  /* This should be READ but SW4STM32/GDB/OpenOCD is not able to step in a read-only code section ... */
  /* So set it RW */
  region.priv_t = RW;
  region.user_t = RW;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Normal_Cod, region);
  region.inner_norm_t = NON_CACHEABLE;
  region.outer_norm_t = NON_CACHEABLE;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Normal_Cod_NonCacheable, region);

  /* Read only L2 pages. */
  region.inner_norm_t = WB_WA;
  region.outer_norm_t = WB_WA;
  region.xn_t = NON_EXECUTE;
  region.priv_t = READ;
  region.user_t = READ;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Normal_RO, region);
  region.inner_norm_t = NON_CACHEABLE;
  region.outer_norm_t = NON_CACHEABLE;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Normal_RO_NonCacheable, region);

  /* Read write L2 pages. */
  region.inner_norm_t = WB_WA;
  region.outer_norm_t = WB_WA;
  region.xn_t = NON_EXECUTE;
  region.priv_t = RW;
  region.user_t = RW;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Normal_RW, region);
  region.inner_norm_t = NON_CACHEABLE;
  region.outer_norm_t = NON_CACHEABLE;
  MMU_GetPageDescriptor(&Page_L1_4k, &Page_4k_Normal_RW_NonCacheable, region);

  /*
   *  Define MMU flat-map regions and attributes
   *
   */

  // Define 256M FMC NOR memory as device, non-shareable, non-executable, ro
  MMU_TTSection (ttb_addr, (uint32_t)AXI_BUS_MEMORY_BASE, ((QSPI_MEM_BASE - AXI_BUS_MEMORY_BASE) >> 20), Sect_Device_RO);

  // Define 256 QSPI memory as device, non-shareable, non-executable, ro
  MMU_TTSection (ttb_addr, (uint32_t)QSPI_MEM_BASE      , ((FMC_NAND_MEM_BASE - QSPI_MEM_BASE) >> 20)  , Sect_Device_RO);

  // Define 256M FMC NAND memory as device, non-shareable, non-executable, rw
  MMU_TTSection (ttb_addr, (uint32_t)FMC_NAND_MEM_BASE  , 256U                                         , Sect_Device_RW);

  // All DDR (1GB) Executable, Cacheable & RW - applications may choose to divide memory into RO executable
  MMU_TTSection (ttb_addr, (uint32_t)DRAM_MEM_BASE      , 1024U                                        , Sect_Normal_Shared);

  //-------------------- SYSRAM ------------------
  // Create (256 * 4k)=1MB faulting entries to cover SYSRAM 1M aligned range
  MMU_TTPage4k (ttb_addr, SYSRAM_BASE & PAGE_1MB_ALIGN_MASK, 1024U/4U, Page_L1_4k, (uint32_t *)sysram_table_l2_base_4k, DESCRIPTOR_FAULT);
  // Create (32 * 4k)=128k Normal entries to cover full SYSRAM, if the executable is in SYSRAM, part of this table will be overwritten
  MMU_TTPage4k (ttb_addr, SYSRAM_BASE                      , 128U/4U , Page_L1_4k, (uint32_t *)sysram_table_l2_base_4k, Page_4k_Normal_RW_NonCacheable);

  //-------------------- SRAM ------------------
  // Create (256 * 4k)=1MB faulting entries to cover SRAM 1M aligned range
  MMU_TTPage4k (ttb_addr, AHB_SRAM, 1024U/4U, Page_L1_4k, (uint32_t *)sram_table_l2_base_4k, DESCRIPTOR_FAULT);
  // Create (8 * 4k)=32k Normal entries to cover full SRAM
  MMU_TTPage4k (ttb_addr, AHB_SRAM, 32U/4U  , Page_L1_4k, (uint32_t *)sram_table_l2_base_4k, Page_4k_Normal_RW_NonCacheable);

  //--------------------- PERIPHERALS -------------------
  // Define Peripheral memory as Strongly ordered, shareable, RW , Non Cacheable
  MMU_TTSection (ttb_addr, (uint32_t)PERIPH_BASE, ((AXI_BUS_MEMORY_BASE - PERIPH_BASE) >> 20), Sect_SO);

  //-------------------- GIC ------------------
  // Create (256 * 4k)=1MB faulting entries to cover GIC 1M aligned range
  MMU_TTPage4k (ttb_addr, GIC_BASE & PAGE_1MB_ALIGN_MASK, 1024U/4U, Page_L1_4k, (uint32_t *)gic_table_l2_base_4k, DESCRIPTOR_FAULT);
  // Create (7 * 4k)=28k Device entries to cover full GIC
  MMU_TTPage4k (ttb_addr, GIC_BASE                      , 28U/4U  , Page_L1_4k, (uint32_t *)gic_table_l2_base_4k, Page_4k_Device_RW);

  //-------------------- Code and data ------------------
  if ((uint32_t)text_start_addr >= DRAM_MEM_BASE)
  {
    /* Code is in DDR */

    // Ensure Code and RO data can fit in single 1M section because this is the size of Level to code_and_data_table_l2_base_4k Table.
    // If it does not fit in a single 1 MB page, then we do not add specific protection for code and data because we would need one more L2 table to do so.
    // To add this new table, this would require to change linker scripts of all examples.
    // So, if the executable is larger than 1M, its code and read-only areas are both writable and executable like the remaining of the DRAM.
    uint32_t aligned_CodeStart   = (uint32_t)text_start_addr & PAGE_1MB_ALIGN_MASK;
    uint32_t aligned_RO_Data_End = ((uint32_t)rodata_end_addr + PAGE_1MB_MASK) & PAGE_1MB_ALIGN_MASK;
    if (aligned_RO_Data_End - aligned_CodeStart <= PAGE_1MB_SIZE)
    {
      // Create (256 * 4k)=1MB RW entries to cover 1M aligned range
      MMU_TTPage4k (ttb_addr, aligned_CodeStart         , 1024U/4U, Page_L1_4k, (uint32_t *)code_and_data_table_l2_base_4k, Page_4k_Normal_RW);

      // Create Normal, executable+RO entries to cover Code
      pageNum = ((uint32_t)text_end_addr  - (uint32_t)text_start_addr + 4095) / 4096;
      MMU_TTPage4k (ttb_addr, (uint32_t)text_start_addr , pageNum,  Page_L1_4k, (uint32_t *)code_and_data_table_l2_base_4k, Page_4k_Normal_Cod);

      // Create Normal, non-executable+RO entries to cover RO data range
      pageNum = ((uint32_t)rodata_end_addr  - (uint32_t)rodata_start_addr + 4095) / 4096;
      MMU_TTPage4k (ttb_addr, (uint32_t)rodata_start_addr   , pageNum,  Page_L1_4k, (uint32_t *)code_and_data_table_l2_base_4k, Page_4k_Normal_RO);
    }
  }
  else
  {
    /* Code is in SYSRAM */

    // Create (256 * 4k)=1MB faulting entries to cover 1M aligned range (this needs to be done again because this is not the same L2 page table)
    MMU_TTPage4k (ttb_addr, SYSRAM_BASE & PAGE_1MB_ALIGN_MASK, 1024U/4U, Page_L1_4k, (uint32_t *)code_and_data_table_l2_base_4k, DESCRIPTOR_FAULT);

    // Create (32 * 4k)=128k Normal entries to cover full SYSRAM (this needs to be done again because this is not the same L2 page table)
    MMU_TTPage4k (ttb_addr, SYSRAM_BASE                      , 128U/4U,  Page_L1_4k, (uint32_t *)code_and_data_table_l2_base_4k, Page_4k_Normal_RW_NonCacheable);

    // Create Normal, executable+RO entries to cover Code
    pageNum = ((uint32_t)text_end_addr  - (uint32_t)text_start_addr + 4095U) / 4096U;
    MMU_TTPage4k (ttb_addr, (uint32_t)text_start_addr       , pageNum,  Page_L1_4k, (uint32_t *)code_and_data_table_l2_base_4k, Page_4k_Normal_Cod);

    // Create Normal, non-executable+RO entries to cover RO data range
    pageNum = ((uint32_t)rodata_end_addr  - (uint32_t)rodata_start_addr + 4095U) / 4096U;
    MMU_TTPage4k (ttb_addr, (uint32_t)rodata_start_addr          , pageNum,  Page_L1_4k, (uint32_t *)code_and_data_table_l2_base_4k, Page_4k_Normal_RO_NonCacheable);
  }

  /* Set location of level 1 page table
  ; 31:14 - Translation table base addr (31:14-TTBCR.N, TTBCR.N is 0 out of reset)
  ; 13:7  - 0x0
  ; 6     - IRGN[0] 0x0 (Inner WB WA)
  ; 5     - NOS     0x0 (Non-shared)
  ; 4:3   - RGN     0x1 (Outer WB WA)
  ; 2     - IMP     0x0 (Implementation Defined)
  ; 1     - S       0x0 (Non-shared)
  ; 0     - IRGN[1] 0x1 (Inner WB WA) */
  __set_TTBR0(((uint32_t)ttb_addr) | 9U);
  __ISB();

  /* Set up domain access control register
  ; We set domain 0 to Client and all other domains to No Access.
  ; All translation table entries specify domain 0 */
  __set_DACR(1);
  __ISB();
}
