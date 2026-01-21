//
//  Component.mm
//  Asura
//
//  Created by Eux on 1/21/26.
//

#include "Component.h"

namespace Component
{
    void Initialize()
    {
        ResolveFunction(ComponentFunctions.GetGameObject, ResolveCall(UNITY_COMPONENT_GETGAMEOBJECT));
        ResolveFunction(ComponentFunctions.GetTransform, ResolveCall(UNITY_COMPONENT_GETTRANSFORM));
    }
}
