
#include <stdint.h>
#include <stddef.h>
#include <kernel/kernel.hpp>
#include <sys/multiboot.h>
#include <klib/io.hpp>
#include <devices/BIOSVideoIO.hpp>
#include <klib/cstdlib.hpp>
#include <kernel/interrupts.hpp>


io::_outstream<io::framebuffer_terminal> out;


void kmain(uint64_t multibootInfo,uint64_t terminalIndex) {
    // Initialize terminal
    io::framebuffer_terminal initTerminal;
    out.init(&initTerminal);
    uint8_t _row = uint8_t(terminalIndex / initTerminal.vga_width);
    uint8_t _column = uint8_t(terminalIndex - _row * initTerminal.vga_width);
    initTerminal.setCursor(_row,_column);

    out << "Entering PatroclusOS 64-bit long mode...\n";
    out << "Row is " << _row << ", column is " << _column << "\n";

    out << "Attempting interrupts...\n";

    sys::setupIDT();
    sys::enableInterrupts(); // TODO need to enable PIC, otherwise we'll immediately double fault

    __asm__ __volatile__ ("int3\ncli");

    //while(true) int a = 1;

    out << "It worked\n";

    abort();
}
