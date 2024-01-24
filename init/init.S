# @file init.S
# @author Diogo Gomes
# @brief Small assembly stub, whose purpose is to setup stack, and call kloader
# @version 0.1
# @date 2023-09-01

# Multiboot header constants
.set ALIGN, 1<<0 # Align loaded modules on page boundaries
.set MEMINFO, 1<<1 # Ask for memory map
.set FLAGS, ALIGN | MEMINFO # multiboot_header->flags
.set MULTIBOOT_MAGIC, 0x1BADB002 # multiboot_header->magic
.set CHECKSUM, -(MULTIBOOT_MAGIC + FLAGS) # multiboot_header->checksum

# Declare the multiboot header, make sure it's near the
# beggining of the binary
.section .multiboot_header
.align 4
.long MULTIBOOT_MAGIC
.long FLAGS
.long CHECKSUM

# BSS
# Stack, and paging tables
.section .bss
.align 4096
p4_table:
.skip 4096
p3_table:
.skip 4096
p2_table:
.skip 4096
stack_bottom:
.skip 16384 # 16 KiB
stack_top:


.code32
.section .text
.extern lmain
.global _start
.type _start, @function
# Init entry point
_start:
    cli # Just in case they were on

    # Setup stack
    mov $stack_top, %esp
    mov $stack_bottom, %ebp

    # Push variables to the stack
    push %ebx # Multiboot info
    push %eax # Multiboot flag, for checking

    call lmain

    # Should never get here
    cli
1:  hlt
    jmp 1b
