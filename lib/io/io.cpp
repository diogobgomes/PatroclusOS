/**
 * @file io.cpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Definitions from io.hpp
 * @version 0.1
 * @date 2024-01-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <klib/io.hpp>
#include <klib/cstdlib.hpp>
#include <stddef.h>
#include <stdint.h>

/*template <class backEnd> size_t io::_outstream<backEnd>::writeHex(uint64_t num)
{
    char str[MAX_NUM_STR_SIZE] = utoa(num, str, 16);
    return writeString(str);
}*/

/*template <class backEnd> size_t io::_outstream<backEnd>::writeInt(int64_t num)
{
    //char str[MAX_NUM_STR_SIZE] = itoa(num, str, 10);
    //char str[MAX_NUM_STR_SIZE] = "\0";
    //return writeString(str);
    return NULL;
}*/

template <class backEnd> size_t io::_outstream<backEnd>::writeNum(int64_t num, int base)
{
    char str[MAX_NUM_STR_SIZE] = itoa(num,str,base);
    return writeString(str);
}

