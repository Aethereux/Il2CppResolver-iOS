//
//  Il2cppArray.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//

#pragma once

#include "Il2Cpp.h"

/*
    This struct can hold a native C# array. Credits to caoyin.
    Think of it like a wrapper for a C array. For example, if you had Player[] players in a dump,
    the resulting Il2CppArray definition would be Il2CppArray<void **> *players;
    To get the C array, call getPointer.
    To get the length, call getLength.
*/

template <typename T>
struct Il2CppArray : Il2CppObject
{
    Il2CppArrayBounds* m_pBounds = nullptr;
    uintptr_t m_uMaxLength = 0;
    T* m_pValues = nullptr;
    
    FORCEINLINE uintptr_t GetData()
    {
        return reinterpret_cast<uintptr_t>(&m_pValues);
    }

    T& operator[](unsigned int m_uIndex)
    {
        return *reinterpret_cast<T*>(GetData() + sizeof(T) * m_uIndex);
    }

    T& At(unsigned int m_uIndex)
    {
        return operator[](m_uIndex);
    }
    
    void Insert(T* m_pArray, uintptr_t m_uSize, uintptr_t m_uIndex = 0)
    {
        if ((m_uSize + m_uIndex) >= m_uMaxLength)
        {
            if (m_uIndex >= m_uMaxLength)
                return;

            m_uSize = m_uMaxLength - m_uIndex;
        }

        for (uintptr_t u = 0; m_uSize > u; ++u)
            operator[](u + m_uIndex) = m_pArray[u];
    }
    
    void Fill(T m_tValue)
    {
        for (uintptr_t u = 0; m_uMaxLength > u; ++u)
            operator[](u) = m_tValue;
    }
    
    void RemoveAt(unsigned int m_uIndex)
    {
        if (m_uIndex >= m_uMaxLength)
            return;

        if (m_uMaxLength > (m_uIndex + 1))
        {
            for (unsigned int u = m_uIndex; (static_cast<unsigned int>(m_uMaxLength) - m_uIndex) > u; ++u)
                operator[](u) = operator[](u + 1);
        }

        --m_uMaxLength;
    }
    
    void RemoveRange(unsigned int m_uIndex, unsigned int m_uCount)
    {
        if (m_uCount == 0)
            m_uCount = 1;

        unsigned int m_uTotal = m_uIndex + m_uCount;
        if (m_uTotal >= m_uMaxLength)
            return;

        if (m_uMaxLength > (m_uTotal + 1))
        {
            for (unsigned int u = m_uIndex; (static_cast<unsigned int>(m_uMaxLength) - m_uTotal) >= u; ++u)
                operator[](u) = operator[](u + m_uCount);
        }

        m_uMaxLength -= m_uCount;
    }
    
    void RemoveAll()
    {
        if (m_uMaxLength > 0)
        {
            memset(GetData(), 0, sizeof(T) * m_uMaxLength);
            m_uMaxLength = 0;
        }
    }
};

template<typename T>
using MonoArray = Il2CppArray<T>;

/*
    This struct represents a List. In the dump, a List is declared as List.
    Deep down, this simply wraps a C array around a C# list. For example, if you had this in a dump,
    public class Player {
        List<int> perks; // 0xDC
    }

    getting that list would look like this: MonoList<int *> *perks = *(MonoList<int *> **)((uint64_t)player + 0xdc);
    You can also get lists that hold objects, but you need to use void ** because we don't have implementation for the Weapon class.
    public class Player {
        List`1<Weapon> weapons; // 0xDC
    }
    getting that list would look like this: monoList<void **> *weapons = *(monoList<void **> **)((uint64_t)player + 0xdc);
    If you need a list of strings, use monoString **.
    To get the C array, call getItems.
    To get the size of a monoList, call getSize.
*/

template <typename T>
struct Il2CppList : Il2CppObject {
    MonoArray<T>* m_pListArray;
    
    FORCEINLINE MonoArray<T>* ToArray() { return m_pListArray; }
};

template<typename T>
using MonoList = Il2CppList<T>;
