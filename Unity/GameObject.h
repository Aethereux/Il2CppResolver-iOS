//
//  GameObject.h
//  Asura
//
//  Created by Eux on 1/21/26.
//
#pragma once

#include "../Defines.h"
#include "../Data/Il2Cpp.h"
#include "../Data/Il2CppString.h"
#include "../Data/Il2CppArray.h"
#include "../Runtime/ResolveCall.h"
#include "Object.h"

class CComponent;
class CTransform;

enum class m_ePrimitiveType : int
{
    Default = 0,
    Sphere = 0,
    Capsule,
    Cylinder,
    Cube,
    Plane,
    Quad,
};

struct GameObjectFunctions_t
{
    void (*AddComponent)(void*, void*) = nullptr;
    void* (*CreatePrimitive)(m_ePrimitiveType) = nullptr;
    void* (*Find)(Il2CppString*) = nullptr;
    Il2CppArray<class CGameObject*>* (*FindGameObjectsWithTag)(void*) = nullptr;
    CComponent* (*GetComponent)(void*, Il2CppString*) = nullptr;
    Il2CppArray<CComponent*>* (*GetComponents)(void*, void*, bool, bool, bool, bool, void*) = nullptr;
    CComponent* (*GetComponentInChildren)(void*, void*, bool) = nullptr;
    bool (*GetActive)(void*) = nullptr;
    unsigned int (*GetLayer)(void*) = nullptr;
    CTransform* (*GetTransform)(void*) = nullptr;
    void (*SetActive)(void*, bool) = nullptr;
    void (*SetLayer)(void*, unsigned int) = nullptr;
};

inline GameObjectFunctions_t GameObjectFunctions;
class CGameObject : public CObject
{
public:
    FORCEINLINE void AddComponent(Il2CppObject* m_pSystemType)
    {
        if (!GameObjectFunctions.AddComponent)
            return;
        
        GameObjectFunctions.AddComponent(this, m_pSystemType);
    }

    FORCEINLINE CComponent* GetComponent(const char* m_pName)
    {
        if (!GameObjectFunctions.GetComponent)
            return nullptr;
        
        return GameObjectFunctions.GetComponent(this, String::New(m_pName));
    }

    FORCEINLINE CComponent* GetComponentInChildren(Il2CppObject* m_pSystemType, bool includeInactive)
    {
        if (!GameObjectFunctions.GetComponentInChildren)
            return nullptr;
        
        return GameObjectFunctions.GetComponentInChildren(this, m_pSystemType, includeInactive);
    }

    FORCEINLINE CComponent* GetComponentInChildren(const char* type)
    {
        Il2CppClass* m_pClass = UnityClass::Find(type);
        if (!m_pClass) return nullptr;

        return GetComponentInChildren(UnityClass::GetSystemType(m_pClass), true);
    }

    FORCEINLINE Il2CppArray<CComponent*>* GetComponents(Il2CppObject* m_pSystemType)
    {
        /*
        0 - Object
        1 - Type
        2 - Use search type as array return type
        3 - Recursive
        4 - Include inactive
        5 - Reverse
        6 - Result list
        */
        if (!GameObjectFunctions.GetComponents)
            return nullptr;
        
        return GameObjectFunctions.GetComponents(this, m_pSystemType, false, false, true, false, nullptr);
    }

    FORCEINLINE Il2CppArray<CComponent*>* GetComponents(const char* m_pSystemTypeName)
    {
        Il2CppClass* m_pClass = UnityClass::Find(m_pSystemTypeName);
        if (!m_pClass) return nullptr;

        return (Il2CppArray<CComponent*>*)GetComponents(UnityClass::GetSystemType(m_pClass));
    }

    FORCEINLINE CComponent* GetComponentByIndex(Il2CppObject* m_pSystemType, unsigned int m_uIndex = 0U)
    {
        Il2CppArray<CComponent*>* m_pComponents = GetComponents(m_pSystemType);
        if (!m_pComponents || m_uIndex >= m_pComponents->m_uMaxLength) return nullptr;

        return (CComponent*)m_pComponents->operator[](m_uIndex);
    }

    FORCEINLINE CComponent* GetComponentByIndex(const char* m_pSystemTypeName, unsigned int m_uIndex = 0U)
    {
        Il2CppClass* m_pClass = UnityClass::Find(m_pSystemTypeName);
        if (!m_pClass) return nullptr;

        return GetComponentByIndex(UnityClass::GetSystemType(m_pClass), m_uIndex);
    }

    FORCEINLINE CTransform* GetTransform()
    {
        if (!GameObjectFunctions.GetTransform)
            return nullptr;
        
        return GameObjectFunctions.GetTransform(this);
    }

    FORCEINLINE bool GetActive()
    {
        if (!GameObjectFunctions.GetActive)
            return false;
        
        return GameObjectFunctions.GetActive(this);
    }

    FORCEINLINE unsigned int GetLayer()
    {
        if (!GameObjectFunctions.GetLayer)
            return -1;
        
        return GameObjectFunctions.GetLayer(this);
    }

    FORCEINLINE void SetActive(bool m_bActive)
    {
        if (!GameObjectFunctions.SetActive)
            return;
        
        GameObjectFunctions.SetActive(this, m_bActive);
    }

    FORCEINLINE void SetLayer(unsigned int m_uLayer)
    {
        if (!GameObjectFunctions.SetLayer)
            return;
        GameObjectFunctions.SetLayer(this, m_uLayer);
    }
};


namespace GameObject
{
    void Initialize();
    
    FORCEINLINE CGameObject* CreatePrimitive(m_ePrimitiveType m_Type)
    {
        if (!GameObjectFunctions.CreatePrimitive)
            return nullptr;
        
        return (CGameObject*)GameObjectFunctions.CreatePrimitive(m_Type);
    }

    FORCEINLINE CGameObject* Find(const char* m_Name)
    {
        if (!GameObjectFunctions.Find)
            return nullptr;
        
        return (CGameObject*)GameObjectFunctions.Find(String::New(m_Name));
    }

    FORCEINLINE Il2CppArray<CGameObject*>* FindWithTag(const char* m_Tag)
    {
        if (!GameObjectFunctions.FindGameObjectsWithTag)
            return nullptr;
        
        return (Il2CppArray<CGameObject*>*)GameObjectFunctions.FindGameObjectsWithTag(String::New(m_Tag));
    }
}

