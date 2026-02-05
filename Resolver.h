//
//  Resolver.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once
#include "Globals.h"

// Utils
#include "Utils/Helper.h"
#include "Utils/Speck.h"
#include "Utils/Memory.h"
#include "Dumping/Helpers.h"
//#include "Dumping/DumpTypeInfo.h" // Uncomment for Usage
//#include "Dumping/DumpUnityClass.h"

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

#include "Runtime/Domain.h"
#include "Runtime/Image.h"
#include "Runtime/Class.h"

#include "StaticClasses.h"
namespace Il2CppResolver
{
    // Call This
    bool Init(const char *dir = "UnityFramework", bool m_DebugMode = true);
}

