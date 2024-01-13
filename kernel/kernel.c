#include <klib/basic_io.h>
#include <kernel/kernel.h>

void kmain(void) {
    //asm("mov $0x3142,%eax;");
    //asm("cli;");
    //asm ("hlt;");
    terminal_initialize();
    terminal_clear();
    terminal_write_string("What up?");
}
