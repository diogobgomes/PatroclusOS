/**
 * @file hidden/stdio_format.c
 * @author Diogo Gomes
 * @brief Definitions for outFormat
 * @version 0.1
 * @date 2024-01-06
 * 
 */

#include <klib/hidden/stdio_format.h>
#include <stdarg.h>
#include <klib/string.h>
#include <klib/stdlib.h>


//#define MAX_STR_SIZE 3000

size_t outFormat(const char* fmt, va_list parameters, int (*putFunc)(const char*,size_t))
{
    // Size of the string
    size_t length = strlen(fmt);

    // Index to keep track of what has been written
    size_t written = 0;

    for( size_t index = 0;
            index < length;
            ++index)
    {
        char c = fmt[index];
        //char* testPointer = &c;
        
        if (c != '%') // Normal character
        { // TODO make a string, or do this with seeking next control char, don't call this func every char
            putFunc(&c,1);
            ++written;
            continue;
        }

        // Check edge case where % is the last character
        else if (! fmt[++index] )
        {
            return written;
        }

        // Now it's either an escape, or a format
        switch (fmt[index])
        {
        char ch;
        const char* cstr;
        size_t cstrLength;
        char str[MAX_INT_STR_SIZE];
        size_t strLength;
        int num;
        unsigned int unNum;
        case '%': // Escape
            putFunc(fmt + index,1);
            ++written;
            break;

        case 'c': // Character
            //TODO check result of putFunc for error
            ch = (char) va_arg(parameters,int);
            putFunc(&ch,1);
            ++written;
            break; 
        
        case 's': //String
            cstr = va_arg(parameters, const char *);
            cstrLength = strlen(cstr);
            putFunc(cstr,cstrLength);
            written += cstrLength;
            break;

        case 'i': // Integer
            num = va_arg(parameters,int);
            if ( ! itoa (num, str,10) )
            {
                return written;
            }
            strLength = strlen(str);
            putFunc(str,strLength);
            written += strLength;
            break;
        
        case 'x': // Hexadecimal
            unNum = va_arg(parameters,unsigned int);
            // Using utoa for very large numbers
            if ( ! utoa (unNum, str, 16) )
            {
                return written;
            }
            strLength = strlen(str);
            putFunc(str,strLength);
            written += strLength;
            break;

        default:
            putFunc(fmt + index - 1,1);
            ++written;
            break;
        }
    }

    return written;
}
