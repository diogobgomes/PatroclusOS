ENTRY(_entry_low)
OUTPUT_FORMAT("elf64-x86-64")

KERNEL_VMA = 0xFFFFFFFF80000000;
/*KERNEL_VMA = 0x0000000080000000;*/
/*KERNEL_VMA = 3M;*/
KERNEL_PHYS = 4M;
BOOTSTRAP_PHYS = 4092K; /*One page for bootstrap*/

/* Tell linker where to put shit */
SECTIONS
{
    /* Where to begin putting sections */
    /*. = KERNEL_PHYS;
    _start = .;

    . += KERNEL_VMA;*/
    . = BOOTSTRAP_PHYS;

    .bootstrap : 
    {
        entry.o (.text)
    }

    . = KERNEL_VMA;

    .text : AT(ADDR(.text) - KERNEL_VMA + KERNEL_PHYS)
    {
        _code_ = .;
        *(EXCLUDE_FILE(entry.o) .text)
        *(.text)
        *(.rodata*)
        . = ALIGN(4096);
    }

    .eh_frame : AT(ADDR(.eh_frame) - KERNEL_VMA + KERNEL_PHYS)
    {
        _ehframe = .;
        *(.eh_frame)
        . = ALIGN(4096);
    }

    .bss : AT(ADDR(.bss) - KERNEL_VMA + KERNEL_PHYS)
    {
        _bss = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4096);
    }

    _end = .;

    /DISCARD/ :
    {
        *(.comment)
    }
}