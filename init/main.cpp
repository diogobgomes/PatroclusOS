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
#include <klib/string.h>
#include <klib/cstdlib.hpp>
#include <klib/io.hpp>
#include <devices/framebuffer_io.hpp>
#include "elfLoader.hpp"
#include "paging.h"

// Declarations
extern "C" {
    [[noreturn]] void lmain( void* pagingArea, uint32_t multiboot_flag, const struct multiboot_info_structure* info );
    [[noreturn,maybe_unused]] void error(const char* errStr);
    [[noreturn,maybe_unused]] void hang();
}

// Assembly functions
extern "C" uint32_t checkCPUID();
//extern "C" uint32_t setupPaging();
extern "C" [[noreturn]] void enterKernel(void* entry, const struct multiboot_info_structure* multiboot_info);

// Internal functions
// This one checks that we were correctly loaded by multiboot, reads info, and
// returns the multiboot module that's actually the kernel
void* multibootStuff(const struct multiboot_info_structure* info, uint32_t multiboot_flag);
// This one loads the ELF module
void* doElfLoad(void* mods_addr);

//static framebuffer_io::framebuffer_terminal terminal;
io::_outstream<framebuffer_io::framebuffer_terminal> icout;

void lmain( void* pagingArea, uint32_t multiboot_flag, const struct multiboot_info_structure* info )
{
    // Initiate the terminal
    framebuffer_io::framebuffer_terminal initTerminal;
    icout.init(&initTerminal);
    icout.clear();

    // Print header
    initTerminal.setColor(framebuffer_io::framebuffer_terminal::vga_color::VGA_COLOR_LIGHT_GREY,
                            framebuffer_io::framebuffer_terminal::vga_color::VGA_COLOR_BLUE);
    icout << "Entering PatroclusOS 32-bit protected mode init.bin\n";
    initTerminal.setColor(framebuffer_io::framebuffer_terminal::vga_color::VGA_COLOR_LIGHT_GREY,
                            framebuffer_io::framebuffer_terminal::vga_color::VGA_COLOR_BLACK);

    // Get the module address from multiboot_info_structure
    void* mods_addr = multibootStuff(info,multiboot_flag);
    
    // Do the ELF loading
    void* kentry = doElfLoad(mods_addr);

    // Setup long mode
    uint32_t cpuidResult = checkCPUID();

    if (cpuidResult == 1)
    {
        error("Error: CPUID not supported");
    }
    else if (cpuidResult == 2)
    {
        error("Error: Long mode not supported");
    }
    
    // Now, attempting to enable paging
    icout << "Attempting to enable paging:...";

    setupPaging( pagingArea );

    icout << " apparently, it went ok?\n";

    // Paging went ok, so now jump to kernel
    icout << "Jumping to kernel. Entry point: 0x" 
          << reinterpret_cast<uint64_t>(kentry);

    icout << ", mbinfo is at: 0x" << reinterpret_cast<uint64_t>(info) << "\n";

    enterKernel(kentry,info);

    error("\nShouldn't be seeing this!!!");

}

[[noreturn,maybe_unused]] void hang()
{
    while(true)
    __asm__ __volatile__ (  "xchgw %bx, %bx\r\n"
                            "cli\r\n"
                            "hlt");
}

[[noreturn,maybe_unused]] void error(const char* str)
{
    framebuffer_io::framebuffer_terminal* termPtr = icout.getBackEnd();
    termPtr->setColor(framebuffer_io::framebuffer_terminal::vga_color::VGA_COLOR_LIGHT_GREY,
                        framebuffer_io::framebuffer_terminal::vga_color::VGA_COLOR_RED);
    
    icout << str;
    hang();
}

void* multibootStuff(const struct multiboot_info_structure* info, uint32_t multiboot_flag)
{
    // First of all, check multiboot_flag
    if (multiboot_flag != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        error("Error: Wasn't loaded by multiboot, aborting!\n");
    }


    // Start searching for modules
    uint32_t mb_flags = info->flags;

    if (!(mb_flags & MULTIBOOT_INFO_MODS))
    {
        error("Error: No multiboot modules!\n");
    }

    uint32_t mods_count = info->mods_count;
    if (mods_count != 1) // We expect 1, and only 1, module
    {
        error("Error: Incorrect number of modules, we only expect 1!");
    }
    multiboot_module_t* mod = reinterpret_cast<multiboot_module_t*>(info->mods_addr);

    void* mods_addr = reinterpret_cast<void*>(mod->mod_start);

    // TODO parse the command line, need a strcmp implementation

    icout << "kernel.bin is at 0x" << icout.hex() <<
            reinterpret_cast<uint64_t>(mods_addr) << "\n";
    
    return mods_addr;
}

void* doElfLoad(void* mods_addr)
{
    elf64Loader elfLoader(reinterpret_cast<elf64_Ehdr*>(mods_addr));

    if (!elfLoader.checkHeader())
    {
        error("Error: ELF64 header is invalid or we don't support it!");
    }

    if (!elfLoader.elf_load())
    {
        error("Error: Couldn't load ELF64 binary kernel.bin");
    }

    // This is successful
    return elfLoader.getEntryPoint();
}