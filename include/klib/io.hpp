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

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <klib/cstdlib.hpp>
#include <klib/string.h>
#include <devices/framebuffer_io.hpp>
#include <stdarg.h>

extern "C" size_t outFormat(const char* fmt, va_list parameters, int (*putfunc)(const char* ,size_t));

namespace io
{

template<class backEnd>
class _outstream
{
private:
    backEnd* _backEnd;
    int _base;

public:
    void clear() { _backEnd->clear(); }

    void init(backEnd* aaaa) { _backEnd = aaaa; _base=10; /*_backEnd->init();*/ }

    backEnd* getBackEnd() { return _backEnd; }

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
        xtoa(num,str,16);
        return writeString(str);
    }

    size_t writeInt( int64_t num) {
        char str[MAX_NUM_STR_SIZE];
        xtoa(num,str,10);
        return writeString(str);
    }

    //size_t writeUInt(uint64_t num);

    /*
    size_t writeNum( int64_t num, int base) {
        char str[MAX_NUM_STR_SIZE];
        itoa(num,str,base);
        return writeString(str);
    }*/

    template<typename T> size_t writeNum( T num ) {
        char str[MAX_NUM_STR_SIZE];
        xtoa(num,str,_base);
        return writeString(str);
    }

    int changeBase(int num) {
        if (num < 2 || num > 16)
        {
            return -1;
        }
        _base = num;
        return base;
    }

    auto &operator<<(const char* str) {
        writeString(str);
        return *this;
    }

    template<typename T> auto &operator<<(T num) {
        writeNum(num);
        return *this;
    }

    auto &hex() { _base=16; return *this;}

    auto &dec() { _base=10; return *this;}

    auto &base(int num) {
        if (num < 2 || num > 16)
        {
            return *this; // Don't change it
            // TODO Maybe static_assert?
        }
        _base = num; return *this;
    }

    auto &operator<<(_outstream&) {return *this;}

}; // End of class "_outstream"

} // End of namespace "io"

extern io::_outstream<framebuffer_io::framebuffer_terminal> icout;
