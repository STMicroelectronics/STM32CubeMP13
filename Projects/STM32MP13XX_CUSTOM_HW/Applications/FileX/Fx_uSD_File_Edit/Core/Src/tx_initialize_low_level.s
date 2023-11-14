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


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** ThreadX Component                                                     */
/**                                                                       */
/**   Initialize                                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

    .syntax unified
#if defined(ARM_MODE)
    .arm
#else
    .thumb
#endif

  FIQ_MODE        =       0x11                    // Disable IRQ/FIQ FIQ mode
  IRQ_MODE        =       0x12                    // Disable IRQ/FIQ IRQ mode
  SVC_MODE        =       0x13                    // Disable IRQ/FIQ SVC mode
  MON_MODE        =       0x16                    // Disable IRQ/FIQ MON mode
  ABT_MODE        =       0x17                    // Disable IRQ/FIQ ABT mode
  HYP_MODE        =       0x1A                    // Disable IRQ/FIQ HYP mode
  UND_MODE        =       0x1B                    // Disable IRQ/FIQ UND mode
  SYS_MODE        =       0x1F                    // Disable IRQ/FIQ SYS mode

  .global     _tx_thread_system_stack_ptr
  .global     _tx_initialize_unused_memory
  .global     _tx_thread_context_save
  .global     _tx_thread_context_restore
  .global     _stack_bottom
  .global     _end

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _tx_initialize_low_level                             ARMv7-A        */
/*                                                           6.1.11       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Microsoft Corporation                             */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is responsible for any low-level processor            */
/*    initialization, including setting up interrupt vectors, setting     */
/*    up a periodic timer interrupt source, saving the system stack       */
/*    pointer for use in ISR processing later, and finding the first      */
/*    available RAM memory address for tx_application_define.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
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
/*    _tx_initialize_kernel_enter           ThreadX entry function        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     William E. Lamie         Initial Version 6.1           */
/*  04-25-2022     Zhen Kong                Updated comments,             */
/*                                            resulting in version 6.1.11 */
/*                                                                        */
/**************************************************************************/
  .text
  .align 2

#if !defined(ARM_MODE)
  .thumb_func
#endif
  .global _tx_initialize_low_level
  .type   _tx_initialize_low_level,function
_tx_initialize_low_level:

  /* We must be in SYS mode at this point! */

#ifdef TX_ENABLE_FIQ_SUPPORT
  CPSID   if                                  // Disable IRQ and FIQ
#else
  CPSID   i                                   // Disable IRQ
#endif

  /* Save sp and lr to use them in SVC mode */
  MOV     r0, sp
  MOV     r1, lr

#if 1
  /* Optional clearing of registers for easier debugging */
  MOV     r3, #0

  CPS     #IRQ_MODE                           // Enter IRQ mode
  MOV     lr, r3                              // Clear LR for easier debugging
  MSR     SPSR_cxsf, r3                       // Clear SPSR for easier debugging

  CPS     #FIQ_MODE                           // Enter FIQ mode
  MOV     r8, r3                              // Clear R8 for easier debugging
  MOV     r9, r3                              // Clear R9 for easier debugging
  MOV     r10, r3                             // Clear R10 for easier debugging
  MOV     r11, r3                             // Clear R11 for easier debugging
  MOV     r12, r3                             // Clear R12 for easier debugging
  MOV     lr, r3                              // Clear LR for easier debugging
  MSR     SPSR_cxsf, r3                       // Clear SPSR for easier debugging

  CPS     #MON_MODE                           // Enter MON mode
  MOV     lr, r3                              // Clear LR for easier debugging
  MSR     SPSR_cxsf, r3                       // Clear SPSR for easier debugging

  CPS     #ABT_MODE                           // Enter ABT mode
  MOV     lr, r3                              // Clear LR for easier debugging
  MSR     SPSR_cxsf, r3                       // Clear SPSR for easier debugging

  CPS     #UND_MODE                           // Enter UND mode
  MOV     lr, r3                              // Clear LR for easier debugging
  MSR     SPSR_cxsf, r3                       // Clear SPSR for easier debugging
#endif

  CPS     #SVC_MODE                           // Enter SVC mode

  /* Use sp and lr from SYS mode in SVC mode */
  MOV     sp, r0
  MOV     lr, r1

  LDR     r1, =_stack_bottom                  // Get pointer to stack area
  LDR     r2, =_tx_thread_system_stack_ptr    // Pickup stack pointer
  STR     r1, [r2]                            // Save the system stack

  LDR     r1, =_end                           // Get end of non-initialized RAM area
  LDR     r2, =_tx_initialize_unused_memory   // Pickup unused memory ptr address
  ADD     r1, r1, #8                          // Increment to next free word
  STR     r1, [r2]                            // Save first free memory address

  MOV     pc, lr                              // Return to caller

/**************************************************************************/
/* Undefined exception */
/**************************************************************************/
#if !defined(ARM_MODE)
  .thumb_func
#endif
    .global __tx_undefined
__tx_undefined:
  BKPT    0x0000
__tx_undefined_loop:
  B       __tx_undefined_loop

/**************************************************************************/
/* Reserved exception */
/**************************************************************************/
#if !defined(ARM_MODE)
  .thumb_func
#endif
    .global __tx_reserved_handler
__tx_reserved_handler:
  BKPT    0x0000
__tx_reserved_handler_loop:
  B       __tx_reserved_handler_loop

/**************************************************************************/
/* IRQ exception */
/**************************************************************************/
#if !defined(ARM_MODE)
  .thumb_func
#endif
  .global __tx_irq_handler
  .global __tx_irq_processing_return
__tx_irq_handler:

  /* Jump to context save to save system context.  */
  B       _tx_thread_context_save
__tx_irq_processing_return:
  /* At this point execution is still in the IRQ mode.  The CPSR, point of
     interrupt, and all C scratch registers are available for use.  In
     addition, IRQ interrupts may be re-enabled - with certain restrictions -
     if nested IRQ interrupts are desired.  Interrupts may be re-enabled over
     small code sequences where lr is saved before enabling interrupts and
     restored after interrupts are again disabled. */

  /* Interrupt nesting is allowed after calling _tx_thread_irq_nesting_start
     from IRQ mode with interrupts disabled.  This routine switches to the
     system mode and returns with IRQ interrupts enabled.

     NOTE:  It is very important to ensure all IRQ interrupts are cleared
     prior to enabling nested IRQ interrupts. */
#ifdef TX_ENABLE_IRQ_NESTING
  BL      _tx_thread_irq_nesting_start
#endif


  /*
   * Handle all IRQ and in particular the one used for Threadx tick:
   * SecurePhysicalTimer_IRQHandler which calls _tx_timer_interrupt function
   */
  BL     IRQ_Handler

  /* If interrupt nesting was started earlier, the end of interrupt nesting
     service must be called before returning to _tx_thread_context_restore.
     This routine returns in processing in IRQ mode with interrupts disabled.  */
#ifdef TX_ENABLE_IRQ_NESTING
  BL      _tx_thread_irq_nesting_end
#endif

  /* Jump to context restore to restore system context.  */
  B       _tx_thread_context_restore

/**************************************************************************/
/* FIQ exception */
/**************************************************************************/
#ifdef TX_ENABLE_FIQ_SUPPORT
#if !defined(ARM_MODE)
  .thumb_func
#endif
  .global  __tx_fiq_handler
  .global  __tx_fiq_processing_return
__tx_fiq_handler:

  /* Jump to fiq context save to save system context.  */
  B       _tx_thread_fiq_context_save
__tx_fiq_processing_return:

  /* At this point execution is still in the FIQ mode.  The CPSR, point of
     interrupt, and all C scratch registers are available for use.  */

  /* Interrupt nesting is allowed after calling _tx_thread_fiq_nesting_start
     from FIQ mode with interrupts disabled.  This routine switches to the
     system mode and returns with FIQ interrupts enabled.

     NOTE:  It is very important to ensure all FIQ interrupts are cleared
     prior to enabling nested FIQ interrupts.  */
#ifdef TX_ENABLE_FIQ_NESTING
  BL      _tx_thread_fiq_nesting_start
#endif

  /* Application FIQ handlers can be called here!  */

  /* If interrupt nesting was started earlier, the end of interrupt nesting
     service must be called before returning to _tx_thread_fiq_context_restore.  */
#ifdef TX_ENABLE_FIQ_NESTING
    BL      _tx_thread_fiq_nesting_end
#endif

  /* Jump to fiq context restore to restore system context.  */
  B       _tx_thread_fiq_context_restore


#else

#if !defined(ARM_MODE)
  .thumb_func
#endif
    .global  __tx_fiq_handler
__tx_fiq_handler:
  BKPT    0x0000
__tx_fiq_handler_loop:
  B       __tx_fiq_handler_loop

#endif

/**************************************************************************/
/* Prefetch exception */
/**************************************************************************/
#if !defined(ARM_MODE)
  .thumb_func
#endif
  .global  __tx_prefetch_handler
__tx_prefetch_handler:
  BKPT    0x0000
__tx_prefetch_handler_loop:
  B       __tx_prefetch_handler_loop

/**************************************************************************/
/* Abort exception */
/**************************************************************************/
#if !defined(ARM_MODE)
  .thumb_func
#endif
  .global  __tx_abort_handler
__tx_abort_handler:
  BKPT    0x0000
__tx_abort_handler_loop:
  B       __tx_abort_handler_loop

/**************************************************************************/
/* Build options and version */
/**************************************************************************/
    /* Reference build options and version ID to ensure they come in.  */

BUILD_OPTIONS:
    .word  _tx_build_options                    // Reference to bring in
VERSION_ID:
    .word  _tx_version_id                       // Reference to bring in
