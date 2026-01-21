//
//  Object.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//
#pragma once
#include "../Data/Il2CppArray.h"
#include "../Runtime/Class.h"
#include "../Runtime/ResolveCall.h"
#include "../Defines.h"


struct ObjectFunctions_t
{
    void (*Destroy)(void*, float) = nullptr;
    void* FindObjectsOfType = nullptr; // bruh
    Il2CppString* (*GetName)(void*) = nullptr;
};

inline ObjectFunctions_t ObjectFunctions;

class CObject : public UnityClass::CClass
{
public:
    FORCEINLINE void Destroy(float fTimeDelay = 0.f)
    {
        if (!ObjectFunctions.Destroy)
            return;
        
        ObjectFunctions.Destroy(this, fTimeDelay);
    }

    FORCEINLINE Il2CppString* GetName()
    {
        if (!ObjectFunctions.GetName)
            return nullptr;
        
        return ObjectFunctions.GetName(this);
    }
};

namespace Object
{
    void Initialize();
    static Il2CppObject* New(Il2CppClass* m_pClass);

    template<typename T>
    FORCEINLINE Il2CppArray<T*>* FindObjectsOfType(Il2CppObject* m_pSystemType, bool m_bIncludeInactive = false)
    {
        return reinterpret_cast<Il2CppArray<T*>*(*)(void*, bool)>(ObjectFunctions.FindObjectsOfType)(m_pSystemType, m_bIncludeInactive);
    }

    template<typename T>
    FORCEINLINE Il2CppArray<T*>* FindObjectsOfType(const char* m_pSystemTypeName, bool m_bIncludeInactive = false)
    {
        Il2CppClass* m_pClass = UnityClass::Find(m_pSystemTypeName);
        if (!m_pClass)
            return nullptr;

        return FindObjectsOfType<T>(UnityClass::GetSystemType(m_pClass), m_bIncludeInactive);
    }

    template<typename T>
    FORCEINLINE T* FindObjectOfType(Il2CppObject* m_pSystemType, bool m_bIncludeInactive = false)
    {
        Il2CppArray<T*>* m_pArray = FindObjectsOfType<T>(m_pSystemType, m_bIncludeInactive);
        if (!m_pArray || m_pArray->m_uMaxLength == 0U)
            return nullptr;

        return m_pArray->m_pValues[0];
    }

    template<typename T>
    FORCEINLINE T* FindObjectOfType(const char* m_pSystemTypeName, bool m_bIncludeInactive = false)
    {
        Il2CppClass* m_pClass = UnityClass::Find(m_pSystemTypeName);
        if (!m_pClass)
            return nullptr;

        return FindObjectOfType<T>(UnityClass::GetSystemType(m_pClass), m_bIncludeInactive);
    }
}
