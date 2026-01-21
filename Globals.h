//
//  Globals.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#ifndef ASURA_GLOBALS_H
#define ASURA_GLOBALS_H

#include "Utils/Memory.h"

// Forward Declarations
struct Il2CppAssembly;
struct Il2CppClass;
struct Il2CppObject;
struct Il2CppType;
struct Il2CppFieldInfo;
struct Il2CppMethodInfo;
struct Il2CppPropertyInfo;
struct Il2CppString;

static IMemoryUtils::DyldInfo UnityFramework;
struct Globals_t
{
    void* GameBase;
    bool bDebugMode = false;
};

struct Functions_t
{
    // IL2CPP Exported Function Pointers
    Il2CppClass* (*ClassFromName)(void*, const char*, const char*) = nullptr;
    
    Il2CppFieldInfo* (*ClassGetFields)(void*, void**) = nullptr;
    Il2CppFieldInfo* (*ClassGetFieldFromName)(void*, const char*) = nullptr;
    
    Il2CppMethodInfo* (*ClassGetMethods)(void*, void**) = nullptr;
    Il2CppMethodInfo* (*ClassGetMethodFromName)(void*, const char*, int) = nullptr;

    Il2CppPropertyInfo* (*ClassGetPropertyFromName)(void*, const char*) = nullptr;
    
    Il2CppType* (*ClassGetType)(void*) = nullptr;

    Il2CppAssembly** (*DomainGetAssemblies)(void*, size_t*) = nullptr;
    void* (*DomainGet)() = nullptr;

    void* (*AssembliesGetImage)(Il2CppAssembly*) = nullptr;
    const char* (*ImageGetName)(void*) = nullptr;

//    void* Free = nullptr; // unused

    Il2CppClass* (*ImageGetClass)(void*, size_t) = nullptr;
    size_t* (*ImageGetClassCount)(void*) = nullptr;

    void* (*ResolveFunction)(const char*) = nullptr;

    Il2CppString* (*StringNew)(const char*) = nullptr;

    void* (*ThreadAttach)(void*) = nullptr;
    void (*ThreadDetach)(void*) = nullptr;

    Il2CppObject* (*TypeGetObject)(void*) = nullptr;
    Il2CppObject* (*ObjectNew)(void*) = nullptr;
    const char* (*MethodGetParamName)(void*, uint32_t) = nullptr;
    Il2CppType* (*MethodGetParam)(void*, uint32_t) = nullptr;
    Il2CppClass* (*ClassFromIl2cppType)(void*) = nullptr;

    void (*FieldStaticGetValue)(Il2CppFieldInfo*, void*) = nullptr;
    void (*FieldStaticSetValue)(Il2CppFieldInfo*, void*) = nullptr;
};

inline Globals_t Globals;
inline Functions_t Functions;

#endif // ASURA_GLOBALS_H
