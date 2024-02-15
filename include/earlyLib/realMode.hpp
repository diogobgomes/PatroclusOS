/**
 * @file realMode.hpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Functions to run code in 16-bit real mode
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

extern "C" {
    void realModeCall(...);
}

