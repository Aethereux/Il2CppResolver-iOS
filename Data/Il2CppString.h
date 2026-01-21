//
//  Il2CppString.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once
#include <cstdint>
#include "Il2Cpp.h"
#include "Il2CppArray.h"
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
    char  chars[1];

    FORCEINLINE int GetLength() const
    {
        return length;
    }

    FORCEINLINE const char16_t* GetChars() const
    {
        return reinterpret_cast<const char16_t*>(chars);
    }

    FORCEINLINE std::string ToCppString() const
    {
        static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        return convert.to_bytes(GetChars(), GetChars() + length);
    }


    FORCEINLINE const char* ToCString() const
    {
        static thread_local std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        static thread_local std::string temp;
        temp = convert.to_bytes(GetChars(), GetChars() + length);
        return temp.c_str();
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
template<typename T>
using MonoString = Il2CppList<T>;
