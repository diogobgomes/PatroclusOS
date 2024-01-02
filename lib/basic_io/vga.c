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
#include <klib/vga.h>
#include <klib/string.h>

//TODO implement newline
//TODO implement scrolling


size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
//TODO instead of clearing the screen, go to next available line
void terminal_initialize(void) 
{
	terminal_row = 11;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;

	// Set a red line to separate everything
    uint8_t red_color = vga_entry_color(VGA_COLOR_BLACK,VGA_COLOR_RED);
    for(size_t x = 0; x < VGA_WIDTH; x++)
    {
        const size_t index = terminal_row * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ',red_color);
    }
    ++terminal_row;

    
    

    /*
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}*/
}
 
void terminal_set_color(uint8_t color) 
{
	terminal_color = color;
}
 
int terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y) 
{
    // Check that x and y are within bounds
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT )
    {
        return 1; // Failure, position out of bounds
    }
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
 
int terminal_putchar(char c) 
{
    if(c=='\n') {
        terminal_column = 0;
        if(++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
        return 0;
    }
	int test = terminal_put_entry_at(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
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

