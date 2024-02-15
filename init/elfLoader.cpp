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

#include "elfLoader.hpp"
#include <stdint.h>
#include <stddef.h>
#include <sys/elf.h>
#include <klib/io.hpp>

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

bool elf64Loader::elf_load()
{
	// ATTENTION: This makes no sanity checks on loadable segments, if they wanna be
    // loaded somewhere ridiculous, we're not checking, like, at all
	elf64_Phdr* ph = reinterpret_cast<elf64_Phdr*>( reinterpret_cast<uint8_t*>(_header) + _header->e_phoff ); // BUG This is buggy, no? Should be the same as the other
	elf64_Phdr* eph = ph + _header->e_phnum;

    out.hex(); // Set terminal to hexadecimal

	for (; ph < eph; ph = reinterpret_cast<elf64_Phdr*>( reinterpret_cast<uint8_t*>(ph) + _header->e_phentsize ))
	{
        out << "Found segment at 0x" << reinterpret_cast<uint64_t>(ph)
                << " of type " << ph->p_type << " and mem size 0x" << ph->p_memsz;

		if( !(checkProgramHeader(ph)) ) return false; // We've hit something
        if( ph->p_type != PT_LOAD ) continue; // We don't care about these
        if( ph->p_memsz == 0 ) continue; // These are also not gonna be loaded
        uint8_t *dest = reinterpret_cast<uint8_t*>(ph->p_paddr);
		const uint8_t *src = reinterpret_cast<uint8_t*>( reinterpret_cast<uint64_t>(_header) + ph->p_offset );

		// Copy it!
        out << ", loading it to 0x" << reinterpret_cast<uint64_t>(dest) << "\n";
		memcpy(dest,src,ph->p_filesz);

		// Check to see if p_memsz > p_filesz
		if (ph->p_memsz > ph->p_filesz)
		{
			memset( dest+ph->p_filesz, 0, ph->p_memsz - ph->p_filesz );
		}	
	}
    return true;
	
}