//
//  Transform.mm
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//

#include "Transform.h"

namespace Transform
{
    void Initialize()
    {
        ResolveFunction(TransformFunctions.GetParent, ResolveCall(UNITY_TRANSFORM_GETPARENT));
        ResolveFunction(TransformFunctions.GetRoot, ResolveCall(UNITY_TRANSFORM_GETROOT));
        ResolveFunction(TransformFunctions.GetChild, ResolveCall(UNITY_TRANSFORM_GETCHILD));
        ResolveFunction(TransformFunctions.GetChildCount, ResolveCall(UNITY_TRANSFORM_GETCHILDCOUNT));
        ResolveFunction(TransformFunctions.FindChild, ResolveCall(UNITY_TRANSFORM_FINDCHILD));
        ResolveFunction(TransformFunctions.GetPosition, ResolveCall(UNITY_TRANSFORM_GETPOSITION));
        ResolveFunction(TransformFunctions.GetForward, ResolveCall(UNITY_TRANSFORM_GETFORWARD));
        ResolveFunction(TransformFunctions.GetRotation, ResolveCall(UNITY_TRANSFORM_GETROTATION));
        ResolveFunction(TransformFunctions.GetLocalPosition, ResolveCall(UNITY_TRANSFORM_GETLOCALPOSITION));
        ResolveFunction(TransformFunctions.GetLocalScale, ResolveCall(UNITY_TRANSFORM_GETLOCALSCALE));
        ResolveFunction(TransformFunctions.SetPosition, ResolveCall(UNITY_TRANSFORM_SETPOSITION));
        ResolveFunction(TransformFunctions.SetForward, ResolveCall(UNITY_TRANSFORM_SETFORWARD));
        ResolveFunction(TransformFunctions.SetRotation, ResolveCall(UNITY_TRANSFORM_SETROTATION));
        ResolveFunction(TransformFunctions.SetLocalPosition, ResolveCall(UNITY_TRANSFORM_SETLOCALPOSITION));
        ResolveFunction(TransformFunctions.SetLocalScale, ResolveCall(UNITY_TRANSFORM_SETLOCALSCALE));
    }
}
