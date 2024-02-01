/**
 * @file abort.c
 * @author Diogo Gomes
 * @brief Abort implementation, required by libgcc
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <klib/basic_io.h>
#include <klib/stdlib.h>
 
__attribute__((__noreturn__))
void abort(void) {
	// TODO: Add proper kernel panic.
	bprintf("kernel: panic: abort()\n");
    asm volatile("cli;"
                 "hlt;");

	while (1) { }
	__builtin_unreachable();
}