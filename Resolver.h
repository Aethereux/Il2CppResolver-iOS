//
//  Resolver.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once
#pragma once
#include "Globals.h"
#include "Defines.h"

// Utils
#include "Utils/Logger.hpp"

// Math
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Math/Math.hpp"

// Data
#include "Data/Il2Cpp.h"
#include "Data/Il2CppString.h"
#include "Data/Il2CppArray.h"
#include "Data/Il2CppDictionary.h"

// Unity Wrappers
#include "Unity/Object.h"
#include "Unity/GameObject.h"
#include "Unity/Component.h"
#include "Unity/Transform.h"
#include "Unity/Camera.h"

namespace Il2CppResolver
{
    static inline const char* IL2CPP_FRAMEWORK(const char* NAME) {
        NSString *appPath = [[NSBundle mainBundle] bundlePath];
        NSString *binaryPath = [NSString stringWithFormat:@"%s", NAME];
        if ([binaryPath isEqualToString:@"UnityFramework"])
        {
            binaryPath = [appPath stringByAppendingPathComponent:@"Frameworks/UnityFramework.framework/UnityFramework"];
        }
        else
        {
            binaryPath = [appPath stringByAppendingPathComponent:binaryPath];
        }
        return [binaryPath UTF8String];
    }
    
    // Call This
    bool Init(const char *dir = "UnityFramework", bool m_DebugMode = true);
    void DumpUnityClasses();
}
