/**
 * @file paging.hpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Setting up early paging in init
 * @version 0.1
 * @date 2024-02-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Main function, sets up paging, and enables it
 * 
 * @param ptr Pointer to memory with 16384 (16KiB) free, to fill up with paging
 */
void setupPaging( void* ptr );

/**
 * @brief Fills up the pages with our special mapping
 * 
 * @param ptr Pointer to memory with 16384 (16KiB) free, to fill up with paging
 */
void fillPages( uint64_t* ptr );

/**
 * @brief Assembly function, enables paging
 * 
 * @param ptr Pointer to p4_table
 */
extern void enablePaging( uint64_t* ptr );


#ifdef __cplusplus
}
#endif
