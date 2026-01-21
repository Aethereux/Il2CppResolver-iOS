#include "Component.h"
#include "../Utils/Logger.hpp"

namespace Component
{
    void Initialize()
    {
        LOG_INFO("Initializing Component...");
        ResolveFunction(ComponentFunctions.GetGameObject, ResolveCall(UNITY_COMPONENT_GETGAMEOBJECT));
        ResolveFunction(ComponentFunctions.GetTransform, ResolveCall(UNITY_COMPONENT_GETTRANSFORM));
    }
}
