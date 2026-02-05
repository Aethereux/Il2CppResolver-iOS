//
//  StringLiteral.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 2/5/26.
//

#pragma once
#include <string>
#include <algorithm>

template<int32_t Len>
struct StringLiteral
{
    char Chars[Len];
    
    consteval StringLiteral(const char(&String)[Len])
    {
        std::copy_n(String, Len, Chars);
    }
    
    operator std::string() const
    {
        return static_cast<const char*>(Chars);
    }
    
    operator const char*() const
    {
        return static_cast<const char*>(Chars);
    }
};
