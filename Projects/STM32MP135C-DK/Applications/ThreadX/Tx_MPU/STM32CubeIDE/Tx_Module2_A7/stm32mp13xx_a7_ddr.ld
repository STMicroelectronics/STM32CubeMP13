MEMORY {
  SYSRAM_BASE (rwx)   : ORIGIN = 0x2FFE0000, LENGTH = 128K
  SRAM1_BASE (rwx)    : ORIGIN = 0x30000000, LENGTH = 16K
  SRAM2_BASE (rwx)    : ORIGIN = 0x30004000, LENGTH = 8K
  SRAM3_BASE (rwx)    : ORIGIN = 0x30006000, LENGTH = 8K
  DDR_BASE (rwx)      : ORIGIN = 0xC0000000, LENGTH = 512M
}

SECTIONS
{
  __file_start__ = 0x00000000;
  
	. = __file_start__;

  /******************************************/
  /* Start of code sections. */
  /******************************************/
  __code_start__ = .;

  /* The preamble must be at the start of the file.  */
  .preamble . :
  {
    __preamble_start__ = .;
    *preamble*
    __preamble_end__ = .;
  }

  /**********************/
  /* Executable section. */
  /**********************/
  __executable_start__ = .;

  .text ALIGN(4) :
  {
    __text_start__ = .;

    *(.text .text.* .gnu.linkonce.t.*)

    KEEP (*(.init))
    KEEP (*(.fini))
    
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)
    
    __text_end__ = .;
  }

  __executable_end__ = .;

  /**********************/
  /* Read only data sections. */
  /**********************/
  __read_only_data_start__ = .;

  .rodata ALIGN(4) :
  {
    __rodata_start__ = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
    . = ALIGN(4);
    __rodata_end__ = .;
  }
 
  .dynsym ALIGN(4) :
  {
    __dynsym_start__ = .;
    KEEP (*(.dynsym))
    KEEP (*(.dynsym*))
    . = ALIGN(4);
    __dynsym_end__ = .;
  }

  .dynstr ALIGN(4) :
  {
    __dynstr_load_start__ = .;
    KEEP (*(.dynstr))
    KEEP (*(.dynstr*))
    . = ALIGN(4);
    __dynstr_end__ = .;
  }

  .rel.dyn ALIGN(4) :
  {
    __reldyn_load_start__ = .;
    KEEP (*(.rel.dyn))
    KEEP (*(.rel.dyn*))
    . = ALIGN(4);
    __reldyn_end__ = .;
  }

  .rel.plt ALIGN(4) :
  {
    __relplt_start__ = .;
    KEEP (*(.rel.plt))
    KEEP (*(.rel.plt*))
    . = ALIGN(4);
    __relplt_end__ = .;
  }

  .plt ALIGN(4) :
  {
    __plt_start__ = .;
    KEEP (*(.plt))
    KEEP (*(.plt*))
    . = ALIGN(4);
    __plt_end__ = .;
  }

  .interp ALIGN(4) :
  {
    __interp_start__ = .;
    KEEP (*(.interp))
    KEEP (*(.interp*))
    . = ALIGN(4);
    __interp_end__ = .;
  }

  .hash ALIGN(4) :
  {
    __hash_start__ = .;
    KEEP (*(.hash))
    KEEP (*(.hash*))
    . = ALIGN(4);
    __hash_end__ = .;
  }

  __read_only_data_end__ = .;

  /******************************************/
  /* Start of relocated sections. */
  /******************************************/
  __relocated_data_start__ = .;

  /**********************/
  /* Initialized data sections. */
  /**********************/
  __initialized_data_start__ = .;

  /* .got is necessary for relocatable code. */
  .got ALIGN(4) :
  {
    __got_start__ = .;
    KEEP (*(.got))
    KEEP (*(.got*))
    . = ALIGN(4);
    __got_end__ = .;
  } 

  .data ALIGN(4) :
  {
    __data_start__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
    __data_end__ = .;
  }

  .preinit_array ALIGN(4) :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  }

  .init_array ALIGN(4) :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  }

  .fini_array ALIGN(4) :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  }

  .vtable ALIGN(4) :
  {
    __vtable_start__ = .;
    *(vtable)
    __vtable_end__ = .;
  }

  __initialized_data_end__ = .;

  __code_end__ = .;
  /******************************************/
  /* End of code sections. */
  /******************************************/

  /**********************/
  /* Uninitialized data sections. */
  /**********************/
  __uninitialized_data_start__ = .;

  .bss ALIGN(4) (NOLOAD) :
  {
    __bss_start__ = .;
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  }
  
  __uninitialized_data_end__ = .;

  __relocated_data_end__ = .;
  /******************************************/
  /* End of relocated sections. */
  /******************************************/

  __file_end__ = .;
  
  /* Code size contains all non writable data. */
  __code_size__ = __code_end__ - __code_start__;
  /* Data size contains all writable data. */
  __data_size__ = __relocated_data_end__ - __relocated_data_start__;
}
