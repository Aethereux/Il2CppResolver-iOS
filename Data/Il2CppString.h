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
#include "../Utils/utf8/unchecked.h"

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
        if (length <= 0 || length > 10000) return;
        
        ReturnVal.reserve(length * 1.5);
        utf8::unchecked::utf16to8(chars, chars + length, std::back_inserter(ReturnVal));
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
