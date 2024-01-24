ENTRY(_start)
OUTPUT_FORMAT("elf64-x86-64")

KERNEL_VMA = 0xFFFFFFFF80000000;
KERNEL_PHYS = 3M;

/* Tell linker where to put shit */
SECTIONS
{
    /* Where to begin putting sections */
    . = KERNEL_VMA;

    .text : AT(ADDR(.text) - KERNEL_VMA + KERNEL_PHYS)
    {
        _code_ = .;
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