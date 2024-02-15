# @file boot.S
# @author Diogo Gomes
# @brief Kernel trampoline code
# @version 0.1
# @date 2023-09-01

.code64

# Reserve a small stack for early use, maybe later we'll set up
# a proper one
.section .bss
.align 16
stack_bottom:
#.skip 16384 # 16 KiB
.skip 64
stack_top:

# Kernel entry point
.section .text
.global _start
.type _start, @function

_start:
    # Setup stack
    mov $stack_top, %rbx
    mov %rbx, %rsp
    mov %rbx, %rbp

    # FIXME Remove from final
    mov $0xb8000, %rdi
    mov $0x1F201F201F201F20, %rax
    mov $500, %ecx
    rep stosq

    call kmain

hang:
    cli
1:  hlt
    jmp 1b

.size _start, . - _start
