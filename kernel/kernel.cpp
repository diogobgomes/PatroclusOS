//#include <klib/basic_io.h>
#include <stdint.h>
#include <stddef.h>
#include <kernel/kernel.hpp>
//#include <klib/basic_cpp_io.hpp>
#include <sys/multiboot.h>
#include <klib/io.hpp>
#include <devices/framebuffer_io.hpp>
#include <klib/basic_io.h>

io::_outstream<framebuffer_io::framebuffer_terminal> icout;

void kmain(uint32_t multibootMagic, multiboot_info_structure* multibootInfo) {
    return;
}
