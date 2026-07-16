#ifndef _WCHAR_H
#define _WCHAR_H

#include <stddef.h>

// On 32-bit/i386 systems, wchar_t is typically just an unsigned short (16-bit) 
// or an int (32-bit). We'll define it safely here if the compiler hasn't already.
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
typedef unsigned short wchar_t; // 16 bits of character goodness
#endif

#ifndef _WINT_T_DEFINED
#define _WINT_T_DEFINED
typedef unsigned int wint_t;   // Big enough to hold any wide char plus WEOF
#endif

#define WEOF (wint_t)(0xFFFF)  // The "nothing left to read" signal for wide chars

// Fast inline function: Get the length of a wide string (like strlen but for wide chars)
static inline size_t wcslen(const wchar_t *s) {
    const wchar_t *p = s;
    while (*p) {
        p++; // Keep moving until we hit the null terminator (0)
    }
    return p - s;
}

// Fast inline function: Copy a wide string from src to dest
static inline wchar_t *wcscpy(wchar_t *dest, const wchar_t *src) {
    wchar_t *d = dest;
    while ((*d++ = *src++)) {
        // Just copying character by character
    }
    return dest;
}

#endif
