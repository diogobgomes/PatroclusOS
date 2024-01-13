/**
 * @file hidden/stdio_format.h
 * @author Diogo Gomes
 * @brief Contains functions useful for stdio, that do all the processing. That
 * way, it's easy to write multiple stdio wrappers
 * @version 0.1
 * @date 2024-01-06
 * 
 */

#ifndef _HIDDEN_STDIO_FORMAT_H
#define _HIDDEN_STDIO_FORMAT_H 1

#ifdef __cplusplus
extern "C" {
#endif

// Macros for handling definition of inline functions
#ifndef INLINE
# define INLINE static inline
#endif

#include <stdarg.h>
#include <stddef.h>

size_t outFormat(const char* fmt, va_list parameters, int (*putfunc)(const char* ,size_t));


#ifdef __cplusplus
}
#endif

#endif