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

#include <klib/stdlib.h>
 
__attribute__((__noreturn__))
void abort(void) {
	// TODO: Add proper kernel panic.
	earlyPanic("kernel: panic: abort()");
    __asm__ __volatile__("cli;"
                 "hlt;");

	while (1) { }
	__builtin_unreachable();
}