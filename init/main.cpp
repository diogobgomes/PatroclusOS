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
#include <devices/BIOSVideoIO.hpp>
#include "elfLoader.hpp"
#include "paging.h"

/*******************************************************************************
 * 
 * DECLARATIONS
 * 
*******************************************************************************/
extern "C" {
    [[noreturn]] void lmain( void* pagingArea, uint32_t multiboot_flag,
            const struct multiboot_info_structure* info, uint32_t terminalIndex );
//    [[noreturn,maybe_unused]] void error(const char* errStr);
//    [[noreturn,maybe_unused]] void hang();
    uint32_t checkCPUID();
    [[noreturn]] void enterKernel(void* entry,
        const multiboot_info_structure* multiboot_info, uint32_t terminalIndex);
}

// Variables
io::_outstream<io::framebuffer_terminal> out;

// Internal functions
// This one loads the ELF module
void* doElfLoad(void* mods_addr);
// Check that we've been loaded by a "multiboot" bootloader
void checkMultiboot(uint32_t multiboot_flag);
// Get the module address
void* getMultibootModuleAddr(const multiboot_info_structure* info);

void checkMultiboot(uint32_t multiboot_flag)
{
    if (multiboot_flag != MULTIBOOT_BOOTLOADER_MAGIC &&
            multiboot_flag != MULTIBOOT_CUSTOM_BOOTLOADER_MAGIC)
    {
        earlyPanic("We weren't loaded by a compatible multiboot bootloader, bailing!");
    }
}

void* getMultibootModuleAddr(const multiboot_info_structure* info)
{
    // Start searching for modules
    uint32_t mb_flags = info->flags;

    if (!(mb_flags & MULTIBOOT_INFO_MODS))
    {
        earlyPanic("Whoops, there's no multiboot modules, you sure you configured this correctly?");
    }

    uint32_t mods_count = info->mods_count;
    if (mods_count != 1) // We expect 1 and only one module
    {
        earlyPanic("Hold on there, we only need one module! I don't know how to deal with this number");
    }
    auto mod = reinterpret_cast<multiboot_mod_list*>(info->mods_addr);

    void* mods_addr = reinterpret_cast<void*>(mod->mod_start);

    // Check the command line for the expected string
    if (!strcmp(reinterpret_cast<char*>(info->cmdline),"PATROCLUS_KERNEL"))
    {
        earlyPanic("I don't know what this module is, but it isn't what I wanted!");
    }

    out.hex();

    out << "kernel.bin is at 0x" << mods_addr << "\n";

    return mods_addr;
}

void lmain( void* pagingArea, uint32_t multiboot_flag,
        const struct multiboot_info_structure* info, uint32_t terminalIndex )
{
    // Initiate the terminal
    io::framebuffer_terminal initTerminal;
    out.init(&initTerminal);
    out.hex();
    
    // Test to see if we were booted from custom bootloader
    if (multiboot_flag == MULTIBOOT_CUSTOM_BOOTLOADER_MAGIC)
    {
        uint8_t _row = uint8_t(terminalIndex / initTerminal.vga_width);
        uint8_t _column = uint8_t( terminalIndex - _row * initTerminal.vga_width );
        initTerminal.setCursor(_row,_column);
    }
    else // Not loaded by custom bootloader, clear the screen
    {
        initTerminal.clear();
    }

    // Print header
    initTerminal.setColor(io::vga_color::VGA_COLOR_LIGHT_GREY,
                            io::vga_color::VGA_COLOR_BLUE);
    out << "Entering PatroclusOS 32-bit protected mode init.bin\n";
    initTerminal.setColor(io::vga_color::VGA_COLOR_LIGHT_GREY,
                            io::vga_color::VGA_COLOR_BLACK);

    // MULTIBOOT STUFF
    checkMultiboot(multiboot_flag); // Check flag
    void* mods_addr = getMultibootModuleAddr(info);
    out << "mods_addr is at 0x" << mods_addr << "\n";
    
    // Do the ELF loading
    void* kentry = doElfLoad(mods_addr);

    // Setup long mode
    uint32_t cpuidResult = checkCPUID();

    if (cpuidResult == 1)
    {
        earlyPanic("Error: CPUID not supported");
    }
    else if (cpuidResult == 2)
    {
        earlyPanic("Error: Long mode not supported");
    }
    
    // Now, attempting to enable paging
    out << "Attempting to enable paging:...";

    setupPaging( pagingArea );

    out << " apparently, it went ok?\n";

    // Paging went ok, so now jump to kernel
    out << "Jumping to kernel. Entry point: 0x" << kentry;

    out << ", mbinfo is at: 0x" << info << "\n";

    uint32_t tIndex = initTerminal.getRow() * initTerminal.vga_width
            + initTerminal.getColumn();

    enterKernel(kentry,info,tIndex);

    earlyPanic("Shouldn't be seeing this!!!");

}


void* doElfLoad(void* mods_addr)
{
    // FIXME Don't know if this should be a class, actually
    elf64Loader elfLoader(reinterpret_cast<elf64_Ehdr*>(mods_addr));

    if (!elfLoader.checkHeader())
    {
        earlyPanic("Error: ELF64 header is invalid or we don't support it!");
    }

    if (!elfLoader.elf_load())
    {
        earlyPanic("Error: Couldn't load ELF64 binary kernel.bin");
    }

    // This is successful
    return elfLoader.getEntryPoint();
}