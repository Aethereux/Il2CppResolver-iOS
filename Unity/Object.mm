//
//  Object.mm
//  Asura
//
//  Created by Eux on 1/21/26.
//

#include "Object.h"

namespace Object
{
    void Initialize()
    {
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
