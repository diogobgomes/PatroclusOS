/**
 * @file cstdlib.cpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Definitions for cstdlib.hpp and stdlib.h
 * @version 0.1
 * @date 2024-02-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <klib/cstdlib.hpp>
#include <klib/io.hpp>
#include <devices/BIOSVideoIO.hpp>

[[noreturn,maybe_unused]] void earlyPanic(const char* str)
{
    #if defined(__is32__) || defined(__is64__)
    io::framebuffer_terminal* termPtr = out.getBackEnd();
    termPtr->setColor(io::vga_color::VGA_COLOR_LIGHT_GREY,
                        io::vga_color::VGA_COLOR_RED);
    #endif

    out << "earlyPanic(): ";

    // TODO When we add BIOS color, add definition for 16 bit
    out << str;
    hang();
}




[[noreturn,maybe_unused]] void error(const char* str)
{
    
    
    out << str;
    hang();
}