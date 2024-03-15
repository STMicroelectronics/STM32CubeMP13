/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**  Abstract    : Linker script for STM32MP13xx series
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed �as is,� without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

MEMORY {
  SYSRAM_BASE (rwx)   : ORIGIN = 0x2FFE0000, LENGTH = 128K
  SRAM1_BASE (rwx)    : ORIGIN = 0x30000000, LENGTH = 16K
  SRAM2_BASE (rwx)    : ORIGIN = 0x30004000, LENGTH = 8K
  SRAM3_BASE (rwx)    : ORIGIN = 0x30006000, LENGTH = 8K
  DDR_BASE (rwx)      : ORIGIN = 0xC0000000, LENGTH = 512M
}

/* Select Memory in which code has to be placed and offset of code from start of this memory */
REGION_ALIAS("RAM", DDR_BASE);

__MEM_START__ = ORIGIN(RAM);
__MEM_END__ = ORIGIN(RAM) + LENGTH(RAM);
__MEM_SIZE__  = LENGTH(RAM);

PHDRS
{
	text PT_LOAD FLAGS(5);  /* READ and EXECUTE */
	data PT_LOAD FLAGS(6);  /* READ and WRITE */
	bss PT_LOAD FLAGS(6);   /* READ and WRITE */
}

SECTIONS
{
  . = __MEM_START__;

  /*********************************************************************************
   * Text
   * Memory page(s) mapped to this section will be marked as
   * read-only, executable.  No non-executable data from the next section must
   * creep in.
   **********************************************************************************/
  .text . : ALIGN(4096) {
    __TEXT_START__ = .;

    *(.text.Reset_Handler)
    * (RESET)
    *(.text*)

    /* .init is used by libc_nano */
    KEEP (*(.init))
    KEEP (*(.fini))
  
    __TEXT_END_UNALIGNED__ = .;
    . = NEXT(4096);
    __TEXT_END__ = .;
  } :text
  
  /*********************************************************************************
   * Read only data
   * Memory page(s) mapped to this section will be marked as
   * read-only, non-executable.  No RW data from the next section must
   * creep in.  Ensure the rest of the current memory page is unused.
   **********************************************************************************/
  .rodata . : ALIGN(4096) {
    __RO_START__ = .;

    *(.rodata*)

    __RO_END_UNALIGNED__ = .;
    . = NEXT(4096);
    __RO_END__ = .;
  }

  /*********************************************************************************
   * Writable data
   * Memory pages mapped to this section will be marked as
   * read-write, non-executable.
   **********************************************************************************/
  . = ALIGN(4096);
  __RW_START__ = .;

  /*--------------------------------------------------------------------------------
   * Writable initialized data.
   *--------------------------------------------------------------------------------*/
  .ARM.extab . : ALIGN(4) {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  }

  .ARM . : ALIGN(4) {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  }

  /* .init_array is used by libc_nano */

  .preinit_array . : ALIGN(4) {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  }

  .init_array . : ALIGN(4) {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  }

  .fini_array . : ALIGN(4) {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  }

  .data . : ALIGN(4) {
      RW_DATA = .;
      *(.data*)
      __DATA_END__ = .;
    . = ALIGN(4);
  }

  /*--------------------------------------------------------------------------------
   * Writable uninitialized data
   * The .bss section gets initialised to 0 at runtime.
   * Its base address must be 16-byte aligned.
   *--------------------------------------------------------------------------------*/
  .bss . (NOLOAD) : {
      . = ALIGN(32);
      ZI_DATA = .;
      *(SORT_BY_ALIGNMENT(.bss*))
      *(COMMON)
      . = ALIGN(32);
      __BSS_END__ = .;
  }

  __BSS_SIZE__ = SIZEOF(.bss);

  /*--------------------------------------------------------------------------------
   * Stacks
   *--------------------------------------------------------------------------------*/
  __SYS_STACK_SIZE = 0x8000;
  __SVC_STACK_SIZE = 0x4000;
  __FIQ_STACK_SIZE = 0x1000;
  __IRQ_STACK_SIZE = 0x1000;
  __ABT_STACK_SIZE = 0x1000;
  __UND_STACK_SIZE = 0x1000;
  
  .stack . (NOLOAD) : {
    . = ALIGN(32);

    LONG(0xDEADBEEF) /* Canary */
    . += __UND_STACK_SIZE - 4;
    UND_STACK = .;

    LONG(0xDEADBEEF) /* Canary */
    . += __ABT_STACK_SIZE - 4;
    ABT_STACK = .;
    
    LONG(0xDEADBEEF) /* Canary */
    . += __IRQ_STACK_SIZE - 4;
    IRQ_STACK = .;
    
    LONG(0xDEADBEEF) /* Canary */
    . += __FIQ_STACK_SIZE - 4;
    FIQ_STACK = .;

    LONG(0xDEADBEEF) /* Canary */
    . += __SVC_STACK_SIZE - 4;
    SVC_STACK = .;

    LONG(0xDEADBEEF) /* Canary */
    _stack_bottom = .;
    . += __SYS_STACK_SIZE - 4;
    _stack_top = .;
    SYS_STACK = .;

  }

  /*--------------------------------------------------------------------------------
   * MMU page tables
   *--------------------------------------------------------------------------------*/
  .mmu . (NOLOAD) : {
    *(.mmu)
  }

  /*********************************************************************************
   * End
   **********************************************************************************/
  end_unaligned = .;
  . = NEXT(4096);
  __RW_END__ = .;
  _end = .;
  end = .;

  /*********************************************************************************
   * Special buffers
   **********************************************************************************/
  /* SD card buffers are read and written by SDMMC DMA (IDMA), they are put in a non-cacheable area to avoid cache
     maintenance. Also, SDMMC DMA (IDMA) is not securable (this is not clearly specified in the Reference manual
     RM0475 Rev 1), so the buffers must be in a non-secured area. Here SD card buffers are put in SYSRAM, this means
     SYSRAM must be set as non-secure via ETZPC. */
  .sd_buffer (NOLOAD) : 
  {
    *(.SdBufferSection)
  } >SYSRAM_BASE

  /*********************************************************************************
  /* Remove information from the compiler libraries
   **********************************************************************************/
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
}
