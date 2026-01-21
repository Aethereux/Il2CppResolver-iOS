#include "Object.h"
#include "../Utils/Logger.hpp"

namespace Object
{
    void Initialize()
    {
        LOG_INFO("Initializing Object...");
        ResolveFunction(ObjectFunctions.Destroy, ResolveCall(UNITY_OBJECT_DESTROY));
        ObjectFunctions.FindObjectsOfType = ResolveCall(UNITY_OBJECT_FINDOBJECTSOFTYPE);
        ResolveFunction(ObjectFunctions.GetName, ResolveCall(UNITY_OBJECT_GETNAME));
    }

    static Il2CppObject* New(Il2CppClass* m_pClass)
    {
        if (!Functions.ObjectNew)
            return nullptr;
        
        return Functions.ObjectNew(m_pClass);
    }
}
