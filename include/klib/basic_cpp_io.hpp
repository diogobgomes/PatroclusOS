/**
 * @file klib/basic_cpp_io.hpp
 * @author Diogo Gomes
 * @brief Basic functions to support basic IO, in cpp
 * @version 0.1
 * @date 2024-01-14
 * 
 */

#ifndef __BASIC_CPP_IO
#define __BASIC_CPP_IO 1

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

namespace basic_io
{

class terminal
{
public:
    enum class vga_color:uint8_t {
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

    /**
     * @brief Class constructor, sets up the terminal, but doesn't clear it
     * 
     */
    terminal(/* args */);

    /**
     * @brief Clears the terminal with the currently set color
     * 
     */
    void clear();

    /**
     * @brief Set the Color of the terminal
     * 
     * @param fg Foreground color
     * @param bg Background color
     */
    void setColor(enum vga_color fg, enum vga_color bg);

    /**
     * @brief Writes a C-terminated string to the next available spot on screen
     * 
     * @param str String to write
     * @return int 0 for success, 1 for invalid character 
     */
    int write(const char* str);

    int printf(const char* fmt, ...);


    int putEntryAt(char c, size_t x, size_t y);

    int scroll(size_t lines);
    

    const size_t vga_height = 25;
    const size_t vga_width = 80;


    // TODO change outFormat to accept just write, no reason for strlen
    int writeSize(const char* str, size_t len);

private:
    size_t row;
    size_t column;
    uint8_t color;
    uint16_t* buffer;

    uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
        return (uint8_t) fg | (uint8_t) bg << 4;
    }

    uint16_t vga_entry(unsigned char uc, uint8_t tColor) {
        return (uint16_t) uc | tColor << 8;
    }

    int putchar(char c);

    size_t outFormat(const char* fmt, va_list parameters);
    
    


};








} // namespace basic_io




#ifdef __cplusplus
}
#endif

#endif