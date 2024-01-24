/**
 * @file init/main.cpp
 * @author Diogo Gomes
 * @brief 32-bit protected mode loader, takes the kernel.bin
 * that was loaded as a modules, elf-loads it to the correct
 * location, and returns
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <sys/multiboot.h>
#include <sys/elf.h>
#include <stdint.h>
#include <stdbool.h>
#include "init_common.hpp"
#include <klib/io.hpp>
#include <devices/framebuffer_io.hpp>

extern "C" {
    void* lmain( uint32_t multiboot_flag, const struct multiboot_info_structure* info );
    [[noreturn,maybe_unused]] void error(const char* errStr, initTerminal& terminal);
    [[noreturn,maybe_unused]] void hang();
    
}

//static framebuffer_io::framebuffer_terminal terminal;
static io::_outstream<framebuffer_io::framebuffer_terminal> icout;

void* lmain( uint32_t multiboot_flag, const struct multiboot_info_structure* info )
{
    framebuffer_io::framebuffer_terminal aaaa;
    icout.init(&aaaa);
    icout.clear();
    icout << "HEllo!" << "\nYooSOMWTHING\n";
    //aaaa.setColor(framebuffer_io::framebuffer_terminal::vga_color::VGA_COLOR_BLUE,framebuffer_io::framebuffer_terminal::vga_color::VGA_COLOR_LIGHT_GREEN);
    icout.writeInt(int64_t(43));
    //icout.writeString("Something\n");
    int64_t a= 32;
    aaaa.scroll(2);
    //framebuffer_io::framebuffer_terminal tt = icout.getBackEnd();
    //tt.framebuffer_terminal();
    //tt.clear();

    hang();

    framebuffer_io::framebuffer_terminal term;
    //term.init();
    //term.clear();

    io::_outstream<framebuffer_io::framebuffer_terminal> testcout;
    //testcout.init();
    testcout.clear();
    term.putchar('p');
    term.putchar('\n');
    term.putchar('e');
    term.scroll(1);
    testcout.writeString("Hello beautiful people\n");
    testcout.writeString("Yooooooo");
    testcout << "yassssss";
    icout << "Fodasse\n";
    //testcout << "Hello";


    /*tt.putchar('a');
    tt.putchar('\n');
    tt.putchar('d');
    tt.putchar('b');
    tt.putchar('z');*/
    //tt.scroll(1);
    //tt.putchar('a');
    //icout.writeString("Something");
    //icout.writeInt(a);
    //icout << "Something ashgdjvhb";
    //icout << "This is an int: " << a << "\nSee?";
    hang();
    /*initTerminal terminal;
    terminal.clear();
    /*terminal.setColor(initTerminal::vga_color::VGA_COLOR_LIGHT_GREY,
                      initTerminal::vga_color::VGA_COLOR_BLUE);
    terminal.writeString("Entering PatroclusOS 32-bit protected mode init.bin\n");
    terminal.setColor(initTerminal::vga_color::VGA_COLOR_LIGHT_GREY,
                      initTerminal::vga_color::VGA_COLOR_BLACK);*/

    // First of all, check multiboot_flag
    /*if (multiboot_flag != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        error("Error: Wasn't loaded by multiboot, aborting!\n",terminal);
    }
    
    // FIXME Setup GDT

    // Start searching for modules
    uint32_t mb_flags = info->flags;

    if (!(mb_flags & MULTIBOOT_INFO_MODS))
    {
        error("Error: No multiboot modules!",terminal);
    }

    uint32_t mods_count = info->mods_count;
    if (mods_count != 1) // We expect 1, and only 1, module
    {
        error("Error: Incorrect number of modules, we only expect 1!",terminal);
    }
    multiboot_module_t* mod =
            reinterpret_cast<multiboot_module_t*>(info->mods_addr);

    void* mods_addr = reinterpret_cast<void*>(mod->mod_start);
        
   // TODO parse the command line, perhaps
   // Need a strcmp implementation

    terminal.writeString("kernel.bin is at 0x");
    terminal.writeHex((uint32_t)mods_addr);
    terminal.writeString("\n");

    // Do the ELF loading
    elf64Loader elfLoader(reinterpret_cast<elf64_Ehdr*>(mods_addr));

    if (!elfLoader.checkHeader())
    {
        error("Error: ELF64 header is invalid!",terminal);
    }

    if (!elfLoader.elf_load(terminal))
    {
        error("Error: Couldn't load ELF64 binary kernel.bin",terminal);
    }

    return elfLoader.getEntryPoint();
    
    // Test error
    error("Test error",terminal);

    terminal.writeString("Test hang");
    hang();*/
}

[[noreturn,maybe_unused]] void hang()
{
    while(true)
    __asm__ __volatile__ (  "cli\r\n"
                            "hlt");
}

[[noreturn,maybe_unused]] void error(const char* str, initTerminal& terminal)
{
    terminal.setColor(initTerminal::vga_color::VGA_COLOR_LIGHT_GREY,
                        initTerminal::vga_color::VGA_COLOR_RED);
    terminal.writeString(str);
    hang();
}