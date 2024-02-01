# @file boot.S
# @author Diogo Gomes
# @brief Small assembly stub that stis in a FAT32 partition,
# and whose purpose is to set up the CPU to a known state,
# and jump to the main kernel function
# @version 0.1
# @date 2023-09-01


# Reserve a small stack for early use, maybe later we'll set up
# a proper one
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# Kernel entry point
.section .text
.global _start
.type _start, @function
.code64
_start:
    # Setup stack
    mov $stack_top, %rbx
    xchgw %bx, %bx # FIXME Remove from final
    mov %rbx, %rsp
    mov %rbx, %rbp

    mov $0xb8000, %rdi
    mov $0x1F201F201F201F20, %rax
    mov $500, %ecx
    rep stosq

    # FIXME Remove from final
    xchgw %bx, %bx

    call kmain

hang:
    cli
1:  hlt
    jmp 1b

.size _start, . - _start
