/**
 * @file multiboot.h
 * @author Diogo Gomes
 * @brief Header file for the multiboot loader program, defines useful structures
 * @version 0.1
 * @date 2023-11-10
 * 
 */

#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Multiboot header structure, that we must search for in the kernel image
 * 
 */
struct multiboot_header
{
    /* Must be MULTIBOOT_MAGIC */
    uint32_t magic;

    /* Feature flags:
    flags[0] - All boot modules loaded along with the OS must be aligned on page (4KB)
               boundaries
    flags[1] - Info on available memory through mem_* (required) and mmap_* (optional)
               fields must be on the info structure
    flags[2] - Information about the video mode must be available to the kernel */
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

/**
 * @brief Multiboot info structure that we must pass to the kernel
 * 
 */
struct multiboot_info_structure
{
    /* Flags passed to the OS (required) */
    uint32_t flags;

    /* Sizes of lower and upper memory (present if flags[0] is set) */
    uint32_t mem_lower;
    uint32_t mem_upper;

    /* Boot device used, as understood by int 13h (present if flags[1] is set)*/
    uint32_t boot_device;

    /* (present if flags[2] is set) */
    uint32_t cmdline;

    /* (present if flags[3] is set) */
    uint32_t mods_count;
    uint32_t mods_addr;

    /* present if flags[4] or flags[5] is set */
    uint32_t syms_1; //TODO use union
    uint32_t syms_2;
    uint32_t syms_3;
    uint32_t syms_4;

    /* Memory map fields (present if flags[6] is set) */
    uint32_t mmap_length; // Length of the mmap structure we're passing
    uint32_t mmap_addr; // Address where structure is located

    /* (present if flags[7] is set) */
    uint32_t drives_length;
    uint32_t drives_addr;

    /* (present if flags[8] is set) */
    uint32_t config_table;

    /* Contains address of a C-terminated string with the bootloader name
    (present if flags[9] is set) */
    uint32_t boot_loader_name;

    /* (present if flags[10] is set) */
    uint32_t apm_table;

    /* (present if flags[11] is set) */
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    /* (present if flags[12] is set) */
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint32_t color_1;
    uint16_t color2;
};

struct mmap_structure_entry
{
    /* Length of entry, always use 24 for simplicity (extended ACPI) */
    uint32_t entry_length;

    /* Base address is the starting address for the entry */
    uint64_t base_addr;

    /* Length of the region in bytes */
    uint64_t length;

    /* Region type:
    1 - Usable (normal) RAM
    2 - Reserved, unusable
    3 - ACPI reclaimable memory
    4 - ACPI NVS memory
    5 - Area containing bad memory*/
    uint32_t type;

    /* We always have this for simplicity, and all 0 is just ignore this */
    uint32_t acpi_3_0_extended;
    
};

/*
struct mmap_structure_entry_write
{
    uint32
};*/




/* Multiboot header must be contained within the first MULTIBOOT_SEARCH
   bytes of the kernel image */
#define MULTIBOOT_SEARCH 8192

/* Multiboot header should be 4 byte aligned */
#define MULTIBOOT_HEADER_ALIGN 4

/* Magic value identifying the multiboot_header */
#define MULTIBOOT_MAGIC 0x1badb002

/* Multiboot info structure size */
#define MULTIBOOT_INFO_SIZE 120

//uint32_t* multiboot_header_search(uint32_t* ptr);

#ifdef __cplusplus
}
#endif

#endif
