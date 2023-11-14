
    .syntax unified
#if defined(ARM_MODE)
    .arm
#else
    .thumb
#endif

    .text
    .align 4

#if !defined(ARM_MODE)
    .thumb_func
#endif
    .global _gcc_setup
_gcc_setup:

    push    {r4-r6}

    ldr     r3, =__code_start__ /* r3 = __code_start__ */
    ldr     r4, =__relocated_data_start__ /* r4 = __relocated_data_start__ */
    mov     r5, r0 /* r5 = codeAddress */

    /* Relocate GOT table. */
    ldr     r0, =__got_start__ /* r0 = __got_start__ */
    sub     r0, r3 /* r0 = gotCodeOffset  = __got_start__ - __code_start__ */
    add     r0, r5 /* r0 = gotCodeAddress = gotCodeOffset + codeAddress */
    ldr     r1, =__got_start__ /* r1 = __got_start__ */
    sub     r1, r4 /* r1 = gotRelocatedStartOffset  = __got_start__ - __relocated_data_start__ */
    add     r1, r9 /* r1 = gotRelocatedStartAddress = gotRelocatedStartOffset + sb */
    ldr     r2, =__got_end__ /* r2 = __got_end__ */
    sub     r2, r4 /* r2 = gotRelocatedEndOffset  = __got_end__ - __relocated_data_start__ */
    add     r2, r9 /* r2 = gotRelocatedEndAddress = gotRelocatedEndOffset + sb */
relocate_got_loop:
    cmp     r1, r2 /* if (gotRelocatedStartAddress == gotRelocatedEndAddress) */
    beq     relocate_got_done
    ldr     r6, [r0] /* r6 = gotEntry = *gotCodeAddress */
    cmp     r6, #0 /* if (gotEntry == 0) */
    beq     relocate_got_address_built
    cmp     r6, r4 /* if (gotEntry < __relocated_data_start__) */
    blt     relocate_got_code_area
    sub     r6, r4 /* gotEntry -= __relocated_data_start__ */
    add     r6, r9 /* gotEntry += sb */
    b       relocate_got_address_built
relocate_got_code_area:
    sub     r6, r3 /* gotEntry -= __code_start__ */
    add     r6, r5 /* gotEntry += codeAddress */
relocate_got_address_built:
    str     r6, [r1] /* *gotRelocatedStartAddress = gotEntry */
    add     r0, #4 /* gotCodeAddress += 4 */
    add     r1, #4 /* gotRelocatedStartAddress += 4 */
    b       relocate_got_loop
relocate_got_done:

    /* Copy initialized writable data. */
    ldr     r0, =__data_start__ /* r0 = __data_start__ */
    sub     r0, r3 /* r0 = dataCodeOffset  = __data_start__ - __code_start__ */
    add     r0, r5 /* r0 = dataCodeAddress = dataCodeOffset + codeAddress */
    ldr     r1, =__data_start__ /* r1 = __data_start__ */
    sub     r1, r4 /* r1 = dataRelocatedStartOffset  = __data_start__ - __relocated_data_start__ */
    add     r1, r9 /* r1 = dataRelocatedStartAddress = dataRelocatedStartOffset + sb */
    ldr     r2, =__data_end__ /* r2 = __data_end__ */
    sub     r2, r4 /* r2 = dataRelocatedEndOffset  = __data_end__ - __relocated_data_start__ */
    add     r2, r9 /* r2 = dataRelocatedEndAddress = dataRelocatedEndOffset + sb */
copy_data_loop:
    cmp     r1, r2 /* if (dataRelocatedStartAddress == dataRelocatedEndAddress) */
    beq     copy_data_done
    ldrb    r6, [r0] /* r6 = data = *dataCodeAddress */
    add     r0, #1 /* dataCodeAddress += 1 */
    strb    r6, [r1] /* *dataRelocatedStartAddress = data */
    add     r1, #1 /* dataRelocatedStartAddress += 1 */
    b       copy_data_loop
copy_data_done:

    /* Zero bss. */
    ldr     r1, =__bss_start__ /* r1 = __bss_start__ */
    sub     r1, r4 /* r1 = bssStartOffset  = __bss_start__ - __relocated_data_start__ */
    add     r1, r9 /* r1 = bssStartAddress = bssStartOffset + sb */
    ldr     r2, =__bss_end__ /* r2 = __bss_end__ */
    sub     r2, r4 /* r2 = bssEndOffset  = __bss_end__ - __relocated_data_start__ */
    add     r2, r9 /* r2 = bssEndAddress = bssEndOffset + sb */
    mov     r6, #0
zero_bss_loop:
    cmp     r1, r2 /* if (bssStartAddress == bssEndAddress) */
    beq     zero_bss_done
    strb    r6, [r1] /* *bssStartAddress = data */
    add     r1, #1 /* bssStartAddress += 1 */
    b       zero_bss_loop
zero_bss_done:

    pop     {r4-r6}
    bx      lr

    .align 4

    /* Setup attributes of heap section so it doesn't take up room in the elf file */
    .section .heap, "wa", %nobits
    .section .stack, "wa", %nobits
    .section .stack_process, "wa", %nobits
