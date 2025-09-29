.global _start
.global irq_handler

.section .text
.align 4

_start:
    // Inicializa stack pointer (stack_top definido na seção .bss)
    adrp x0, stack_top
    add  x0, x0, :lo12:stack_top
    mov  sp, x0

    // Chama main em C
    bl main

1:  b 1b   // loop infinito

irq_handler:
    stp x0, x1, [sp, #-16]!
    stp x2, x3, [sp, #-16]!
    stp x4, x5, [sp, #-16]!
    stp x6, x7, [sp, #-16]!

    bl irq_c_handler

    ldp x6, x7, [sp], #16
    ldp x4, x5, [sp], #16
    ldp x2, x3, [sp], #16
    ldp x0, x1, [sp], #16

    eret

.section .bss
.align 16
stack_bottom:
    .space 0x1000
stack_top: