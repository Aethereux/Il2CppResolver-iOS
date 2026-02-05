//
//  Resolver.mm
//  Asura
//
//  Created by Eux on 1/21/26.
//

#include "Utils/Logger.hpp"
#include "Resolver.h"
#include <dlfcn.h>
#include <array>
#import <Foundation/Foundation.h>

namespace 
{
    // Obfuscation handling
    enum class ExportObfuscationType
    {
        None = 0,
        ROT,
        MAX
    };

    ExportObfuscationType g_ExportObfuscation = ExportObfuscationType::None;
    int g_ROTValue = -1;

    void* ResolveExport(const char* Name)
    {
        if (g_ExportObfuscation == ExportObfuscationType::ROT)
        {
            if (g_ROTValue == -1) // Bruteforce detect
            {
                for (int i = 1; i < 26; ++i)
                {
                    void* Symbol = dlsym(Globals.GameBase, Name);
                    if (Symbol)
                    {
                        g_ROTValue = i;
                        return Symbol;
                    }
                }
                return nullptr;
            }
            // If ROT is known (or default path)
             return dlsym(Globals.GameBase, Name);
        }
        
        // Default / None
        return dlsym(Globals.GameBase, Name);
    }
}

bool Il2CppResolver::Init(const char *dir, bool DebugMode)
{
    Globals.bDebugMode = DebugMode;

    UnityFramework = IMemoryUtils::GetDyldInfo(dir);
    if (UnityFramework.Header)
    {
        LOG_INFO("Found module via Dyld iteration: %{public}s", UnityFramework.Path);
        Globals.GameBase = dlopen(UnityFramework.Path, RTLD_LAZY);
    }
    else
    {
        LOG_FAULT("Module %s not found via Dyld iteration. Attempting direct dlopen...", dir);
        Globals.GameBase = dlopen(dir, RTLD_LAZY);
    }
    
    LOG_INFO("Initializing Il2CppResolver with DebugMode: %{public}d", DebugMode);
    
    if (!Globals.GameBase)
    {
        LOG_ERROR("Failed to dlopen module: %{public}s", dir);
        return false;
    }
    
    LOG_INFO("Module loaded at: %{public}p", Globals.GameBase);
    
    bool bInitResolved = false;
    LOG_INFO("Resolving exports for binary: %{public}s", dir);
    
    for (int i = 0; i < (int)ExportObfuscationType::MAX; ++i)
    {
        g_ExportObfuscation = static_cast<ExportObfuscationType>(i);
        if (ResolveExport(IL2CPP_INIT_EXPORT))
        {
            bInitResolved = true;
            LOG_INFO("Export obfuscation resolved. Type index: %{public}d", i);
            break;
        }
    }
    
    if (!bInitResolved)
    {
        LOG_ERROR("Failed to resolve %s. Obfuscation check failed.", IL2CPP_INIT_EXPORT);
        return false;
    }
    
    struct ExportEntry { const char* Name; void** FuncPtr; };
    const std::array<ExportEntry, 25> Exports = {{
        { IL2CPP_CLASS_FROM_NAME_EXPORT,             (void**)&Functions.ClassFromName },
        { IL2CPP_CLASS_IS_SUBCLASS_OF_EXPORT,        (void**)&Functions.ClassIsSubclassOf },
        { IL2CPP_CLASS_GET_FIELDS,                   (void**)&Functions.ClassGetFields },
        { IL2CPP_CLASS_GET_FIELD_FROM_NAME_EXPORT,   (void**)&Functions.ClassGetFieldFromName },
        { IL2CPP_CLASS_GET_METHODS,                  (void**)&Functions.ClassGetMethods },
        { IL2CPP_CLASS_GET_METHOD_FROM_NAME_EXPORT,  (void**)&Functions.ClassGetMethodFromName },
        { IL2CPP_CLASS_GET_PROPERTY_FROM_NAME_EXPORT,(void**)&Functions.ClassGetPropertyFromName },
        { IL2CPP_CLASS_GET_TYPE_EXPORT,              (void**)&Functions.ClassGetType },
        { IL2CPP_DOMAIN_GET_EXPORT,                  (void**)&Functions.DomainGet },
        { IL2CPP_DOMAIN_GET_ASSEMBLIES_EXPORT,       (void**)&Functions.DomainGetAssemblies },
        { IL2CPP_IMAGE_GET_CLASS_EXPORT,             (void**)&Functions.ImageGetClass },
        { IL2CPP_IMAGE_GET_CLASS_COUNT_EXPORT,       (void**)&Functions.ImageGetClassCount },
        { IL2CPP_RESOLVE_FUNC_EXPORT,                (void**)&Functions.ResolveFunction },
        { IL2CPP_STRING_NEW_EXPORT,                  (void**)&Functions.StringNew },
        { IL2CPP_THREAD_ATTACH_EXPORT,               (void**)&Functions.ThreadAttach },
        { IL2CPP_THREAD_DETACH_EXPORT,               (void**)&Functions.ThreadDetach },
        { IL2CPP_TYPE_GET_OBJECT_EXPORT,             (void**)&Functions.TypeGetObject },
        { IL2CPP_OBJECT_NEW,                         (void**)&Functions.ObjectNew },
        { IL2CPP_METHOD_GET_PARAM_NAME,              (void**)&Functions.MethodGetParamName },
        { IL2CPP_METHOD_GET_PARAM,                   (void**)&Functions.MethodGetParam },
        { IL2CPP_CLASS_FROM_IL2CPP_TYPE,             (void**)&Functions.ClassFromIl2cppType },
        { IL2CPP_FIELD_STATIC_GET_VALUE,             (void**)&Functions.FieldStaticGetValue },
        { IL2CPP_FIELD_STATIC_SET_VALUE,             (void**)&Functions.FieldStaticSetValue },
        { IL2CPP_ASSEMBLY_GET_IMAGE,                 (void**)&Functions.AssembliesGetImage },
        { IL2CPP_IMAGE_GET_NAME,                     (void**)&Functions.ImageGetName }
    }};
    
    for (const auto& Entry : Exports)
    {
        LOG_DEBUG("Resolving export: %{public}s", Entry.Name);
        void* Addr = ResolveExport(Entry.Name);
        if (!Addr) {
            LOG_ERROR("Failed to resolve export: %{public}s", Entry.Name);
            return false;
        }
        *Entry.FuncPtr = Addr;
        LOG_DEBUG("Resolved %{public}s -> %{public}p", Entry.Name, Addr);
    }
    LOG_INFO("Exports resolved successfully. Initializing Unity Wrappers...");
    
    // Unity APIs
    Camera::Initialize();
    Component::Initialize();
    GameObject::Initialize();
    Object::Initialize();
    Transform::Initialize();

    LOG_INFO("Il2CppResolver Initialization Complete.");
    return true;
}
