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
#include "multiboot.h"




/**
 * @brief Multiboot loader entry point
 * 
 * @param ptr Pointer to the start of the kernel. Since this is called through assembly, this must be
 * pushed to the stack
 * @return uint8_t 0 if failed, 1 if the kernel was successfully loaded
 */
uint32_t _multiboot_main(uint32_t ptr) {
    //__asm__("cli;""hlt;");

    // Right now, this is just a proof of concept that calls to C work

    //uint32_t a = 0;
    return ptr;
}