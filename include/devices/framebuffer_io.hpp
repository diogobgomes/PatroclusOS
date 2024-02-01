/**
 * @file framebuffer_io.hpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Defines classes for accessing memory-mapped framebuffer IO
 * @version 0.1
 * @date 2024-01-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <stdint.h>
#include <stddef.h>

namespace framebuffer_io
{

constexpr uint32_t FRAMEBUFFER_LOCATION = 0xb8000;

class framebuffer_terminal
{
public:
    /**
     * @brief Possible colors for framebuffer io
     * 
     */
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
    framebuffer_terminal() : row(0), column(0),color(0x07), fullLine(false),
                buffer(reinterpret_cast<uint16_t*>(FRAMEBUFFER_LOCATION)) {} ;

    /**
     * @brief Resets the terminal, like if the constructor had been called
     * 
     */
    void reset() {
        buffer = reinterpret_cast<uint16_t*>(FRAMEBUFFER_LOCATION);
        row = 0;
        column = 0;
        fullLine = false;
        setColor(vga_color::VGA_COLOR_LIGHT_GREY,vga_color::VGA_COLOR_BLACK);
    }

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
     * @brief Scrolls the terminal by lines
     * 
     * @param lines Number of lines to scroll
     * @return int 0 for success, any other number for failure
     */
    int scroll(size_t lines);
    
    const size_t vga_height = 25;
    const size_t vga_width = 80;

    /**
     * @brief Puts a character in the next available location and increments position
     * 
     * @param c Character to put
     * @return int 0 for success, 1 for character out-of-bounds
     */
    int putchar(char c);

    

private:
    uint16_t row;
    uint16_t column;
    uint8_t color;
    bool fullLine; // Ugly hack, to prevent problem with full line and a '\n'
    uint16_t* buffer;

    uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
        return static_cast<uint8_t>(fg) | static_cast<uint8_t>(bg) << 4;
    }

    uint16_t vga_entry(unsigned char uc, uint8_t tColor) {
        return static_cast<uint16_t>(uc) | tColor << 8;
    }

};





} // namespace framebuffer_io
