//
//  Resolver.mm
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//
#include "Resolver.h"
#include "Defines.h"
#include <dlfcn.h>
#include <chrono>
#include <thread>
#include <unordered_map>

#include "Unity/Camera.h"
#include "Unity/Component.h"
#include "Unity/GameObject.h"
#include "Unity/Object.h"
#include "Unity/Transform.h"

enum m_eExportObfuscationType
{
    None = 0,
    ROT = 1,
    MAX = 2,
};

m_eExportObfuscationType m_ExportObfuscation = m_eExportObfuscationType::None;

int m_ROTObfuscationValue = -1;

void* ResolveExport(const char* m_Name)
{
    switch (m_ExportObfuscation)
    {
        case m_eExportObfuscationType::ROT:
        {
            if (m_ROTObfuscationValue == -1) // Bruteforce
            {
                for (int i = 1; 26 > i; ++i)
                {
                    void* m_Return = dlsym(Globals.GameBase, m_Name);
                    if (m_Return)
                    {
                        m_ROTObfuscationValue = i;
                        return m_Return;
                    }
                }
                
                return nullptr;
            }
            
            return dlsym(Globals.GameBase, m_Name);
        }
        default:
            return dlsym(Globals.GameBase, m_Name);
    }
    
    return nullptr;
}

bool ResolveExport_Boolean(void** m_Address, const char* m_Name)
{
    *m_Address = ResolveExport(m_Name);
    assert(*m_Address != nullptr && "Couldn't resolve export!");
    return (*m_Address);
}

bool InitializeUnityAPI(const char* BinaryName)
{
    Globals.UnityFramework = IMemoryUtils::GetDyldInfo(BinaryName);
    bool m_InitExportResolved = false;
    for (int i = 0; m_eExportObfuscationType::MAX > i; ++i)
    {
        m_ExportObfuscation = static_cast<m_eExportObfuscationType>(i);
        if (ResolveExport(IL2CPP_INIT_EXPORT))
        {
            m_InitExportResolved = true;
            break;
        }
    }
    
    if (!m_InitExportResolved)
        return false;
    
    std::unordered_map<const char*, void**> m_ExportMap =
    {
        { IL2CPP_CLASS_FROM_NAME_EXPORT,                    (void**)&Functions.ClassFromName },
        { IL2CPP_CLASS_GET_FIELDS,                            (void**)&Functions.ClassGetFields },
        { IL2CPP_CLASS_GET_FIELD_FROM_NAME_EXPORT,            (void**)&Functions.ClassGetFieldFromName },
        { IL2CPP_CLASS_GET_METHODS,                            (void**)&Functions.ClassGetMethods },
        { IL2CPP_CLASS_GET_METHOD_FROM_NAME_EXPORT,            (void**)&Functions.ClassGetMethodFromName },
        { IL2CPP_CLASS_GET_PROPERTY_FROM_NAME_EXPORT,        (void**)&Functions.ClassGetPropertyFromName },
        { IL2CPP_CLASS_GET_TYPE_EXPORT,                        (void**)&Functions.ClassGetType },
        { IL2CPP_DOMAIN_GET_EXPORT,                            (void**)&Functions.DomainGet },
        { IL2CPP_DOMAIN_GET_ASSEMBLIES_EXPORT,                (void**)&Functions.DomainGetAssemblies },
        { IL2CPP_IMAGE_GET_CLASS_EXPORT,                    (void**)&Functions.ImageGetClass },
        { IL2CPP_IMAGE_GET_CLASS_COUNT_EXPORT,                (void**)&Functions.ImageGetClassCount },
        { IL2CPP_RESOLVE_FUNC_EXPORT,                        (void**)&Functions.ResolveFunction },
        { IL2CPP_STRING_NEW_EXPORT,                            (void**)&Functions.StringNew },
        { IL2CPP_THREAD_ATTACH_EXPORT,                        (void**)&Functions.ThreadAttach },
        { IL2CPP_THREAD_DETACH_EXPORT,                        (void**)&Functions.ThreadDetach },
        { IL2CPP_TYPE_GET_OBJECT_EXPORT,                    (void**)&Functions.TypeGetObject },
        { IL2CPP_OBJECT_NEW,                                (void**)&Functions.ObjectNew },
        { IL2CPP_METHOD_GET_PARAM_NAME,                        (void**)&Functions.MethodGetParamName },
        { IL2CPP_METHOD_GET_PARAM,                            (void**)&Functions.MethodGetParam },
        { IL2CPP_CLASS_FROM_IL2CPP_TYPE,                    (void**)&Functions.ClassFromIl2cppType },
        { IL2CPP_FIELD_STATIC_GET_VALUE,                    (void**)&Functions.FieldStaticGetValue },
        { IL2CPP_FIELD_STATIC_SET_VALUE,                    (void**)&Functions.FieldStaticSetValue },
        { IL2CPP_ASSEMBLY_GET_IMAGE,                        (void**)&Functions.AssembliesGetImage },
        { IL2CPP_IMAGE_GET_NAME,                            (void**)&Functions.ImageGetName },
    };
    
    
    for (auto& m_ExportPair : m_ExportMap)
    {
        if (!ResolveExport_Boolean(m_ExportPair.second, &m_ExportPair.first[0]))
            return false;
    }
    
    // Unity APIs
    Camera::Initialize();
    Component::Initialize();
    GameObject::Initialize();
    Object::Initialize();
    Transform::Initialize();
    
    return true;
}

bool Il2CppResolver::Init(bool m_WaitForModule, int m_MaxSecondsWait, const char *dir)
{
   
    if (m_WaitForModule)
    {
        int m_SecondsWaited = 0;
        while (!Globals.GameBase)
        {
            if (m_SecondsWaited >= m_MaxSecondsWait)
                return false;

            Globals.GameBase = dlopen(dir, RTLD_LAZY);
            ++m_SecondsWaited;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    else
        Globals.GameBase = dlopen(dir, RTLD_LAZY);
    
    if (Globals.GameBase == nullptr)
        return false;
    
    if (!InitializeUnityAPI(dir))
        return false;

    return true;
}
