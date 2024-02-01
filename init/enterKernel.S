# @file enterKernel.S
# @author Diogo Gomes
# @brief Setup GDT, jump to kernel
# @version 0.1
# @date 2024-02-01

.code32
.section .text
.global enterKernel
.type enterKernel, @function

enterKernel:
    # Get arguments
    pop %eax # Discard, return value not needed
    pop %edx # Kentry
    pop %esi # Multiboot info

    # Start by setting up a 64-bit GDT
    lgdt (gdt_desc)

    # Activate it
    ljmp $CODE_SEGMENT, $enter_64

enter_64:
    # First order of business, reload the data segments
    mov $DATA_SEGMENT, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov %ax, %fs
    mov %ax, %gs

    # Jump to kernel!
    jmp *%edx

# Should never get here
    cli
1:  hlt
    jmp 1b

.code32
.section .rodata
.align 4096


# Null Descriptor
gdt_start:
gdt_null:
   .word 0x0000, 0x0000
   .byte 0x00, 0b00000000, 0b00000000, 0x00
   # Code Descriptor
gdt_code:
   .word 0xffff, 0x0000
   .byte 0x00, 0x9A, 0b00101111, 0x00
   # Data Descriptor
gdt_data:
   .word 0xffff, 0x0000
   .byte 0x00, 0x92, 0b00101111, 0x00
gdt_end:

.equ GDT_START, gdt_start # This is necessary, cause for some reason just doing
                          # .long gdt_start doesn't store the right address, and
                          # I can't figure out why, but this hack works

gdt_desc:
    .word gdt_end - gdt_start - 1
    .long GDT_START

.equ CODE_SEGMENT, gdt_code - gdt_start
.equ DATA_SEGMENT, gdt_data - gdt_start

