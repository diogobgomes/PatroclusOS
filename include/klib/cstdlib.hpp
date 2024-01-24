/**
 * @file cstdlib.hpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief stdlib for cpp
 * @version 0.1
 * @date 2024-01-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

//#pragma once

#ifndef _CSTDLIB_HPP
#define _CSTDLIB_HPP 1

#include <stdint.h>
#include <stddef.h>

#define MAX_NUM_STR_SIZE 65

char* itoa(int32_t value, char* str, int base);

char* utoa(uint32_t value, char* str, int base);

char* utoa64(uint64_t value, char* str, int base);

#endif