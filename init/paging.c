/**
 * @file paging.c
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Definitions to set up early paging in init
 * @version 0.1
 * @date 2024-02-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdint.h>
#include <stddef.h>
#include "paging.h"

void setupPaging(void* ptr)
{
    uint64_t* pointer = (uint64_t*) ptr;
    fillPages(pointer);
    enablePaging(pointer);
}

void fillPages(uint64_t* ptr)
{
    // We receive one pointer, but it's actually
    // p4 table - 4096 bytes
    // p3 table - 4096 bytes
    // p2 table - 4096 bytes
    // p2_ident - 4096 bytes
    uint64_t* p4 = ptr;
    uint64_t* p3 = ptr + 512;
    uint64_t* p3_ident = ptr + 2*512;
    uint64_t* p2 = ptr + 3*512;
    uint64_t* p2_ident = ptr + 4*512;

    // Fill p2 up
    p2[0] = (2 * 0x200000) | 0b10000011; // 4 MiB
    p2[1] = 0 | 0b10000011; // 0 MiB
    p2[2] = 0x200000 | 0b10000011; // 2 MiB

    // Now put it in second to last p3 entry
    p3[510] = ((uint32_t) p2) | 3;

    // And put it on the last p4 entry
    p4[511] = ((uint32_t) p3) | 3;

    // Fill p2_ident up
    for (size_t i = 0; i <3; i++)
    {
        p2_ident[i] = (i * 0x200000) | 0b10000011;
    }

    // Put p2_ident in p3_ident
    p3_ident[0] = ((uint32_t) p2_ident) | 3;

    // Put p3 in p4
    p4[0] = ((uint32_t) p3_ident) | 3;
}