//
//  Transform.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//
#pragma once
#include "../Defines.h"
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"
#include "../Runtime/Class.h"
#include "../Runtime/ResolveCall.h"
#include "../Data/Il2CppString.h"

struct TransformFunctions_t
{
    class CTransform* (*GetParent)(void*) = nullptr;
    class CTransform* (*GetRoot)(void*) = nullptr;
    class CTransform* (*GetChild)(void*, int) = nullptr;
    int (*GetChildCount)(void*) = nullptr;
    class CTransform* (*FindChild)(void*, Il2CppString*, bool) = nullptr;
    void (*GetPosition)(void*, Vector3&) = nullptr;
    Vector3 (*GetForward)(void*) = nullptr;
    void (*GetRotation)(void*, Quaternion&) = nullptr;
    void (*GetLocalPosition)(void*, Vector3&) = nullptr;
    void (*GetLocalScale)(void*, Vector3&) = nullptr;
    void (*SetPosition)(void*, Vector3&) = nullptr;
    void (*SetForward)(void*, Vector3) = nullptr;
    void (*SetRotation)(void*, Quaternion) = nullptr;
    void (*SetLocalPosition)(void*, Vector3) = nullptr;
    void (*SetLocalScale)(void*, Vector3) = nullptr;
};

inline TransformFunctions_t TransformFunctions;

class CTransform : public UnityClass::CClass
{
public:
    FORCEINLINE CTransform* GetParent()
    {
        if (!TransformFunctions.GetParent)
            return nullptr;
        
        return TransformFunctions.GetParent(this);
    }

    FORCEINLINE CTransform* GetRoot()
    {
        if (!TransformFunctions.GetRoot)
            return nullptr;
        
        return TransformFunctions.GetRoot(this);
    }
    
    FORCEINLINE CTransform* GetChild(int iIndex)
    {
        if (!TransformFunctions.GetChild)
            return nullptr;
        
        return TransformFunctions.GetChild(this, iIndex);
    }

    FORCEINLINE int GetChildCount()
    {
        if (!TransformFunctions.GetChildCount)
            return -1;
        
        return TransformFunctions.GetChildCount(this);
    }

    FORCEINLINE CTransform* FindChild(const char* path, bool isActiveOnly)
    {
        if (!TransformFunctions.FindChild)
            return nullptr;
        
        return TransformFunctions.FindChild(this, String::New(path), isActiveOnly);
    }

    FORCEINLINE CTransform* FindChild(const char* path)
    {
        if (path == NULL) {
            return nullptr;
        }
        return FindChild(path, false);
    }

    FORCEINLINE Vector3 GetPosition()
    {
        Vector3 vRet;
        if (!TransformFunctions.GetPosition)
            return vRet;
        
        TransformFunctions.GetPosition(this, vRet);
        return vRet;
    }
    
    FORCEINLINE Vector3 GetForward()
    {
        Vector3 vRet;
        if (!TransformFunctions.GetForward)
            return vRet;
        
        vRet = TransformFunctions.GetForward(this);
        return vRet;
    }
    
    FORCEINLINE Quaternion GetRotation()
    {
        Quaternion qRet;
        if (!TransformFunctions.GetRotation)
            return qRet;
        
        TransformFunctions.GetRotation(this, qRet);
        return qRet;
    }

    FORCEINLINE Vector3 GetLocalPosition()
    {
        Vector3 vRet;
        if (!TransformFunctions.GetLocalPosition)
            return vRet;
        
        TransformFunctions.GetLocalPosition(this, vRet);
        return vRet;
    }

    FORCEINLINE Vector3 GetLocalScale()
    {
        Vector3 vRet;
        if (!TransformFunctions.GetLocalScale)
            return vRet;
        
        TransformFunctions.GetLocalScale(this, vRet);
        return vRet;
    }

    FORCEINLINE void SetPosition(Vector3 vVector)
    {
        if (!TransformFunctions.SetPosition)
            return;
        
        TransformFunctions.SetPosition(this, vVector);
    }
    
    FORCEINLINE void SetForward(Vector3 vVector)
    {
        if (!TransformFunctions.SetForward)
            return;
        
        TransformFunctions.SetForward(this, vVector);
    }
    
    FORCEINLINE void SetRotation(Quaternion qQuat)
    {
        if (!TransformFunctions.SetRotation)
            return;
        
        TransformFunctions.SetRotation(this, qQuat);
    }

    FORCEINLINE void SetLocalPosition(Vector3 vVector)
    {
        if (!TransformFunctions.SetLocalPosition)
            return;
        
        TransformFunctions.SetLocalPosition(this, vVector);
    }

    FORCEINLINE void SetLocalScale(Vector3 vVector)
    {
        if (!TransformFunctions.SetLocalScale)
            return;
        TransformFunctions.SetLocalScale(this, vVector);
    }
};

namespace Transform
{
    void Initialize();
}
