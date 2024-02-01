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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/elf.h>
//#include <klib/io.hpp>

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
    
    bool elf_load();


}; /* End of elf64Loader */
