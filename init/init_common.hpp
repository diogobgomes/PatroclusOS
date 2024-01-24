/**
 * @file loader_common.h
 * @author Diogo Gomes
 * @brief Common function for the 32-bit loader
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/elf.h>


/*********************************
 * Useful standard functions
**********************************/

size_t strlen(const char* str);

void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size);

void* memset(void* bufptr, int value, size_t size);

void* memmove(void* dstptr, const void* srcptr, size_t size);

/*void charSetTest(unsigned char c);*/

char* itoa(int value, char* str, int base);

char* utoa(unsigned int value, char* str, int base);


/*********************************
 * Basic IO framebuffer class
**********************************/

#define BASIC_IO_FRAMEBUFFER 0xb8000
#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define MAX_INT_STR_SIZE 33


class initTerminal
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

    initTerminal(){};

    void setColor(enum vga_color fg, enum vga_color bg);

    void clear();

    int scroll(size_t lines);

    int putchar(char c);

    int writeString(const char* str);

    int writeInt(int num);

    int writeHex( unsigned int num);

private:
        size_t row = 0;
        size_t column = 0;
        uint8_t color = 0x07;
        uint16_t* buffer = reinterpret_cast<uint16_t*>(BASIC_IO_FRAMEBUFFER);
}; /* end of initTerminal */


/*********************************
 * ELF64 loader class
**********************************/
/*class elf63Loader
{
//private:
public:
    elf64_Ehdr _header;
    //elf64_Phdr _program_header;
    bool checkProgramHeader(elf64_Phdr* p_header);

//public:
    elf63Loader() = delete;
    // TODO move checkHeader here
    elf63Loader(elf64_Ehdr &header) : _header(header) {        
    };

    bool checkHeader(initTerminal&);

    
    // Dependency on terminal to print nice error messages
    bool elf_load(initTerminal& terminal);


};*/


class elf64Loader
{
//private:
public:
    elf64_Ehdr* _header;
    //elf64_Phdr _program_header;
    bool checkProgramHeader(elf64_Phdr* p_header);

//public:
    elf64Loader() = delete;
    // TODO move checkHeader here
    elf64Loader(elf64_Ehdr *header) : _header(header) {        
    };

    bool checkHeader();

    void* getEntryPoint() { return reinterpret_cast<void*>(_header->e_entry); };
    
    // Dependency on terminal to print nice error messages
    // FIXME this is a mess, you know that, right? In your heart?
    bool elf_load(initTerminal& terminal);


}; /* End of elf64Loader */

#ifdef __cplusplus
}
#endif