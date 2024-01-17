/**
 * @file kernel.h
 * @author Diogo Gomes
 * @brief Main kernel functions
 * @version 0.1
 * @date 2024-01-13
 *
 */

#ifndef __KERNEL_H
#define __KERNEL_H 1

#ifdef __cplusplus
extern "C" {
#endif

// Macros for handling definition of inline functions
#ifndef INLINE
# define INLINE static inline
#endif

#include <stdint.h>
#include <stddef.h>
#include <sys/multiboot.h>

void kmain(uint32_t multibootMagic, multiboot_info_structure* multibootInfo);

#ifdef __cplusplus
}
#endif

#endif