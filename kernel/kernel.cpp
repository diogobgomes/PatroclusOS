//#include <klib/basic_io.h>
#include <stdint.h>
#include <stddef.h>
#include <kernel/kernel.hpp>
//#include <klib/basic_cpp_io.hpp>
#include <sys/multiboot.h>
#include <klib/io.hpp>
#include <devices/BIOSVideoIO.hpp>
//#include <klib/basic_io.h>

io::_outstream<io::framebuffer_terminal> out;

void kmain(uint32_t multibootMagic, multiboot_info_structure* multibootInfo) {
    io::framebuffer_terminal initTerminal;
    out.init(&initTerminal);
    out.clear();
    out << "Entering PatroclusOS 64-bit long mode...\n";
    return;
}
