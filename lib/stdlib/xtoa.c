/**
 * @file xtoa.c
 * @author Diogo Gomes
 * @brief Definitions for the *toa() functions
 * @version 0.1
 * @date 2024-01-07
 * 
 */

#include <klib/stdlib.h>
#include <stdint.h>
#include <stddef.h>

char* itoa(int value, char* str, int base)
{
    // Test base
    if (base < 2 || base > 16)
    {
        return NULL;
    }

    // Check edge case of value == 0
    if (value == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    

    int num = value;
    size_t index = 0;
    for (; num != 0; ++index)
    {
        char digit = (char) (num % base); // Guaranteed between 0 and base
        if (digit < 10)
        {
            str[index] = '0' + digit;
        }
        else
        {
            str[index] = 'A' + digit - 10;
        }
        num /= base;
    }

    if (base == 10 && value < 0)
    {
        str[index] = '-';
        index++;
    }

    // Terminate string
    str[index--] = '\0';

    // Now invert the string (it's the wrong way around)
    for (size_t i = 0; i < index/2+1; i++)
    {
         char tmp = str[i];
         str[i] = str[index-i];
         str[index-i] = tmp;
    }

    // Now we have everything
    return str;
    
}


char* utoa(unsigned int value, char* str, int base)
{
    // Test base
    if (base < 2 || base > 16)
    {
        return NULL;
    }

    // Check edge case of value == 0
    if (value == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    unsigned int num = value;
    size_t index = 0;
    for (; num != 0; ++index)
    {
        char digit = (char) (num % (unsigned int) base); // Guaranteed to be between 0 and base, base is positive
        if (digit < 10)
        {
            str[index] = '0' + digit;
        }
        else
        {
            str[index] = 'A' + digit - 10;
        }
        num /= (unsigned int) base; // Base is positive
    }


    // Terminate string
    str[index--] = '\0';

    // Now invert the string (it's the wrong way around)
    for (size_t i = 0; i < index/2+1; i++)
    {
         char tmp = str[i];
         str[i] = str[index-i];
         str[index-i] = tmp;
    }

    // Now we have everything
    return str;
    
}