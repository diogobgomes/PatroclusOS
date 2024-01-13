/**
 * @file basic_io.h
 * @author Diogo Gomes
 * @brief Header file for vga.c, the basic bootloader output
 * @version 0.1
 * @date 2023-12-21
 * 
 */

#ifndef _BOOTLOADER_VGA_H
#define _BOOTLOADER_VGA_H 1

#ifdef __cplusplus
extern "C" {
#endif

// Macros for handling definition of inline functions
#ifndef INLINE
# define INLINE static inline
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_MEM_LOCATION 0xB8000

/**
 * @brief Returns the correct background/foreground color for some entry
 * 
 * @param fg Foreground color
 * @param bg Background color
 * @return uint8_t containing the correct color, shift by 8 in final entry
 */
INLINE uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);

/**
 * @brief Returns the correct color and character combination, that can be
 * written to memory
 * 
 * @param uc Desired character
 * @param color Color given by vga_entry_color
 * @return uint16_t containing the color+character, ready to be written to memory
 */
INLINE uint16_t vga_entry(unsigned char uc, uint8_t color);

/**
 * @brief Terminal initialization function, must be called before writing
 * 
 */
void terminal_initialize(void);

/**
 * @brief Sets color of the terminal
 * 
 * @param color Color returned from vga_entry_color
 */
void terminal_set_color(uint8_t color);

/**
 * @brief Places a character at a specific location in the screen
 * 
 * @param c Character to write
 * @param color Color returned from vga_entry_color
 * @param x x location, must be between 0 and VGA_WIDTH
 * @param y y location, must be between 0 and VGA_HEIGHT 
 * @return 0 for success, 1 if x or y are out of bounds, 2 if c is not a valid ASCII character
 */
int terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y);

/**
 * @brief Writes a character, with the default color, to the next available spot
 * 
 * @param c Character to write
 * @return Pass-along failure from terminal_put_entry_at, 0 for success
 */
int terminal_putchar(char c);

/**
 * @brief Writes a string to the next available spot on screen, using terminal_putchar
 * 
 * @param data String to write
 * @param size Size of string
 * @return Pass-along failure from terminal_put_entry_at, 0 for success
 */
int terminal_write(const char* data, size_t size);

/**
 * @brief Writes a C-terminated string to the next available spot on screen
 * 
 * @param data C-terminated string to write
 * @return Pass-along failure from terminal_put_entry_at, 0 for success
 */
int terminal_write_string(const char* data);

/**
 * @brief Clears the screen with black
 * 
 */
void terminal_clear(void);

/**
 * @brief Scrolls terminal by one line
 * 
 */
void terminal_scroll(void);

/**
 * @brief Basic version of printf from stdio, to be used for now
 * 
 * @param fmt Formatted string
 * @param ... Extra arguments
 * @return int Number of written bytes
 */
int bprintf(const char* fmt, ...);

/**
 * @brief Get the correct color code
 * 
 * @param fg Foreground color
 * @param bg Background color
 * @return uint8_t Desired color code
 */
INLINE uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return (uint8_t) fg | (uint8_t) bg << 4;
}
 
/**
 * @brief Get the vga entry code
 * 
 * @param uc Character to write
 * @param color Color code, obtained from vga_entry_color
 * @return uint16_t Desired vga entry code, to be written to memory
 */
INLINE uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}



#ifdef __cplusplus
}
#endif

#endif