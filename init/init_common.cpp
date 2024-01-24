/**
 * @file init_common.cpp
 * @author Diogo Gomes
 * @brief Definitions from loader_common.h
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "init_common.hpp"
#include <stdint.h>
#include <stddef.h>

/*********************************
 * Useful standard functions
**********************************/

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* memcpy(void* __restrict__ dstptr, const void* __restrict__ srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (size_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

char* itoa(int value, char* str, int base)
{
    // Test base
    if (base < 2 || base > 16)
    {
        return NULL;
    }

    // Check edge case of value == 0
    if (value == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    

    int num = value;
    size_t index = 0;
    for (; num != 0; ++index)
    {
        char digit = (char) (num % base); // Guaranteed between 0 and base
        if (digit < 10)
        {
            str[index] = '0' + digit;
        }
        else
        {
            str[index] = 'A' + digit - 10;
        }
        num /= base;
    }

    if (base == 10 && value < 0)
    {
        str[index] = '-';
        index++;
    }

    // Terminate string
    str[index--] = '\0';

    // Now invert the string (it's the wrong way around)
    for (size_t i = 0; i < index/2+1; i++)
    {
         char tmp = str[i];
         str[i] = str[index-i];
         str[index-i] = tmp;
    }

    // Now we have everything
    return str;
    
}


char* utoa(unsigned int value, char* str, int base)
{
    // Test base
    if (base < 2 || base > 16)
    {
        return NULL;
    }

    // Check edge case of value == 0
    if (value == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    unsigned int num = value;
    size_t index = 0;
    for (; num != 0; ++index)
    {
        char digit = (char) (num % (unsigned int) base); // Guaranteed to be between 0 and base, base is positive
        if (digit < 10)
        {
            str[index] = '0' + digit;
        }
        else
        {
            str[index] = 'A' + digit - 10;
        }
        num /= (unsigned int) base; // Base is positive
    }


    // Terminate string
    str[index--] = '\0';

    // Now invert the string (it's the wrong way around)
    for (size_t i = 0; i < index/2+1; i++)
    {
         char tmp = str[i];
         str[i] = str[index-i];
         str[index-i] = tmp;
    }

    // Now we have everything
    return str;
    
}


/*void charSetTest(unsigned char c) {
    uint8_t* ptr = 0xB8000;

    //unsigned char c = 'O';

    // Put ptr
    uint16_t aa = c | 0x17 << 8;

    *ptr = aa;
}*/


/*********************************
 * Basic IO class
**********************************/

static inline uint8_t vga_entry_color(initTerminal::vga_color fg, initTerminal::vga_color bg) {
        return (uint8_t) fg | (uint8_t) bg << 4;
    }

static inline uint16_t vga_entry(unsigned char uc, uint8_t tColor) {
    return (uint16_t) uc | tColor << 8;
}

void initTerminal::setColor(enum vga_color fg, enum vga_color bg)
{
	color = vga_entry_color(fg,bg);
}

int initTerminal::scroll(size_t lines)
{
	// Check lines is number allowed
    if (lines >= VGA_HEIGHT)
    {
        return 1;
    }

    size_t size = VGA_WIDTH * (VGA_HEIGHT - lines) * 2;

    memmove(buffer, buffer + VGA_WIDTH*lines, size);

    // Clear last lines
    for (size_t y = VGA_HEIGHT - lines; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            buffer[index] = vga_entry(' ',color);
        }   
    }

    return 0;
}

int initTerminal::putchar(char c) // FIXME annoying buf with full lines and \n, basically have to change from pointing to next space to pointing to last space
{
	if(c=='\n')
    {
        column = 0;
        if (++row == VGA_HEIGHT)
        {
            scroll(1);
            row--;
        }
        return 0;
    }
    
    const size_t index = row* VGA_WIDTH + column;

    // Check the character
    if (c < 20 || c > 126)
    {
        return 1; // Failure, character out of bounds
    }

    buffer[index] = vga_entry((unsigned char)c, color);

    // Increment column, check if we need to change lines, or scroll
    if (++column == VGA_WIDTH)
    {
        column = 0;
        if (++row == VGA_HEIGHT)
        {
            scroll(1);
            row--;
        }
    }

    return 0;
    
    
}

int initTerminal::writeString(const char *str)
{
    size_t length = strlen(str);
	for (size_t i = 0; i < length; i++)
	{
		int test = putchar(str[i]);
		if(test) return test;
	}
	return 0;
}

int initTerminal::writeInt(int num)
{
	char str[MAX_INT_STR_SIZE];
	itoa(num,str,10);
	return writeString(str);
}

int initTerminal::writeHex(unsigned int num)
{
	char str[MAX_INT_STR_SIZE];
	utoa(num,str,16);
	return writeString(str);
}

void initTerminal::clear(void)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            buffer[index] = vga_entry(' ',color);
        }
        
    }
    
}



/*********************************
 * ELF64 loader class
**********************************/
bool elf64Loader::checkHeader()
{
	// Have to check each of the necessary parts of the header
    // Start by checking it's a valid pointer
    // TODO could actually separate in two functions, check if valid, and check if supported
    if(!_header) return false;

    // Check magic numbers
    if(_header->e_ident[EI_MAG0] != ELFMAG0) return false;
    if(_header->e_ident[EI_MAG1] != ELFMAG1) return false;
    if(_header->e_ident[EI_MAG2] != ELFMAG2) return false;
    if(_header->e_ident[EI_MAG3] != ELFMAG3) return false;

    // If we got here, it's a valid ELF file, but we gotta check that we support it
    if(_header->e_ident[EI_CLASS] != ELFCLASS64) return false;
    if(_header->e_ident[EI_DATA] != ELFDATA2LSB) return false;
    if(_header->e_machine != EM_X86_64) return false;
    if(_header->e_ident[EI_VERSION] != EV_CURRENT) return false;
    if(_header->e_type != ET_EXEC) return false; // Kernel must be ET_EXEC

    // If we got here, it's a loadable kernel
    return true;
}

bool elf64Loader::checkProgramHeader(elf64_Phdr* p_header)
{
    // Just check that the program header is compatible, no weird stuff
    // We can't deal with dynamic or interp
    if(p_header->p_type == PT_DYNAMIC) return false;
    if(p_header->p_type == PT_INTERP) return false;

    return true;
}

bool elf64Loader::elf_load(initTerminal& terminal)
{
	// ATTENTION: This makes no sanity checks on loadable segments, if they wanna be
    // loaded somewhere ridiculous, we're not checking, like, at all
	elf64_Phdr* ph = reinterpret_cast<elf64_Phdr*>( reinterpret_cast<uint8_t*>(_header) + _header->e_phoff ); // BUG This is buggy, no? Should be the same as the other
	elf64_Phdr* eph = ph + _header->e_phnum;

	for (; ph < eph; ph = reinterpret_cast<elf64_Phdr*>( reinterpret_cast<uint8_t*>(ph) + _header->e_phentsize ))
	{
		terminal.writeString("Found segment at 0x");
		terminal.writeHex(reinterpret_cast<uint32_t>(ph));
		terminal.writeString(" of type ");
		terminal.writeHex(reinterpret_cast<uint32_t>(ph->p_type));
        terminal.writeString(", and mem size 0x");
        terminal.writeHex((ph->p_memsz));

		if( !(checkProgramHeader(ph)) ) return false; // We've hit something
        if( ph->p_type != PT_LOAD ) continue; // We don't care about these
        if( ph->p_memsz == 0 ) continue; // These are also not gonna be loaded
        uint8_t *dest = reinterpret_cast<uint8_t*>(ph->p_paddr);
		const uint8_t *src = reinterpret_cast<uint8_t*>( reinterpret_cast<uint64_t>(_header) + ph->p_offset );

		// Copy it!
        terminal.writeString(", loading it to 0x");
        terminal.writeHex(reinterpret_cast<uint32_t>(dest));
		memcpy(dest,src,ph->p_filesz);

        terminal.writeString("\n");

		// Check to see if p_memsz > p_filesz
		if (ph->p_memsz > ph->p_filesz)
		{
			memset( dest+ph->p_filesz, 0, ph->p_memsz - ph->p_filesz );
		}
		
	}

    /*elf64_Phdr* ph = (elf64_Phdr*)((uint8_t*)_header + _header->e_phoff);
    elf64_Phdr* eph = ph + _header->e_phnum; // BUG This is buggy, no?



    terminal.writeString("Found segment at 0x");
		terminal.writeHex(reinterpret_cast<uint32_t>(ph));
		terminal.writeString(" of type ");
		terminal.writeHex(reinterpret_cast<uint32_t>(ph->p_type));
		terminal.writeString("\n");


    //ph =  (elf64_Phdr*) (((uint8_t*)ph) + _header->e_phentsize);
    //ph =  (elf64_Phdr*) (((uint8_t*)ph) + _header->e_phentsize);

    


    terminal.writeHex((uint32_t)eph);
    terminal.writeString("\n");
    terminal.writeHex((uint64_t) (((uint8_t*)ph) + _header->e_phentsize));
    //return false;

    terminal.writeString("\nYOOOOOO\n");

    for (; ph < eph; ph =  (elf64_Phdr*) (((uint8_t*)ph) + _header->e_phentsize))
    // Conversions inside for are necessary for pointer arithmetic to work
    // e_phnum is in bytes, but pointer arithmetic will assume size
    {
        terminal.writeString("Found segment at 0x");
		terminal.writeHex(reinterpret_cast<uint32_t>(ph));
		terminal.writeString(" of type ");
		terminal.writeHex(reinterpret_cast<uint32_t>(ph->p_type));
		terminal.writeString("\n");

        if( !(checkProgramHeader(ph)) ) return false; // We've hit something
        if( ph->p_type != PT_LOAD ) continue; // We don't care about these
        if( ph->p_memsz == 0 ) continue; // These are also not gonna be loaded
        elf64_Phdr *dest = (elf64_Phdr*) ph->p_vaddr;
        const elf64_Phdr *src = (elf64_Phdr*)((uint8_t*)_header + ph->p_offset);
        
        // Copy it!
        //mem(dest,src,ph->p_filesz);

        // Check to see if p_memsz > p_filesz
        if(ph->p_memsz > ph->p_filesz) {
            // Fill with zeros
            //memset(_header + ph->p_filesz, 0, ph->p_memsz - ph->p_filesz); // BUG Crashes the whole damn thing
        }
        //return false;
    }*/





    return true;
	
}