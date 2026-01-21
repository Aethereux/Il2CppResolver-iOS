#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "../Utils/Logger.hpp"

void GameObject::Initialize()
{
    LOG_INFO("Initializing GameObject...");
    ResolveFunction(GameObjectFunctions.AddComponent, ResolveCall(UNITY_GAMEOBJECT_ADDCOMPONENT));
    ResolveFunction(GameObjectFunctions.CreatePrimitive, ResolveCall(UNITY_GAMEOBJECT_CREATEPRIMITIVE));
    ResolveFunction(GameObjectFunctions.Find, ResolveCall(UNITY_GAMEOBJECT_FIND));
    ResolveFunction(GameObjectFunctions.FindGameObjectsWithTag, ResolveCall(UNITY_GAMEOBJECT_FINDGAMEOBJECTWITHTAG));
    ResolveFunction(GameObjectFunctions.GetComponent, ResolveCall(UNITY_GAMEOBJECT_GETCOMPONENT));
    ResolveFunction(GameObjectFunctions.GetComponents, ResolveCall(UNITY_GAMEOBJECT_GETCOMPONENTS));
    ResolveFunction(GameObjectFunctions.GetComponentInChildren, ResolveCall(UNITY_GAMEOBJECT_GETCOMPONENTINCHILDREN));
    ResolveFunction(GameObjectFunctions.GetActive, ResolveCall(UNITY_GAMEOBJECT_GETACTIVE));
    ResolveFunction(GameObjectFunctions.GetLayer, ResolveCall(UNITY_GAMEOBJECT_GETLAYER));
    ResolveFunction(GameObjectFunctions.GetTransform, ResolveCall(UNITY_GAMEOBJECT_GETTRANSFORM));
    ResolveFunction(GameObjectFunctions.SetActive, ResolveCall(UNITY_GAMEOBJECT_SETACTIVE));
    ResolveFunction(GameObjectFunctions.SetLayer, ResolveCall(UNITY_GAMEOBJECT_SETLAYER));
}
