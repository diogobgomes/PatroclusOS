/**
 * @file stdlib.h
 * @author Diogo Gomes
 * @brief General purpose C library
 * @version 0.1
 * @date 2024-01-06
 * 
 */

#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Max size a number as a string can be
#define MAX_INT_STR_SIZE 33

/**
 * @brief Converts a number into a string
 * 
 * @param value Value to convert
 * @param str Should be an array big enough to contain any possible value,
 * it's MAX_INT_STR_SIZE
 * @param base Number between 2 and 16 (should be 36, but eh). If it's base 10,
 * can be positive or negative, any other base assume positive
 * @return char* Same as str, NULL for error
 */
//char* itoa(int value, char* str, int base);

/**
 * @brief Converts a number into a string
 * 
 * @param value Value to be converted
 * @param str Should be an array big enough to contain any possible value,
 * it's MAX_INT_STR_SIZE
 * @param base Number between 2 and 16 (should be 36, but eh)
 * @return char* Same as str, NULL for error
 */
//char* utoa(unsigned int value, char* str, int base);

/**
 * @brief Abort function, required by libgcc
 * 
 */
__attribute__((__noreturn__))
void abort(void);

[[noreturn,maybe_unused]] void earlyPanic(const char* str);
[[noreturn,maybe_unused]] inline static void hang()
{
    while(true)
    __asm__ __volatile__ (  "xchgw %bx, %bx\r\n"
                            "cli\r\n"
                            "hlt");
}


#ifdef __cplusplus
}
#endif


