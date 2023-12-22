/**
 * @file multiboot-loader.c
 * @author Diogo Gomes
 * @brief Multiboot loader, to be linked with stage1.o to produce the final stage1.bin
 * It's purpose is to read the kernel image loaded by stage1, verify it's a
 * multiboot compatible kernel image, load it into high memory, and write the
 * multiboot information structure
 * @version 0.1
 * @date 2023-11-10
 * 
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/multiboot.h>
#include <sys/elf.h>
#include <bootloader/multiboot-loader.h>
#include <bootloader/string.h>
#include <bootloader/vga.h>


uint8_t* _multiboot_main(uint8_t* ptr,const uint16_t* mem_lower,const uint8_t* mem_upper,uint32_t kernel_size) {
    terminal_initialize();
    terminal_write_string("Entering _multiboot_main\n");
    
    // Find the header in our kernel
    const struct multiboot_header *header = multiboot_header_search(ptr);

    if(!header) return NULL; // If we failed to find the header, return 0

    struct multiboot_info_structure *info_table = (struct multiboot_info_structure*) (ptr - MULTIBOOT_INFO_SIZE);

    // We must now parse the header->flags field
    if (header->flags & 2) { // Flags[1]
        // Lower memory is easy
        info_table->mem_lower = *mem_lower;

        // Upper memory is harder
        // mem_upper + 4 is the start of the actual list of entries
        info_table->mem_upper = get_upper_memory_size( (struct mmap_structure_entry*) (mem_upper + 4), *mem_upper );

        if(info_table->mem_upper == 0) info_table->mem_upper = 0; //FIXME test this condition, return an error

        // Now for the mmap, is really easy, cause everything was setup for us
        info_table->mmap_length = *mem_upper;
        info_table->mmap_addr = (uint32_t) (mem_upper + 4);
    }
    if (header->flags & 4) { // Flags[2]
        return NULL; // We can't deal with this request yet
    }
    /*if (!(header->flags & 65536)) { // Flags[16]
        // If this isn't set, we actually have an issue, cause we can't parse ELF headers
        // So we're not actually compliant with multiboot. Best we can do is fail gracefully
        // TODO implement different error codes, so we can output different messages?
        return 0;
    }*/
    if ( header->flags & 65528) { // Any other flags set until bit 15
        return NULL; // This is actually the multiboot standard, hey!
    }

    terminal_write_string("Header is lekker, do the elf loading\n");

    // Do the elf loading!
    elf32_Ehdr *elf_header = (elf32_Ehdr*) ptr;
    if(!(elf_check_file(elf_header))) return NULL;
    //return(elf_check_file2(elf_header));
    if(!(elf_load(elf_header))) return NULL; // Here we actually do the loading, cool

    //asm ("movl $0xfafa,%eax;");
    //asm ("hlt;");

    terminal_write_string("Everything is more than amazing, jump to kernel\n");

    return (uint8_t *) elf_header->e_entry;

}

const struct multiboot_header* multiboot_header_search(uint8_t* ptr) {
    // Let's try to do a loop looking for the multiboot header in the buffer
    struct multiboot_header *header;

    for ( header = (struct multiboot_header *) ptr;
          ( (uint8_t*) header <= ptr + MULTIBOOT_SEARCH - 12 ); // Search first n bytes, -12 for size of struct
          header = (struct multiboot_header *) ((uint8_t*) header + MULTIBOOT_HEADER_ALIGN)) // Increment by MULTIBOOT_HEADER_ALIGN
    {
        if( header->magic == MULTIBOOT_MAGIC &&
            (header->magic + header->flags + header->checksum) == 0) // Found magic && checksum checks out
        return header;
    }

    return NULL; // Didn't find it, return NULL    
}

uint32_t get_upper_memory_size( const struct mmap_structure_entry *ptr, uint32_t num_entries ) {
    // Loop through the structure, add up the lengths
    // Pinky promise not to trash the ptr
    uint32_t final_size = 0;
    // FIXME: check that memory length is not greater than limit for uint32_t
    for ( const struct mmap_structure_entry * entry = ptr; (uint32_t) entry < num_entries; ++entry )
    {
        uint64_t test = final_size + entry->length;
        if (test > UINT32_MAX) return 0;
        final_size = (uint32_t) test; // It's safe cause we tested it
    }

    return final_size;
    
}

bool elf_check_file(elf32_Ehdr *header ) {
    // Have to check each of the necessary parts of the header
    // Start by checking it's a valid pointer
    // TODO could actually separate in two functions, check if valid, and check if supported
    if(!header) return false;

    // Check magic numbers
    if(header->e_ident[EI_MAG0] != ELFMAG0) return false;
    if(header->e_ident[EI_MAG1] != ELFMAG1) return false;
    if(header->e_ident[EI_MAG2] != ELFMAG2) return false;
    if(header->e_ident[EI_MAG3] != ELFMAG3) return false;

    // If we got here, it's a valid ELF file, but we gotta check that we support it
    if(header->e_ident[EI_CLASS] != ELFCLASS32) return false;
    if(header->e_ident[EI_DATA] != ELFDATA2LSB) return false;
    if(header->e_machine != EM_386) return false;
    if(header->e_ident[EI_VERSION] != EV_CURRENT) return false;
    if(header->e_type != ET_EXEC) return false; // Kernel must be ET_EXEC

    // If we got here, it's a loadable kernel
    return true;
}

bool elf_check_prog_header(elf32_Phdr *header) {
    // Just check that the program header is compatible, no weird stuff
    // We can't deal with dynamic or interp
    if(header->p_type == PT_DYNAMIC) return false;
    if(header->p_type == PT_INTERP) return false;

    return true;
}

bool elf_load(elf32_Ehdr *header ) {
    // ATTENTION: This makes no sanity checks on loadable segments, if they wanna be
    // loaded somewhere ridiculous, we're not checking, like, at all
    elf32_Phdr* ph = (elf32_Phdr*)((unsigned char*)header + header->e_phoff);
    elf32_Phdr* eph = ph + header->e_phnum;


    for (; ph < eph ; ph += header->e_phentsize)
    {
        if( !(elf_check_prog_header(ph)) ) return false; // We've hit something
        if( ph->p_type != PT_LOAD ) continue; // We don't care about these
        if( ph->p_memsz == 0 ) continue; // These are also not gonna be loaded
        elf32_Phdr *dest = (elf32_Phdr*) ph->p_vaddr;
        const elf32_Phdr *src = (elf32_Phdr*)((unsigned char*)header + ph->p_offset);
        // Copy it!
        memcpy(dest,src,ph->p_filesz);

        // Check to see if p_memsz > p_filesz
        if(ph->p_memsz > ph->p_filesz) {
            // Fill with zeros
            memset(header + ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);
        }
    }

    return true;
}


/*
int elf_check_file2(elf32_Ehdr *header ) {
    // Have to check each of the necessary parts of the header
    // Start by checking it's a valid pointer
    // TODO could actually separate in two functions, check if valid, and check if supported
    if(!header) return 0;

    // Check magic numbers
    if(header->e_ident[EI_MAG0] != ELFMAG0) return 1;
    if(header->e_ident[EI_MAG1] != ELFMAG1) return 2;
    if(header->e_ident[EI_MAG2] != ELFMAG2) return 3;
    if(header->e_ident[EI_MAG3] != ELFMAG3) return 4;

    // If we got here, it's a valid ELF file, but we gotta check that we support it
    if(header->e_ident[EI_CLASS] != ELFCLASS32) return 5;
    if(header->e_ident[EI_DATA] != ELFDATA2LSB) return 6;
    if(header->e_machine != EM_386) return 7;
    if(header->e_ident[EI_VERSION] != EV_CURRENT) return 8;
    if(header->e_type != ET_EXEC) return 9; // Kernel must be ET_EXEC

    // If we got here, it's a loadable kernel
    return 10;
}*/




