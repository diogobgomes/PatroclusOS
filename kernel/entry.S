# @file boot.S
# @author Diogo Gomes
# @brief Small assembly stub that serves as the entry to the kernel, and whose
# purpose is to simply jump to high virtual memory
# @version 0.1
# @date 2024-02-07

.code64

.section .text
.global _entry_low
.type _entry_low, @function

_entry_low:
    mov $_start, %rax
    jmp *%rax

    cli
1:  hlt
    jmp 1b