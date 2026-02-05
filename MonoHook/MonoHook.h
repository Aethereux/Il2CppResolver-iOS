//
//  MonoHook.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/23/26.
//

#include "../Data/Il2Cpp.h"
#include "../Utils/Memory.h"
#include <map>

namespace MethodHook
{
    struct HookTracker {
        void* originalPtr;
        Il2CppMethodInfo* methodInfo;
    };
    
    // Keep track of hooks so we can uninstall them
    std::map<void*, HookTracker> activeHooks;

    // Install: Swaps the function pointer in the Metadata
    void Install(Il2CppMethodInfo* method, void* newFunction, void** originalFunction)
    {
        if (!method) return;

        // 1. Save Original Pointer
        void* oldPtr = method->m_pMethodPointer;
        if (originalFunction) {
            *originalFunction = oldPtr;
        }

        // 2. Track it
        activeHooks[newFunction] = { oldPtr, method };

        // 3. Swap the Pointer (Safe on iOS)
        // We overwrite the pointer inside the struct.
        IMemoryUtils::Get()->Write((uintptr_t)&method->m_pMethodPointer, (uintptr_t)newFunction);
    }

    void Uninstall(void* newFunction)
        {
            if (activeHooks.find(newFunction) != activeHooks.end())
            {
                HookTracker& tracker = activeHooks[newFunction];
                IMemoryUtils::Get()->Write((uintptr_t)&tracker.methodInfo->m_pMethodPointer, (uintptr_t)tracker.originalPtr);
                activeHooks.erase(newFunction);
            }
        }
}
