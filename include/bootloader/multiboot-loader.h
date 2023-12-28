/**
 * @file multiboot-loader.h
 * @author Diogo Gomes
 * @brief Header file for multiboot-loader.c
 * @version 0.1
 * @date 2023-12-20
 * 
 */

#ifndef _MULTIBOOT_LOADER_H
#define _MULTIBOOT_LOADER_H 1

#ifdef __cplusplus
extern "C" {
#endif

// Macros for handling definition of inline functions
#ifndef INLINE
# define INLINE static inline
#endif

#include <stdint.h>
#include <sys/multiboot.h>
#include <sys/elf.h>

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

/**
 * @brief Multiboot loader entry point
 * 
 * @param ptr Pointer to the start of the kernel image file. There must also be
 * a reserved area of memory of size MULTIBOOT_INFO_SIZE, where we store the multiboot
 * info structure to be passed to the kernel, just before this pointer. Since this is
 * called through assembly, it must be pushed to stack
 * @param mem_lower Pointer to the size of lower memory read using int 12h
 * @param mem_upper Pointer to the upper memory map structure. The first 4 bytes are the
 * number of entries, and after is a list of 24 byte entries, read using int 15h
 * @param kernel_size Size of kernel file
 * @return uint8_t* WIP
 */
uint8_t* _multiboot_main(uint8_t* ptr, const uint16_t* mem_lower, const uint8_t* mem_upper, uint32_t boot_device);

/**
 * @brief Search for a valid multiboot header in the first MULTIBOOT_SEARCH bytes
 * of the kernel file
 * 
 * @param ptr Pointer to the kernel file in memory
 * @return Returns a pointer to the multiboot_header (if found), or NULL if not found
 */
const struct multiboot_header* multiboot_header_search(uint8_t* ptr);

/**
 * @brief Get the size of upper memory from RAM, where it was stored by int 15h
 * 
 * @param ptr Pointer to the start of the upper memory structure
 * @param num_entries Number of entries in the structure
 * @return Size of upper memory
 */
uint32_t get_upper_memory_size( const struct mmap_structure_entry *ptr, uint32_t num_entries );

/**
 * @brief Check the validity of the ELF header, and whether we support it
 * 
 * @param header Pointer the ELF header
 * @return true Header is valid, and we can load it
 * @return false Header is either not valid, or not supported
 */
bool elf_check_file(elf32_Ehdr *header);

/**
 * @brief Check that the ELF program header is compatible, no weird stuff
 * 
 * @param header Pointer to a ELF program header
 * @return true We can load it
 * @return false We can't load it
 */
bool elf_check_prog_header(elf32_Phdr *header);

/**
 * @brief Load a program with a specific ELF header. Attention: this makes no sanity
 * check on loadable segments, if they wanna be loaded somewhere ridiculous, we're
 * not checking it, like, at all
 * 
 * @param header Pointer to a valid, loadable ELF header
 * @return true Loading it was successful, we can, in principle, jump to it
 * @return false Something went wrong loading it
 */
bool elf_load(elf32_Ehdr *header);



#ifdef __cplusplus
}
#endif

#endif