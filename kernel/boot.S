# @file boot.S
# @author Diogo Gomes
# @brief Small assembly stub that stis in a FAT32 partition,
# and whose purpose is to set up the CPU to a known state,
# and jump to the main kernel function
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
_start:
    # Why not
    mov $0, %ecx
    mov $0, %edx
    mov $0, %edi
    mov $0, %esi

    #sti

    movl $stack_top, %esp
    movl $stack_top, %ebp



    # Call global contructors
    call _init

    mov $0x3142,%eax

    #jmp 1b

    # Transfer control to the main kernel
    call kmain

    # We should never get here, if we unexpectedly return from
    # kernel_main
    cli
1:  hlt
    jmp 1b
    
.size _start, . - _start
