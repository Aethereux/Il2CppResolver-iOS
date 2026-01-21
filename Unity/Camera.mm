//
//  Camera.mm
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//

#include "Camera.h"
#include "../Runtime/ResolveCall.h"

void Camera::Initialize()
{
    ResolveFunction(CameraFunctions.GetCurrent, ResolveCall(UNITY_CAMERA_GETCURRENT));
    ResolveFunction(CameraFunctions.GetMain, ResolveCall(UNITY_CAMERA_GETMAIN));
    ResolveFunction(CameraFunctions.GetFieldOfView, ResolveCall(UNITY_CAMERA_GETFIELDOFVIEW));
    ResolveFunction(CameraFunctions.SetFieldOfView, ResolveCall(UNITY_CAMERA_SETFIELDOFVIEW));
    ResolveFunction(CameraFunctions.WorldToViewportPoint, ResolveCall(UNITY_CAMERA_WORLDTOVIEWPORT));
}

