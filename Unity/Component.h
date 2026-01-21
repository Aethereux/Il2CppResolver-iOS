#pragma once

#include "../Runtime/ResolveCall.h"
#include "Transform.h"
#include "GameObject.h"
#include "Object.h"

class CGameObject;
struct ComponentFunctions_t
{
    void* (*GetGameObject)(void*) = nullptr;
    void* (*GetTransform)(void*) = nullptr;
};

inline ComponentFunctions_t ComponentFunctions;

class CComponent : public CObject
{
public:
    FORCEINLINE CGameObject* GetGameObject()
    {
        if (!ComponentFunctions.GetGameObject)
            return nullptr;
        
        return (CGameObject*)ComponentFunctions.GetGameObject(this);
    }

    FORCEINLINE CTransform* GetTransform()
    {
        if (!ComponentFunctions.GetTransform)
            return nullptr;
        
        return (CTransform*)ComponentFunctions.GetTransform(this);
    }
};

namespace Component
{
    void Initialize();
}
