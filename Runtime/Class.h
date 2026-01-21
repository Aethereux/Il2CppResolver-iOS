//
//  Class.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//
#pragma once
#include "../Data/Il2Cpp.h"
#include "../Globals.h"
#include "Domain.h"

namespace UnityClass
{
    FORCEINLINE Il2CppFieldInfo* GetFields(Il2CppClass* m_pClass, void** m_pIterator)
    {
        if (!Functions.ClassGetFields)
            return nullptr;
        
        return Functions.ClassGetFields(m_pClass, m_pIterator);
    }

    FORCEINLINE Il2CppType* GetType(Il2CppClass* m_pClass)
    {
        if (!Functions.ClassGetType)
            return nullptr;
        
        return Functions.ClassGetType(m_pClass);
    }

    FORCEINLINE Il2CppObject* GetSystemType(Il2CppClass* m_pClass)
    {
        if (!Functions.TypeGetObject)
            return nullptr;
        
        return Functions.TypeGetObject(GetType(m_pClass));
    }

    FORCEINLINE Il2CppClass* GetFromName(Il2CppImage* m_pImage, const char* m_pNamespace, const char* m_pName)
    {
        if (!Functions.ClassFromName)
            return nullptr;
        
        return Functions.ClassFromName(m_pImage, m_pNamespace, m_pName);
    }

    void FetchFields(Il2CppClass* m_pClass, std::vector<Il2CppFieldInfo*>* m_pVector, void* m_pFieldIterator = nullptr);
    FORCEINLINE Il2CppMethodInfo* GetMethods(Il2CppClass* m_pClass, void** m_pIterator)
    {
        if (!Functions.ClassGetMethods)
            return nullptr;
        
        return Functions.ClassGetMethods(m_pClass, m_pIterator);
    }

    void FetchMethods(Il2CppClass* m_pClass, std::vector<Il2CppMethodInfo*>* m_pVector, void* m_pMethodIterator = nullptr);
    Il2CppClass* Find(const char* m_pName);
    void* GetMethodPointer(const char* m_pClassName, const char* m_pMethodName, std::initializer_list<const char*> m_vNames);
    Il2CppClass* FilterClass(std::vector<Il2CppClass*>* m_pClasses, std::initializer_list<const char*> m_vNames, int m_iFoundCount = -1);
    void* FilterClassToMethodPointer(std::vector<Il2CppClass*>* m_pClasses, const char* m_pMethodName, int m_iArgs = -1);

    FORCEINLINE Il2CppObject* GetSystemType(const char* m_pClassName)
    {
        Il2CppClass* m_pClass = Find(m_pClassName);
        if (!m_pClass)
            return nullptr;

        return GetSystemType(m_pClass);
    }

    void FetchClasses(std::vector<Il2CppClass*>* m_pVector, const char* m_pModuleName, const char* m_pNamespace);

    namespace Utils
    {
        FORCEINLINE int GetFieldOffset(Il2CppClass* m_pClass, const char* m_pName)
        {
            void* m_pFieldIterator = nullptr;
            while (1)
            {
                Il2CppFieldInfo* m_pField = GetFields(m_pClass, &m_pFieldIterator);
                if (!m_pField)
                    break;
                
                if (UTF8Utils::Strcmp(m_pField->m_pName, m_pName) == 0)
                    return m_pField->m_iOffset;
            }
            return -1;
        }
        
        FORCEINLINE int GetFieldOffset(const char* m_pClassName, const char* m_pName)
        {
            Il2CppClass* m_pClass = Find(m_pClassName);
            if (m_pClass)
                return GetFieldOffset(m_pClass, m_pName);
            
            return -1;
        }
        
        FORCEINLINE void SetStaticField(Il2CppClass* m_pClass, const char* m_pMemberName, void* m_pValue) {
            if (!Functions.ClassGetFieldFromName || !Functions.FieldStaticSetValue)
                return;
            
            Il2CppFieldInfo* m_pField = Functions.ClassGetFieldFromName(m_pClass, m_pMemberName);
            if (m_pField)
            {
                Functions.FieldStaticSetValue(m_pField, m_pValue);
            }
        }
        
        FORCEINLINE void* GetStaticField(Il2CppClass* m_pClass, const char* m_pMemberName) {
            if (!Functions.ClassGetFieldFromName || !Functions.FieldStaticGetValue)
                return nullptr;
            
            Il2CppFieldInfo* m_pField = Functions.ClassGetFieldFromName(m_pClass, m_pMemberName);
            void* m_pValue = nullptr;
            if (m_pField)
                Functions.FieldStaticGetValue(m_pField, &m_pValue);
            
            return m_pValue;
        }
        
        FORCEINLINE void* GetStaticField(const char* m_pClassName, const char* m_pMemberName)
        {
            Il2CppClass* m_pClass = Find(m_pClassName);
            if (m_pClass)
                return GetStaticField(m_pClass, m_pMemberName);
            
            return nullptr;
        }
        
        FORCEINLINE void* GetMethodPointer(Il2CppClass* m_pClass, const char* m_pMethodName, int m_iArgs = -1)
        {
            if (!Functions.ClassGetMethodFromName)
                return nullptr;
            
            Il2CppMethodInfo* pMethod = Functions.ClassGetMethodFromName(m_pClass, m_pMethodName, m_iArgs);
            if (!pMethod)
                return nullptr;
            
            return pMethod->m_pMethodPointer;
        }
        
        FORCEINLINE void* GetMethodPointer(const char* m_pClassName, const char* m_pMethodName, int m_iArgs = -1)
        {
            Il2CppClass* m_pClass = Find(m_pClassName);
            if (m_pClass)
                return GetMethodPointer(m_pClass, m_pMethodName, m_iArgs);
            
            return nullptr;
        }
        
        FORCEINLINE uint64_t GetMethodPointerRVA(Il2CppClass* m_pClass, const char* m_pMethodName, int m_iArgs = -1)
        {
            void* methodPointer = GetMethodPointer(m_pClass, m_pMethodName, m_iArgs);
            if (!methodPointer)
                return 0;
            
            uint64_t rvaOffset = (uint64_t)Globals.UnityFramework.GetOffset(methodPointer); // VMAddr
            return rvaOffset;
        }
        
        FORCEINLINE uint64_t GetMethodPointerRVA(const char* m_pClassName, const char* m_pMethodName, int m_iArgs = -1) {
            Il2CppClass* m_pClass = Find(m_pClassName);
            if (m_pClass)
                return GetMethodPointerRVA(m_pClass, m_pMethodName, m_iArgs);
            
            return 0;
        }
        
        FORCEINLINE const char* MethodGetParamName(Il2CppMethodInfo* m_pMethodInfo, uint32_t index)
        {
            if (!Functions.MethodGetParamName)
                return nullptr;
            
            if (index >= m_pMethodInfo->m_uArgsCount)
                return nullptr;
            
            return Functions.MethodGetParamName(m_pMethodInfo, index);
        }
        
        FORCEINLINE Il2CppType* GetMethodParamType(Il2CppMethodInfo* m_pMethodInfo, uint32_t index)
        {
            if (!Functions.MethodGetParam)
                return nullptr;
            
            if (index >= m_pMethodInfo->m_uArgsCount)
                return nullptr;
            
            return Functions.MethodGetParam(m_pMethodInfo, index);
        }
        
        FORCEINLINE Il2CppClass* ClassFromType(Il2CppType* type)
        {
            if (!Functions.ClassFromIl2cppType)
                return nullptr;
            
            return Functions.ClassFromIl2cppType(type);
        }
    }

    enum class m_eClassPropType : int
    {
        Unknown = 0,
        Field,            // Member of class that can be accessed directly by RVA
        Property,        // Member of class that can be accessed by calling function
        Method,            // Function of class
    };

    // Main Class
    class CClass
    {
    public:
        Il2CppObject m_Object;
        void* m_CachedPtr = nullptr;

        // Wrappers for namespace, ah...
        FORCEINLINE Il2CppFieldInfo* GetFields(void** m_pIterator)
        {
            return UnityClass::GetFields(m_Object.m_pClass, m_pIterator);
        }

        FORCEINLINE void FetchFields(std::vector<Il2CppFieldInfo*>* m_pVector, void* m_pFieldIterator = nullptr)
        {
            UnityClass::FetchFields(m_Object.m_pClass, m_pVector, m_pFieldIterator);
        }

        FORCEINLINE Il2CppMethodInfo* GetMethods(void** m_pIterator)
        {
            return UnityClass::GetMethods(m_Object.m_pClass, m_pIterator);
        }

        FORCEINLINE void FetchMethods(std::vector<Il2CppMethodInfo*>* m_pVector, void* m_pMethodIterator = nullptr)
        {
            UnityClass::FetchMethods(m_Object.m_pClass, m_pVector, m_pMethodIterator);
        }

        FORCEINLINE void* GetMethodPointer(const char* m_pMethodName, int m_iArgs = -1)
        {
            return UnityClass::Utils::GetMethodPointer(m_Object.m_pClass, m_pMethodName, m_iArgs);
        }

        FORCEINLINE m_eClassPropType GetPropType(const char* m_pPropType)
        {
            if (!Functions.ClassGetFieldFromName || !Functions.ClassGetPropertyFromName)
                return m_eClassPropType::Unknown;
            
            Il2CppFieldInfo* pField = Functions.ClassGetFieldFromName(m_Object.m_pClass, m_pPropType);
            if (pField)
                return m_eClassPropType::Field;

            Il2CppPropertyInfo* pProperty = Functions.ClassGetPropertyFromName(m_Object.m_pClass, m_pPropType);
            if (pProperty)
                return m_eClassPropType::Property;

            Il2CppMethodInfo* pMethod = Functions.ClassGetMethodFromName(m_Object.m_pClass, m_pPropType, -1);
            if (pMethod)
                return m_eClassPropType::Method;
            
            return m_eClassPropType::Unknown;
        }

        // Call Method
        template<typename TReturn, typename... TArgs>
        FORCEINLINE TReturn CallMethod(void* m_pMethod, TArgs... tArgs) { return reinterpret_cast<TReturn(*)(void*, TArgs...)>(m_pMethod)(this, tArgs...); }

        template<typename TReturn, typename... TArgs>
        FORCEINLINE TReturn CallMethod(const char* m_pMethodName, TArgs... tArgs) { return CallMethod<TReturn>(GetMethodPointer(m_pMethodName), tArgs...); }

        template<typename TReturn, typename... TArgs>
        FORCEINLINE TReturn CallMethodSafe(void* m_pMethod, TArgs... tArgs)
        {
            if (!m_pMethod)
            {

                TReturn m_tDefault = {}; // void goes like illegal use of type. (use void* and fuck them)
                return m_tDefault;
            }

            return CallMethod<TReturn>(m_pMethod, tArgs...);
        }

        template<typename TReturn, typename... TArgs>
        FORCEINLINE TReturn CallMethodSafe(const char* m_pMethodName, TArgs... tArgs) { return CallMethodSafe<TReturn>(GetMethodPointer(m_pMethodName), tArgs...); }

        // Properties/Fields

        template<typename T>
        FORCEINLINE T GetPropertyValue(const char* m_pPropertyName)
        {
            T tDefault = {};
            if (!Functions.ClassGetPropertyFromName)
                return tDefault;
            
            Il2CppPropertyInfo* pProperty = Functions.ClassGetPropertyFromName(m_Object.m_pClass, m_pPropertyName);
            if (pProperty && pProperty->m_pGet)
                return reinterpret_cast<T(*)(void*)>(pProperty->m_pGet->m_pMethodPointer)(this);

            return tDefault;
        }

        template<typename T>
        FORCEINLINE void SetPropertyValue(const char* m_pPropertyName, T m_tValue)
        {
            Il2CppPropertyInfo* pProperty = Functions.ClassGetPropertyFromName(m_Object.m_pClass, m_pPropertyName);
            if (pProperty && pProperty->m_pSet)
                return reinterpret_cast<void(*)(void*, T)>(pProperty->m_pSet->m_pMethodPointer)(this, m_tValue);
        }

        template<typename T>
        FORCEINLINE T GetMemberValue(int m_iOffset)
        {
            return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + m_iOffset);
        }

        template<typename T>
        FORCEINLINE void SetMemberValue(int m_iOffset, T m_tValue)
        {
            *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + m_iOffset) = m_tValue;
        }

        template<typename T>
        FORCEINLINE T GetMemberValue(Il2CppFieldInfo* m_pField)
        {
            if (!m_pField || 0 > m_pField->m_iOffset)
            {
                T m_tDefault = {};
                return m_tDefault;
            }

            return GetMemberValue<T>(m_pField->m_iOffset);
        }

        template<typename T>
        FORCEINLINE void SetMemberValue(Il2CppFieldInfo* m_pField, T m_tValue)
        {
            if (!m_pField || 0 > m_pField->m_iOffset)
                return;

            SetMemberValue<T>(m_pField->m_iOffset, m_tValue);
        }

        template<typename T>
        FORCEINLINE T GetMemberValue(const char* m_pMemberName)
        {
            T tDefault = {};
            if (!Functions.ClassGetFieldFromName)
                return tDefault;
            
            Il2CppFieldInfo* pField = Functions.ClassGetFieldFromName(m_Object.m_pClass, m_pMemberName);
            if (pField)
            {
                if (pField->m_iOffset >= 0) return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + pField->m_iOffset);
            }
            else
                return GetPropertyValue<T>(m_pMemberName);

            return tDefault;
        }

        template<typename T>
        FORCEINLINE void SetMemberValue(const char* m_pMemberName, T m_tValue)
        {
            if (!Functions.ClassGetFieldFromName)
                return;
            
            Il2CppFieldInfo* pField = Functions.ClassGetFieldFromName(m_Object.m_pClass, m_pMemberName);
            if (pField)
            {
                if (pField->m_iOffset >= 0)
                    *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + pField->m_iOffset) = m_tValue;
                return;
            }

            SetPropertyValue<T>(m_pMemberName, m_tValue);
        }

        template<typename T>
        FORCEINLINE T GetObscuredViaOffset(int m_iOffset)
        {
            if (m_iOffset >= 0)
            {
                switch (sizeof(T))
                {
                    case sizeof(double) :
                    {
                        long long m_lKey = *reinterpret_cast<long long*>(reinterpret_cast<uintptr_t>(this) + m_iOffset);
                        long long m_lValue = *reinterpret_cast<long long*>(reinterpret_cast<uintptr_t>(this) + m_iOffset + sizeof(m_lKey));

                        m_lValue ^= m_lKey;
                        return *reinterpret_cast<T*>(&m_lValue);
                    }
                    break;
                    case sizeof(int) :
                    {
                        int m_iKey = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + m_iOffset);
                        int m_iValue = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + m_iOffset + sizeof(m_iKey));

                        m_iValue ^= m_iKey;
                        return *reinterpret_cast<T*>(&m_iValue);
                    }
                    break;
                    case sizeof(bool) :
                    {
                        unsigned char m_uKey = *reinterpret_cast<unsigned char*>(reinterpret_cast<uintptr_t>(this) + m_iOffset);
                        int m_iValue = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + m_iOffset + sizeof(m_uKey));

                        m_iValue ^= m_uKey;
                        return *reinterpret_cast<T*>(&m_iValue);
                    }
                    break;
                }
            }

            T m_tDefault = { 0 };
            return m_tDefault;
        }

        template<typename T>
        FORCEINLINE T GetObscuredValue(const char* m_pMemberName)
        {
            if (!Functions.ClassGetFieldFromName)
                return -1;
            
            Il2CppFieldInfo* m_pField = Functions.ClassGetFieldFromName(m_Object.m_pClass, m_pMemberName);
            return GetObscuredViaOffset<T>(m_pField ? m_pField->m_iOffset : -1);
        }

        template<typename T>
        FORCEINLINE void SetObscuredViaOffset(int m_iOffset, T m_tValue)
        {
            if (0 > m_iOffset)
                return;

            switch (sizeof(T))
            {
                case sizeof(double) :
                {
                    long long m_lKey = *reinterpret_cast<long long*>(reinterpret_cast<uintptr_t>(this) + m_iOffset);
                    long long* m_pValue = reinterpret_cast<long long*>(reinterpret_cast<uintptr_t>(this) + m_iOffset + sizeof(m_lKey));

                    *m_pValue = *reinterpret_cast<long long*>(&m_tValue) ^ m_lKey;
                }
                break;
                case sizeof(int) :
                {
                    int m_iKey = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + m_iOffset);
                    int* m_pValue = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + m_iOffset + sizeof(m_iKey));

                    *m_pValue = *reinterpret_cast<int*>(&m_tValue) ^ m_iKey;
                }
                break;
                case sizeof(bool) :
                {
                    unsigned char m_uKey = *reinterpret_cast<unsigned char*>(reinterpret_cast<uintptr_t>(this) + m_iOffset);
                    int* m_pValue = reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + m_iOffset + sizeof(m_uKey));

                    *m_pValue = *reinterpret_cast<int*>(&m_tValue) ^ m_uKey;
                }
                break;
            }
        }

        template<typename T>
        void SetObscuredValue(const char* m_pMemberName, T m_tValue)
        {
            if (!Functions.ClassGetFieldFromName)
                return;
            
            Il2CppFieldInfo* m_pField = Functions.ClassGetFieldFromName(m_Object.m_pClass, m_pMemberName);
            if (!m_pField)
                return;

            SetObscuredViaOffset<T>(m_pField->m_iOffset, m_tValue);
        }
    };
}

