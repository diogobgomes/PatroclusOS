/**
 * @file multiboot.h
 * @author Diogo Gomes
 * @brief Header file for the multiboot loader program, defines useful structures
 * @version 0.1
 * @date 2023-11-10
 * 
 */

#include <stdint.h>

/**
 * @brief Multiboot header structure, that we must search for in the kernel image
 * 
 */
struct multiboot_header
{
    /* Must be MULTIBOOT_MAGIC */
    uint32_t magic;

    /* Feature flags */
    uint32_t flags;

    /* Checksum: magic + flags + checksum == 0 */
    uint32_t checksum;

    /* Optional features, only if flags[16] is set */
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;
    /* Optional features, only if flags[2] is set */
    uint32_t mode_type;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
};

/* Multiboot header must be contained within the first MULTIBOOT_SEARCH
   bytes of the kernel image */
#define MULTIBOOT_SEARCH 8192

/* Magic value identifying the multiboot_header */
#define MULTIBOOT_MAGIC 0x1badb002

