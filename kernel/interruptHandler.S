# @file interruptHandler.S
# @author Diogo Gomes
# @brief Code to handle interrupts. It's just a macro that calls proper C++
# functions to handle whatever was needed
# @version 0.1
# @date 2024-02-28

.macro SAVE_CONTEXT
    push %rbp
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx
    push %rax
    #sub $256, %rsp
    #movdqu %xmm15, (%rsp)
    #movdqu %xmm14, 16(%rsp)
    #movdqu %xmm13, 32(%rsp)
    #movdqu %xmm12, 48(%rsp)
    #movdqu %xmm11, 64(%rsp)
    #movdqu %xmm10, 80(%rsp)
    #movdqu %xmm9, 96(%rsp)
    #movdqu %xmm8, 112(%rsp)
    #movdqu %xmm7, 128(%rsp)
    #movdqu %xmm6, 144(%rsp)
    #movdqu %xmm5, 160(%rsp)
    #movdqu %xmm4, 176(%rsp)
    #movdqu %xmm3, 192(%rsp)
    #movdqu %xmm2, 208(%rsp)
    #movdqu %xmm1, 224(%rsp)
    #movdqu %xmm0, 240(%rsp)
.endm

.macro RESTORE_CONTEXT
    #movdqu (%rsp), %xmm15
    #movdqu 16(%rsp), %xmm14
    #movdqu 32(%rsp), %xmm13
    #movdqu 48(%rsp), %xmm12
    #movdqu 64(%rsp), %xmm11
    #movdqu 80(%rsp), %xmm10
    #movdqu 96(%rsp), %xmm9
    #movdqu 112(%rsp), %xmm8
    #movdqu 128(%rsp), %xmm7
    #movdqu 144(%rsp), %xmm6
    #movdqu 160(%rsp), %xmm5
    #movdqu 176(%rsp), %xmm4
    #movdqu 192(%rsp), %xmm3
    #movdqu 208(%rsp), %xmm2
    #movdqu 224(%rsp), %xmm1
    #movdqu 240(%rsp), %xmm0
    #add $256, %rsp
    pop %rax
    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15
    pop %rbp
.endm

.macro CREATE_HANDLER_NO_ERR number
.global _handler\number
.align 16
_handler\number:
    # Push the interrupt numnber
    xchgw %bx, %bx
    pushq $\number
    jmp _handler_stub_no_err
.endm

.macro CREATE_HANDLER_ERR number
.global _handler\number
.align 16
_handler\number:
    # Error code was pushed to the stack, just push number
    xchgw %bx, %bx
    pushq $\number
    jmp _handler_stub_err
.endm

_handler_stub_err:
    SAVE_CONTEXT

    # Fetch interrupt number, error code
    # 16*8 = size pushed to stack = 384
    mov 120(%rsp), %rdi # Int number
    mov 128(%rsp), %rsi # Error code (136 = 128 + 8)

    call interruptHandler

    RESTORE_CONTEXT

    add $16, %rsp # Added by the _handler\number code

    iretq

_handler_stub_no_err:
    SAVE_CONTEXT

    # Fetch interrupt number
    # 16*8 = size pushed to stack = 128
    mov 120(%rsp), %rdi # Int number
    #mov $0, %rsi # Dummy error code


    call interruptHandler

    xchgw %bx, %bx

    RESTORE_CONTEXT

    add $8, %rsp # Added by the _handler\number code

    iretq

    
# Create all necessary interrupts
CREATE_HANDLER_NO_ERR   0 # Division Error
CREATE_HANDLER_NO_ERR   1 # Debug
CREATE_HANDLER_NO_ERR   2 # Non-maskable interrupt
CREATE_HANDLER_NO_ERR   3 # Breakpoint
CREATE_HANDLER_NO_ERR   4 # Overflow
CREATE_HANDLER_NO_ERR   5 # Bound range exceeded
CREATE_HANDLER_NO_ERR   6 # Invalid opcode
CREATE_HANDLER_NO_ERR   7 # Device not available
CREATE_HANDLER_ERR      8 # Double Fault
CREATE_HANDLER_NO_ERR   9 # Not used
CREATE_HANDLER_ERR      10 # Invalid TSS
CREATE_HANDLER_ERR      11 # Segment not present
CREATE_HANDLER_ERR      12 # Stack-segment fault
CREATE_HANDLER_ERR      13 # General protection fault
CREATE_HANDLER_ERR      14 # Page fault
CREATE_HANDLER_NO_ERR   15 # Reserved
CREATE_HANDLER_NO_ERR   16 # x87 Floating point exception
CREATE_HANDLER_ERR      17 # Alignement check
CREATE_HANDLER_NO_ERR   18 # Machine check
CREATE_HANDLER_NO_ERR   19 # SIMD Floating-Point exception
CREATE_HANDLER_NO_ERR   20 # Virtualization exception
CREATE_HANDLER_ERR      21 # Control protection exception
CREATE_HANDLER_NO_ERR   22 # Reserved
CREATE_HANDLER_NO_ERR   23
CREATE_HANDLER_NO_ERR   24
CREATE_HANDLER_NO_ERR   25
CREATE_HANDLER_NO_ERR   26
CREATE_HANDLER_NO_ERR   27
CREATE_HANDLER_NO_ERR   28 # Hypervisor Injection Exception
CREATE_HANDLER_ERR      29 # VMM comunication exception
CREATE_HANDLER_ERR      30 # Security Exception
CREATE_HANDLER_NO_ERR   31 # Reserved
