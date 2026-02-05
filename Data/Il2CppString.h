//
//  Il2CppString.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once
#include <cstdint>
#include "Il2Cpp.h"
#include "../Globals.h"

/*
This struct represents a C# string. Credits to caoyin.
It is pretty straight forward. If you have this in a dump,
public class Player {
    public string username; // 0xC8
}
getting that string would look like this: MonoString *username = *(MonoString **)((uint64_t)player + 0xc8);
 
C# strings are UTF-16. This means each character is two bytes instead of one.
To get the length of a monoString, call GetLength.
To get a std::string from a monoString, call ToCppString.
To get a C string from a monoString, call ToCString.
*/

struct Il2CppString : Il2CppObject
{
    int   length;
    char16_t  chars[1];

    FORCEINLINE std::string ToCppString() const {
        std::string ReturnVal;
        ToUtf8(ReturnVal);
        return ReturnVal;
    }

    FORCEINLINE const char* ToCString() const {
        // thread_local prevents race conditions between different threads
        static thread_local std::string buffer;
        ToUtf8(buffer);
        return buffer.c_str();
    }

    void ToUtf8(std::string& ReturnVal) const {
        ReturnVal.clear();
        // If length is massive or negative, the offset is definitely wrong
        if (length <= 0 || length > 10000) return;
        ReturnVal.reserve(length * 3);
        for (int i = 0; i < length; ++i) {
            uint32_t codePoint = chars[i];
            if (codePoint >= 0xD800 && codePoint <= 0xDBFF && i + 1 < length) {
                uint32_t low = chars[i + 1];
                if (low >= 0xDC00 && low <= 0xDFFF) {
                    codePoint = ((codePoint - 0xD800) << 10) + (low - 0xDC00) + 0x10000;
                    i++;
                }
            }
            if (codePoint < 0x80) ReturnVal.push_back(static_cast<char>(codePoint));
            else if (codePoint < 0x800) {
                ReturnVal.push_back(static_cast<char>(0xC0 | (codePoint >> 6)));
                ReturnVal.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            } else if (codePoint < 0x10000) {
                ReturnVal.push_back(static_cast<char>(0xE0 | (codePoint >> 12)));
                ReturnVal.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                ReturnVal.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            } else {
                ReturnVal.push_back(static_cast<char>(0xF0 | (codePoint >> 18)));
                ReturnVal.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
                ReturnVal.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                ReturnVal.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            }
        }
    }
};

namespace String {
    FORCEINLINE Il2CppString* New(const char* m_String)
    {
        if (!Functions.StringNew)
            return nullptr;
        
        return Functions.StringNew(m_String);
    }

    FORCEINLINE Il2CppString* New(std::string m_String)
    {
        return New(&m_String[0]);
    }
}

using MonoString = Il2CppString;
