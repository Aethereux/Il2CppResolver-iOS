//
//  Camera.h
//  Asura
//
//  Created by Eux on 1/21/26.
//
#pragma once
#include "../Defines.h"
#include "GameObject.h"
#include "../Math/Vector3.h"

struct CameraFunctions_t
{
    void* (*GetCurrent)() = nullptr;
    void* (*GetMain)() = nullptr;
    float (*GetFieldOfView)(void*) = nullptr;
    void (*SetFieldOfView)(void*, float) = nullptr;
    void (*WorldToViewportPoint)(void*, Vector3&, int, Vector3&) = nullptr;
};

inline CameraFunctions_t CameraFunctions;

class CCamera : public CGameObject
{
public:
    FORCEINLINE static CCamera* GetCurrent()
    {
        if (!CameraFunctions.GetCurrent)
            return nullptr;
        
        return (CCamera*)CameraFunctions.GetCurrent();
    }

    FORCEINLINE static CCamera* GetMain()
    {
        if (!CameraFunctions.GetMain)
            return nullptr;
        
        return (CCamera*)CameraFunctions.GetMain();
    }
    
    FORCEINLINE float GetFieldOfView()
    {
        if (!CameraFunctions.GetFieldOfView)
            return -1;
        
        return CameraFunctions.GetFieldOfView(this);
    }

    FORCEINLINE void SetFieldOfView(float m_fValue)
    {
        if (!CameraFunctions.SetFieldOfView)
            return;
        
        CameraFunctions.SetFieldOfView(this, m_fValue);
    }
    
    FORCEINLINE void WorldToViewportPoint(Vector3& m_vWorld, Vector3& m_vScreen, int m_iEye = 2)
    {
        if (!CameraFunctions.WorldToViewportPoint)
            return;
        
        CameraFunctions.WorldToViewportPoint(this, m_vWorld, m_iEye, m_vScreen);
    }
};

namespace Camera
{
    void Initialize();
}

enum m_eCameraType : int
{
    m_eCameraType_Game = 1,
    m_eCameraType_SceneView = 2,
    m_eCameraType_Preview = 4,
    m_eCameraType_VR = 8,
    m_eCameraType_Reflection = 16,
};

enum m_eCameraEye : int
{
    m_eCameraEye_Left = 0,
    m_eCameraEye_Right = 1,
    m_eCameraEye_Center = 2,
};
