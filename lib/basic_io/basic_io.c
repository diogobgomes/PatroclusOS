/**
 * @file vga.c
 * @author Diogo Gomes
 * @brief Basic write to screen capability, used by the bootloader and early
 * kernel. Works by writing directly to memory at 0xb8000
 * @version 0.1
 * @date 2023-12-21
 * 
 */

#include <stdint.h>
#include <stddef.h>
#include <klib/basic_io.h>
#include <klib/string.h>
#include <klib/hidden/stdio_format.h>
#include <limits.h>


size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
INLINE int _hidden_put_entry_at(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;

    // Check the character
    // For now, allow only simple, non-control ASCII
    if (c < 20 || c > 126)
    {
        return 2; // Failure, character out of bounds
    }
    
	terminal_buffer[index] = vga_entry((unsigned char)c, color);
    return 0; // Success
}

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) VGA_MEM_LOCATION;
}

void terminal_clear(void)
{
    uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    // Faster this way than terminal_put_entry_at, skip checks
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++ ) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', color);
        }
    }
}

void terminal_scroll(void)
{
    // Size of memory to copy
    // Entries are uint16_t, so VGA_WIDTH * (VGA_HEIGHT - 1) * 2
    size_t size = 3840;

    memmove( terminal_buffer, terminal_buffer + VGA_WIDTH, size );

    // Clear last line
    // TODO there is probably optimization here
    //size_t y_off = (VGA_HEIGHT -1) * VGA_WIDTH;
    size_t y= VGA_HEIGHT-1;
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index  = y * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

void terminal_set_color(uint8_t color) 
{
	terminal_color = color;
}
 
int terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y) 
{
    // Check that x and y are within bounds
    // TODO when we switch to c++, put this as private, and just forego this check
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT )
    {
        return 1; // Failure, position out of bounds
    }
	
    return _hidden_put_entry_at(c,color,x,y);

    
}
 
int terminal_putchar(char c) 
{
    if(c=='\n') {
        terminal_column = 0;
        if(++terminal_row == VGA_HEIGHT)
        {
            //terminal_row = 0;
            terminal_scroll();
            terminal_row--;
        }
        return 0;
    }
	int test = _hidden_put_entry_at(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
        {
			//terminal_row = 0;
            terminal_scroll();
            terminal_row--;
        }
	}
    return test;
}
 
int terminal_write(const char* data, size_t size) 
{
    int test = 0;
	for (size_t i = 0; i < size; i++)
    {
		test = terminal_putchar(data[i]);
        if(test) return test;
    }
    return test;
}
 
int terminal_write_string(const char* data) 
{
	int test = terminal_write(data, strlen(data));
    return test;
}

#define MAX_STR_SIZE 3000

int bprintf(const char* fmt, ...)
{
    va_list parameters;
    va_start(parameters,fmt);

    size_t written = outFormat(fmt,parameters,&terminal_write);

    va_end(parameters);

    // Check for overflow, not a perfect solution, but weird and rare edge case
    if (written > INT_MAX)
    {
        return INT_MAX;
    }

    return (int) written;
}