/**
 * @file tracemax.hpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Functions to be used with the TRACEMAX definition
 * @version 0.1
 * @date 2024-02-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <klib/io.hpp>

// We need an _outstream class
template<class backEnd>
class tracemax
{
private:
    backEnd* _backEnd;
public:
    template<typename T> auto &operator<<(T in){
        *_backEnd << in;
        return *this;
    }

    void init(backEnd* ptr) { _backEnd = ptr; }
};

extern tracemax<io::_outstream<io::framebuffer_terminal>> traceOut;