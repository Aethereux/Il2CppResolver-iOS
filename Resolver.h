//
//  Resolver.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once
#include "Globals.h"

namespace Il2CppResolver
{
    // Call This
    bool Init(bool m_WaitForModule = true, int m_MaxSecondsWait = 60, const char *dir = "UnityFramework");
}
