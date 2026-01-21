//
//  Camera.mm
//  Asura
//
//  Created by Eux on 1/21/26.
//

#include "Camera.h"
#include "../Runtime/ResolveCall.h"
#include "../Utils/Logger.hpp"

void Camera::Initialize()
{
    LOG_INFO("Initializing Camera...");
    ResolveFunction(CameraFunctions.GetCurrent, ResolveCall(UNITY_CAMERA_GETCURRENT));
    ResolveFunction(CameraFunctions.GetMain, ResolveCall(UNITY_CAMERA_GETMAIN));
    ResolveFunction(CameraFunctions.GetFieldOfView, ResolveCall(UNITY_CAMERA_GETFIELDOFVIEW));
    ResolveFunction(CameraFunctions.SetFieldOfView, ResolveCall(UNITY_CAMERA_SETFIELDOFVIEW));
    ResolveFunction(CameraFunctions.WorldToViewportPoint, ResolveCall(UNITY_CAMERA_WORLDTOVIEWPORT));
}

