/**
 * @file klib/cpp/terminal.cpp
 * @author Diogo Gomes
 * @brief Definitions for class terminal
 * @version 0.1
 * @date 2024-01-14
 * 
 */

#include <klib/basic_cpp_io.hpp>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <limits.h>
#include <klib/string.h>
#include <klib/stdlib.h>
//#include <klib/hidden/stdio_format.h>

#define VGA_MEM_LOCATION 0xB8000

basic_io::terminal::terminal(void)
{
    row = 0;
    column = 0;
    color = vga_entry_color(vga_color::VGA_COLOR_LIGHT_GREY, vga_color::VGA_COLOR_BLACK);
    buffer = (uint16_t*) VGA_MEM_LOCATION;
}

void basic_io::terminal::clear(void)
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

int basic_io::terminal::putEntryAt(char c, size_t x, size_t y)
{
    if (x >= vga_width || y >= vga_height)
    {
        return 1;
    }

    const size_t index = y * vga_width + x;

    buffer[index] = vga_entry((unsigned char) c, color);
    return 0;
    
}

void basic_io::terminal::setColor(enum vga_color fg, enum vga_color bg)
{
    color = vga_entry_color(fg,bg);
}

int basic_io::terminal::write(const char* str)
{
    size_t length = strlen(str);
    for (size_t i = 0; i < length; i++)
    {
        int test = putchar(str[i]);
        if(test) return test;
    }
    return 0;
}

int basic_io::terminal::putchar(char c)
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

    buffer[index] = vga_entry((unsigned char)c, color);

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

int basic_io::terminal::scroll(size_t lines)
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


//FIXME this whole thing is hacky as fuuuuuck (maybe declare a friend function?)
#define MAX_INT_STR_SIZE 33

size_t basic_io::terminal::outFormat(const char* fmt, va_list parameters)
{
    // Size of the string
    size_t length = strlen(fmt);

    // Index to keep track of what has been written
    size_t written = 0;

    for( size_t index = 0;
            index < length;
            ++index)
    {
        char c = fmt[index];
        //char* testPointer = &c;
        
        if (c != '%') // Normal character
        { // TODO make a string, or do this with seeking next control char, don't call this func every char
            writeSize(&c,1);
            ++written;
            continue;
        }

        // Check edge case where % is the last character
        else if (! fmt[++index] )
        {
            return written;
        }

        // Now it's either an escape, or a format
        switch (fmt[index])
        {
        char ch;
        const char* cstr;
        size_t cstrLength;
        char str[MAX_INT_STR_SIZE];
        size_t strLength;
        int num;
        unsigned int unNum;
        case '%': // Escape
            writeSize(fmt + index,1);
            ++written;
            break;

        case 'c': // Character
            //TODO check result of writeSize for error
            ch = (char) va_arg(parameters,int);
            writeSize(&ch,1);
            ++written;
            break; 
        
        case 's': //String
            cstr = va_arg(parameters, const char *);
            cstrLength = strlen(cstr);
            writeSize(cstr,cstrLength);
            written += cstrLength;
            break;

        case 'i': // Integer
            num = va_arg(parameters,int);
            if ( ! itoa (num, str,10) )
            {
                return written;
            }
            strLength = strlen(str);
            writeSize(str,strLength);
            written += strLength;
            break;
        
        case 'x': // Hexadecimal
            unNum = va_arg(parameters,unsigned int);
            // Using utoa for very large numbers
            if ( ! utoa (unNum, str, 16) )
            {
                return written;
            }
            strLength = strlen(str);
            writeSize(str,strLength);
            written += strLength;
            break;

        default:
            writeSize(fmt + index - 1,1);
            ++written;
            break;
        }
    }

    return written;
}

int basic_io::terminal::printf(const char* fmt, ...)
{
    va_list parameters;
    va_start(parameters, fmt);

    size_t written = outFormat(fmt,parameters);

    va_end(parameters);

    // Check for overflow, not a perfect solution, but weird and rare edge case
    if (written > INT_MAX)
    {
        return INT_MAX;
    }

    return (int) written;
    
}

int basic_io::terminal::writeSize(const char* str, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        int test = putchar(str[i]);
        if(test) return test;
    }
    return 0;
}