ENTRY(_start)
OUTPUT_FORMAT("binary")
OUTPUT_ARCH("i386")
phys = 0x3000;

SECTIONS
{

    . = phys;

    .text : {
        *(.sign)
        /*stage1.S.obj(.text);*/
        . = ALIGN(4);
        *(.text)
    }
/*
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }*/

    /*.text   : { *(.sign) *(.text); *(.data); *(.rodata); *(.bss) }*/
    /*.data   : SUBALIGN(2) { *(.data); *(.rodata); }
    .bss    : { *(.bss); }*/
}