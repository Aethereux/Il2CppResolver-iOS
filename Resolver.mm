//
//  Resolver.mm
//  Asura
//
//  Created by Eux on 1/21/26.
//
#include "Utils/Logger.hpp"
#include "Resolver.h"
#include "Defines.h"
#include <dlfcn.h>
#include <chrono>
#include <thread>
#include <unordered_map>

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>


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
    UnityFramework = IMemoryUtils::GetDyldInfo(BinaryName);
    bool m_InitExportResolved = false;
    
    LOG_INFO("Resolving exports for binary: %s", BinaryName);
    
    for (int i = 0; m_eExportObfuscationType::MAX > i; ++i)
    {
        m_ExportObfuscation = static_cast<m_eExportObfuscationType>(i);
        if (ResolveExport(IL2CPP_INIT_EXPORT))
        {
            m_InitExportResolved = true;
            LOG_INFO("Export obfuscation resolved. Type index: %d", i);
            break;
        }
    }
    
    if (!m_InitExportResolved)
    {
        LOG_ERROR("Failed to resolve %s. Obfuscation check failed.", IL2CPP_INIT_EXPORT);
        return false;
    }
    
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
        LOG_DEBUG("Resolving export: %s", m_ExportPair.first);
        if (!ResolveExport_Boolean(m_ExportPair.second, &m_ExportPair.first[0]))
        {
             LOG_ERROR("Failed to resolve export: %s", m_ExportPair.first);
             return false;
        }
        LOG_DEBUG("Resolved %s -> %p", m_ExportPair.first, *m_ExportPair.second);
    }
    
    LOG_INFO("Exports resolved successfully. Initializing Unity Wrappers...");

    // Unity APIs
    Camera::Initialize();
    Component::Initialize();
    GameObject::Initialize();
    Object::Initialize();
    Transform::Initialize();
    
    return true;
}

bool Il2CppResolver::Init(const char *dir, bool m_DebugMode)
{
    Globals.bDebugMode = m_DebugMode;
    Globals.GameBase = dlopen(dir, RTLD_LAZY);
    LOG_INFO("Initializing Il2CppResolver with DebugMode: %d", m_DebugMode);
    if (Globals.GameBase == nullptr)
    {
        LOG_ERROR("Failed to dlopen module: %s", dir);
        return false;
    }
    
    LOG_INFO("Module %s loaded at: %p", dir, Globals.GameBase);

    if (!InitializeUnityAPI(dir))
    {
        LOG_ERROR("Failed to initialize Unity API");
        return false;
    }

    LOG_INFO("Il2CppResolver Initialization Complete.");
    return true;
}

/* Usage:
ENTRY_POINT void Init() {
    CallAfterSeconds(2) {
        Il2CppResolver::Init();
    });
}
*/


// Helper: Sanitize names for C++ functions
std::string SanitizeName(std::string name)
{
    std::string result = "";
    for (char c : name)
    {
        if (isalnum(c) || c == '_') {
            result += c;
        } else {
            result += '_';
        }
    }
    if (!result.empty() && isdigit(result[0])) result = "_" + result;
    if (result == "int" || result == "float" || result == "bool") result += "_Class";
    return result;
}

std::filesystem::path GetDocumentsPath()
{
    return std::filesystem::path(getenv("HOME")) / "Documents";
}

void DumpToFile(const char* FileName, const std::vector<std::string>& Data)
{
    std::filesystem::path Path = GetDocumentsPath() / FileName;
    std::ofstream File(Path);

    if (File.is_open())
    {
        for (const std::string& Line : Data)
            File << Line;
        File.close();
    }
}

void Il2CppResolver::DumpUnityClasses()
{
    std::vector<std::string> Data;

    // Headers
    Data.push_back("#pragma once\n");
    Data.push_back("#include <string>\n");
    Data.push_back("#include <algorithm>\n");
    Data.push_back("#include <cstdint>\n"); // Needed for int32
    Data.push_back("#include \"Resolver.h\"\n\n");
    Data.push_back("// Auto-generated by Il2CppResolver\n\n");
    
    // StringLiteral Struct
    Data.push_back(
        "template<int32_t Len>\n" // Used int32_t to be safe with standard headers
        "struct StringLiteral\n"
        "{\n"
        "    char Chars[Len];\n"
        "\n"
        "    consteval StringLiteral(const char(&String)[Len])\n"
        "    {\n"
        "        std::copy_n(String, Len, Chars);\n"
        "    }\n"
        "\n"
        "    operator std::string() const\n"
        "    {\n"
        "        return static_cast<const char*>(Chars);\n"
        "    }\n"
        "};\n\n"
    );

    Data.push_back(
        "template<StringLiteral Namespace, StringLiteral Name>\n"
        "inline Il2CppClass* GetCachedClass() {\n"
        "    static Il2CppClass* Cached = nullptr;\n"
        "    if (!Cached)\n"
        "    {\n"
        "        // Fast Lookup (Assembly-CSharp)\n"
        "        Il2CppImage* img = Image::GetByName(\"Assembly-CSharp\");\n"
        "        if(img) Cached = UnityClass::GetFromName(img, Namespace.Chars, Name.Chars);\n"
        "        \n"
        "        // Fallback: Search ALL Assemblies\n"
        "        if (!Cached)\n"
        "            Cached = UnityClass::Find(Namespace.Chars, Name.Chars);\n"
        "    }\n"
        "    return Cached;\n"
        "}\n\n"
    );

    Data.push_back("namespace StaticClasses\n{\n");

    // Iterate and Generate
    size_t AssemblyCount = 0;
    Il2CppAssembly** Assemblies = Domain::GetAssemblies(&AssemblyCount);

    if (Assemblies)
    {
        std::set<std::string> GeneratedFunctions;

        for (size_t i = 0; i < AssemblyCount; i++)
        {
            Il2CppImage* Image = Assemblies[i]->m_pImage;
            if (!Image) continue;

            size_t ClassCount = Image::GetClassCount(Image);

            for (size_t j = 0; j < ClassCount; j++)
            {
                Il2CppClass* Klass = Image::GetClass(Image, j);
                if (!Klass) continue;

                const char* namePtr = Klass->m_pName;
                const char* nsPtr = Klass->m_pNamespace;

                std::string Name = namePtr ? namePtr : "";
                std::string Namespace = nsPtr ? nsPtr : "";

                if (Name.empty() || Name.find("<") != std::string::npos || Name.find("$") != std::string::npos)
                    continue;

                std::string FuncName = Name;
                if (!Namespace.empty()) FuncName = Namespace + "_" + Name;
                
                FuncName = SanitizeName(FuncName);

                if (GeneratedFunctions.count(FuncName)) continue;
                GeneratedFunctions.insert(FuncName);

                // Generate line
                std::string Output = "    inline Il2CppClass* " + FuncName + "() { return GetCachedClass<\"" + Namespace + "\", \"" + Name + "\">(); }\n";
                Data.push_back(Output);
            }
        }
    }

    Data.push_back("}\n");
    DumpToFile("StaticClasses.h", Data);
}
