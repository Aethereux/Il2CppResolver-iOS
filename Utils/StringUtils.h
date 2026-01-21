//
//  StringUtils.h
//  AsuraFF
//
//  Created by Euclid Jan Guillermo on 1/15/26.
//
#pragma once


#include "Macros.h"
#include "Core.h"
#include <__config>
#include <iostream>
#include <chrono>
#include <string>
#include <cstdarg>
#include <cwchar>
#include <memory>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <cwchar>
#include <iterator>
#include <type_traits>

template<typename CharType>
struct TStringUtils
{

    static constexpr FORCEINLINE uint32 ToUnsigned(CharType Char)
    {
        return (std::make_unsigned_t<CharType>)Char;
    }

    static CharType ToUpper(CharType Char)
    {
        return (CharType)(ToUnsigned(Char) - ((uint32(Char) - 'a' < 26u) << 5));
    }

    static CharType ToLower(CharType Char)
    {
        return (CharType)(ToUnsigned(Char) + ((uint32(Char) - 'A' < 26u) << 5));
    }

    static constexpr uint8 LowerAscii[128] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
        'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F
    };

    template<typename CharType1, typename CharType2>
    static FORCEINLINE bool BothAscii(CharType1 C1, CharType2 C2)
    {
        return (((uint32)C1 | (uint32)C2) & 0xffffff80) == 0;
    }

    static int32 Strlen(const CharType* String)
    {
        int32 Length = -1;
        do
        {
            Length++;
        }
        while (*String++);

        return Length;
    }

    static int32 Strcmp(const CharType* String1, const CharType* String2)
    {
        // walk the strings, comparing them case sensitively
        for (; *String1 || *String2; String1++, String2++)
        {
            CharType A = *String1, B = *String2;
            if (A != B)
            {
                return A - B;
            }
        }
        return 0;
    }

    static int32 Strncmp(const CharType* String1, const CharType* String2, size_t Count)
    {
        // walk the strings, comparing them case sensitively, up to a max size
        for (; (*String1 || *String2) && Count; String1++, String2++, Count--)
        {
            CharType A = *String1, B = *String2;
            if (A != B)
            {
                return A - B;
            }
        }
        return 0;
    }

    static int32 Strnicmp(const CharType* String1, const CharType* String2, size_t Count)
    {
        for (; Count > 0; --Count)
        {
            // Quickly move on if characters are identical but
            // return equals if we found two null terminators
            CharType C1 = *String1++;
            CharType C2 = *String2++;

            if (C1 == C2)
            {
                if (C1)
                {
                    continue;
                }

                return 0;
            }
            else if (BothAscii(C1, C2))
            {
                if (int32 Diff = LowerAscii[ToUnsigned(C1)] - LowerAscii[ToUnsigned(C2)])
                {
                    return Diff;
                }
            }
            else
            {
                return ToUnsigned(C1) - ToUnsigned(C2);
            }
        }

        return 0;
    }

    static const CharType* Stristr(const CharType* Str, const CharType* Find)
    {
        // both strings must be valid
        if (Find == NULL || Str == NULL)
        {
            return NULL;
        }

        // get upper-case first letter of the find string (to reduce the number of full strnicmps)
        CharType FindInitial = ToUpper(*Find);
        // get length of find string, and increment past first letter
        int32   Length = Strlen(++Find) - 1;
        // get the first letter of the search string, and increment past it
        CharType StrChar = *Str++;
        // while we aren't at end of string...
        while (StrChar)
        {
            // make sure it's upper-case
            StrChar = ToUpper(StrChar);
            // if it matches the first letter of the find string, do a case-insensitive string compare for the length of the find string
            if (StrChar == FindInitial && !Strnicmp(Str, Find, Length))
            {
                // if we found the string, then return a pointer to the beginning of it in the search string
                return Str - 1;
            }
            // go to next letter
            StrChar = *Str++;
        }
        
        // if nothing was found, return NULL
        return NULL;
    }

    static CharType* Stristr(CharType* String, const CharType* Find)
    {
        return const_cast<CharType*>(Stristr((const CharType*)String, Find));
    }

    static const CharType* Strstr(const CharType* String, const CharType* Find)
    {
        CharType Char1, Char2;
        if ((Char1 = *Find++) != 0)
        {
            size_t Length = Strlen(Find);

            do
            {
                do
                {
                    if ((Char2 = *String++) == 0)
                    {
                        return nullptr;
                    }
                }
                while (Char1 != Char2);
            }
            while (Strncmp(String, Find, Length) != 0);

            String--;
        }

        return String;
    }

    static CharType* Strstr(CharType* String, const CharType* Find)
    {
        return const_cast<CharType*>(Strstr((const CharType*)String, Find));
    }

};

typedef TStringUtils<char> UTF8Utils;
typedef TStringUtils<char16_t> UTF16Utils;
typedef TStringUtils<wchar_t> UTF32Utils;
