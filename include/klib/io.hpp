/**
 * @file io.hpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Basic input/output system, a try at a decent, extensible one
 * @version 0.1
 * @date 2024-01-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _IO_HPP
#define _IO_HPP 1

//#pragma once
#include <stddef.h>
#include <stdint.h>
#include <klib/cstdlib.hpp>
#include <klib/string.h>

namespace io
{

template<class backEnd>
class _outstream
{
private:
    backEnd* _backEnd;

public:
    void clear() { _backEnd->clear(); }

    void init(backEnd* aaaa) { _backEnd = aaaa; /*_backEnd->init();*/ }

    //auto &getBackEnd() { return _backEnd; }

    size_t writeString(const char *str) {
        size_t i = 0;
        for(size_t size = strlen(str);i<size;i++){
            _backEnd->putchar(*(str++));}
        return i;
    }

    size_t writeString(const char *str, size_t size) {
        size_t i = 0;
        for (; i < size; i++)
        {
            _backEnd->putchar(str[i]);
        }
        return i;
    }

    size_t writeHex( uint64_t num) {
        char str[MAX_NUM_STR_SIZE];
        utoa64(num,str,10);
        return writeString(str);
    }

    size_t writeInt( int32_t num) {
        char str[MAX_NUM_STR_SIZE];
        itoa(num,str,10);
        return writeString(str);
    }

    //size_t writeUInt(uint64_t num);

    size_t writeNum( int64_t num, int base);

    auto &operator<<(const char* str) {
        writeString(str);
        return *this;
    }

    auto &operator<<(uint64_t num) {
        writeHex(num);
        return *this;
    }

    auto &operator<<(int64_t num) {
        writeInt(num);
        return *this;
    }

    auto &operator<<(void* ptr) {
        writeHex(reinterpret_cast<uint64_t>(ptr));
        return *this;
    }



}; // End of class "_outstream"

} // End of namespace "io"

#endif