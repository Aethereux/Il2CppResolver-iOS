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
}
