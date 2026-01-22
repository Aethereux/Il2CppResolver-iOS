//
//  Class.mm
//  Asura
//
//  Created by Eux on 1/21/26.
//

#include "Class.h"

void UnityClass::FetchFields(Il2CppClass* m_pClass, std::vector<Il2CppFieldInfo*>* m_pVector, void* m_pFieldIterator)
{
    m_pVector->clear();

    while (1)
    {
        Il2CppFieldInfo* m_pField = GetFields(m_pClass, &m_pFieldIterator);
        if (!m_pField)
            break;

        m_pVector->emplace_back(m_pField);
    }
}

void UnityClass::FetchMethods(Il2CppClass* m_pClass, std::vector<Il2CppMethodInfo*>* m_pVector, void* m_pMethodIterator)
{
    m_pVector->clear();

    while (1)
    {
        Il2CppMethodInfo* m_pMethod = GetMethods(m_pClass, &m_pMethodIterator);
        if (!m_pMethod)
            break;

        m_pVector->emplace_back(m_pMethod);
    }
}

Il2CppClass* UnityClass::Find(const char* m_pName)
{
    size_t m_sAssembliesCount = 0U;
    Il2CppAssembly** m_pAssemblies = Domain::GetAssemblies(&m_sAssembliesCount);
    if (!m_pAssemblies || 0U >= m_sAssembliesCount) return nullptr;

    const char* m_pNameSpaceEnd = strrchr(m_pName, '.');
    char m_pNameSpaceBuffer[1024];

    if (m_pNameSpaceEnd)
    {
        size_t m_uNamespaceSize = (size_t)(m_pNameSpaceEnd - m_pName);
        if (m_uNamespaceSize >= sizeof(m_pNameSpaceBuffer))
            m_uNamespaceSize = sizeof(m_pNameSpaceBuffer) - 1;

        memcpy(m_pNameSpaceBuffer, m_pName, m_uNamespaceSize);
        m_pNameSpaceBuffer[m_uNamespaceSize] = '\0';

        m_pName = m_pNameSpaceEnd + 1;
    }
    else
    {
        m_pNameSpaceBuffer[0] = '\0';
    }


    Il2CppClass* m_pClassReturn = nullptr;
    for (size_t i = 0U; m_sAssembliesCount > i; ++i)
    {
        Il2CppAssembly* m_pAssembly = m_pAssemblies[i];
        if (!m_pAssembly || !m_pAssembly->m_pImage) continue;

        m_pClassReturn = GetFromName(m_pAssembly->m_pImage, m_pNameSpaceBuffer, m_pName);
        if (m_pClassReturn) break;
    }

    return m_pClassReturn;
}

Il2CppClass* UnityClass::Find(const char* m_pNamespace, const char* m_pName)
{
    size_t m_sAssembliesCount = 0;
    Il2CppAssembly** m_pAssemblies = Domain::GetAssemblies(&m_sAssembliesCount);
    if (!m_pAssemblies || m_sAssembliesCount == 0) return nullptr;

    for (size_t i = 0; i < m_sAssembliesCount; ++i)
    {
        Il2CppAssembly* m_pAssembly = m_pAssemblies[i];
        if (!m_pAssembly || !m_pAssembly->m_pImage) continue;

        // Try to get the class from this assembly
        Il2CppClass* m_pClass = GetFromName(m_pAssembly->m_pImage, m_pNamespace, m_pName);
        if (m_pClass)
            return m_pClass;
    }

    return nullptr;
}

void* UnityClass::GetMethodPointer(const char* m_pClassName, const char* m_pMethodName, std::initializer_list<const char*> m_vNames)
{
    Il2CppClass* m_pClass = Find(m_pClassName);
    if (!m_pClass)
        return nullptr;

    int m_iNamesCount = static_cast<int>(m_vNames.size());
    const char** m_pNames = const_cast<const char**>(m_vNames.begin());

    void* m_pMethodIterator = nullptr;
    while (1)
    {
        Il2CppMethodInfo* m_pMethod = GetMethods(m_pClass, &m_pMethodIterator);
        if (!m_pMethod)
            break;

        if (UTF8Utils::Strcmp(m_pMethod->m_pName, m_pMethodName) != 0)
            continue;

        #ifdef UNITY_VERSION_2022_3_8F1
        Il2CppType** m_pCurrentParameterTypes = m_pMethod->m_pParameters;

        for (size_t i = 0; i < m_pMethod->m_uArgsCount; ++i)
        {
            Il2CppType* m_pCurrentParameterType = m_pCurrentParameterTypes[i];
            Il2CppClass* m_pClass = ClassFromType(m_pCurrentParameterType);

            if (UTF8Utils::Strcmp(m_pClass->m_pName, m_pNames[i]) != 0)
                break;

            if ((i + 1) == m_iNamesCount)
                return m_pMethod->m_pMethodPointer;
        }
        #else
        Il2CppParameterInfo* m_pCurrentParameters = m_pMethod->m_pParameters;
        for (int i = 0; m_iNamesCount > i; ++i)
        {
            if (UTF8Utils::Strcmp(m_pCurrentParameters->m_pName, m_pNames[i]) != 0)
                break;

            m_pCurrentParameters++; // m_pCurrentParameters += sizeof(Unity::il2cppParameterInfo);
            if ((i + 1) == m_iNamesCount)
                return m_pMethod->m_pMethodPointer;
        }
        #endif
    }
    return nullptr;
}

Il2CppClass* UnityClass::FilterClass(std::vector<Il2CppClass*>* m_pClasses, std::initializer_list<const char*> m_vNames, int m_iFoundCount)
{
    int m_iNamesCount = static_cast<int>(m_vNames.size());
    const char** m_pNames = const_cast<const char**>(m_vNames.begin());

    if (0 >= m_iFoundCount || m_iFoundCount > m_iNamesCount)
        m_iFoundCount = m_iNamesCount;

    Il2CppClass* m_pReturn = nullptr;
    for (size_t c = 0; m_pClasses->size() > c; ++c)
    {
        int m_iFoundCountNow = 0;

        Il2CppClass* m_pClass = m_pClasses->operator[](c);
        if (!m_pClass)
            continue;

        for (int i = 0; m_iNamesCount > i; ++i)
        {
            const char* m_pNameToFind = m_pNames[i];

            bool m_bFoundInClass = false;
            if (m_pNameToFind[0] == '~') // Field
                m_bFoundInClass = Utils::GetFieldOffset(m_pClass, &m_pNameToFind[1]) >= 0;
            else if (m_pNameToFind[0] == '-') // Method
                m_bFoundInClass = Utils::GetMethodPointer(m_pClass, &m_pNameToFind[1]) != nullptr;
            else // Both
            {
                m_bFoundInClass = Utils::GetFieldOffset(m_pClass, m_pNameToFind) >= 0;
                if (!m_bFoundInClass)
                    m_bFoundInClass = Utils::GetMethodPointer(m_pClass, m_pNameToFind) != nullptr;
            }

            if (m_bFoundInClass)
                ++m_iFoundCountNow;
        }

        if (m_iFoundCount == m_iFoundCountNow)
        {
            m_pReturn = m_pClass;
            break;
        }
    }

    return m_pReturn;
}

void* UnityClass::FilterClassToMethodPointer(std::vector<Il2CppClass*>* m_pClasses, const char* m_pMethodName, int m_iArgs)
{
    void* m_pMethodPointer = nullptr;
    for (size_t c = 0; m_pClasses->size() > c; ++c)
    {
        Il2CppClass* m_pClass = m_pClasses->operator[](c);
        if (!m_pClass)
            continue;

        m_pMethodPointer = Utils::GetMethodPointer(m_pClass, m_pMethodName, m_iArgs);
        if (m_pMethodPointer)
            break;
    }
    return m_pMethodPointer;
}

void UnityClass::FetchClasses(std::vector<Il2CppClass*>* m_pVector, const char* m_pModuleName, const char* m_pNamespace)
{
    m_pVector->clear();

    if (!Functions.ImageGetClassCount || !Functions.ImageGetClass)
        return;
    
    size_t m_sAssembliesCount = 0U;
    Il2CppAssembly** m_pAssemblies = Domain::GetAssemblies(&m_sAssembliesCount);
    if (!m_pAssemblies || 0U >= m_sAssembliesCount) return;

    Il2CppImage* m_pImage = nullptr;
    for (size_t i = 0U; m_sAssembliesCount > i; ++i)
    {
        Il2CppAssembly* m_pAssembly = m_pAssemblies[i];
        if (!m_pAssembly || !m_pAssembly->m_pImage || UTF8Utils::Strcmp(m_pAssembly->m_pImage->m_pNameNoExt, m_pModuleName) != 0)
            continue;

        m_pImage = m_pAssembly->m_pImage;
        break;
    }

    if (m_pImage)
    {
        size_t m_sClassesCount = Functions.ImageGetClassCount(m_pImage);
        for (size_t i = 0U; m_sClassesCount > i; ++i)
        {
            Il2CppClass* m_pClass = Functions.ImageGetClass(m_pImage, i);
            if (m_pNamespace)
            {
                if (m_pNamespace[0] == '\0')
                {
                    if (m_pClass->m_pNamespace[0] != '\0')
                        continue;
                }
                else if (UTF8Utils::Strcmp(m_pClass->m_pNamespace, m_pNamespace) != 0)
                    continue;
            }
            m_pVector->emplace_back(m_pClass);
        }
    }
}
