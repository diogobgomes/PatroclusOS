/**
 * @file framebuffer_io.cpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Definitions from framebuffer_io.hpp
 * @version 0.1
 * @date 2024-01-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdint.h>
#include <stddef.h>
#include <klib/string.h>
#include <devices/framebuffer_io.hpp>

void framebuffer_io::framebuffer_terminal::clear()
{
    for (size_t y = 0; y < vga_height; y++)
    {
        for (size_t x = 0; x < vga_width; x++)
        {
            const size_t index = y * vga_width + x;
            buffer[index] = vga_entry(' ',color);
        }
        
    }
}

void framebuffer_io::framebuffer_terminal::setColor(enum vga_color fg, enum vga_color bg)
{
    color = vga_entry_color(fg,bg);
}

int framebuffer_io::framebuffer_terminal::putchar(char c) // FIXME annoying buf with full lines and \n, basically have to change from pointing to next space to pointing to last space
{
    if(c=='\n')
    {
        column = 0;
        if (++row == vga_height)
        {
            scroll(1);
            row--;
        }
        return 0;
    }
    
    const size_t index = row* vga_width + column;

    // Check the character
    if (c < 20 || c > 126)
    {
        return 1; // Failure, character out of bounds
    }

    buffer[index] = vga_entry(static_cast<unsigned char>(c), color);

    // Increment column, check if we need to change lines, or scroll
    if (++column == vga_width)
    {
        column = 0;
        if (++row == vga_height)
        {
            scroll(1);
            row--;
        }
    }

    return 0;
    
    
}

int framebuffer_io::framebuffer_terminal::scroll(size_t lines)
{
    // Check lines is number allowed
    if (lines >= vga_height)
    {
        return 1;
    }

    size_t size = vga_width * (vga_height - lines) * 2;

    memmove(buffer, buffer + vga_width*lines, size);

    // Clear last lines
    for (size_t y = vga_height - lines; y < vga_height; y++)
    {
        for (size_t x = 0; x < vga_width; x++)
        {
            const size_t index = y * vga_width + x;
            buffer[index] = vga_entry(' ',color);
        }   
    }

    return 0;
}