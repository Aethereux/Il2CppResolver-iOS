//
//  Il2CppType.h
//  Asura
//
//  Created by Eux on 1/21/26.
//
#pragma once
#include <cstdint>
#include "../Utils/Macros.h"

struct Il2CppImage
{
    const char* m_pName;
    const char* m_pNameNoExt;
};

struct Il2CppAssemblyName
{
    const char* m_pName;
    const char* m_pCulture;
    const char* m_pHash;
    const char* m_pPublicKey;
    unsigned int m_uHash;
    int m_iHashLength;
    unsigned int m_uFlags;
    int m_iMajor;
    int m_iMinor;
    int m_iBuild;
    int m_bRevision;
    unsigned char m_uPublicKeyToken[8];
};

struct Il2CppAssembly
{
    Il2CppImage* m_pImage;
    unsigned int m_uToken;
    int m_ReferencedAssemblyStart;
    int m_ReferencedAssemblyCount;
    Il2CppAssemblyName m_aName;
};

struct Il2CppClass
{
    void* m_pImage;
    void* m_pGC;
    const char* m_pName;
    const char* m_pNamespace;
    void* m_pValue;
    void* m_pArgs;
    Il2CppClass* m_pElementClass;
    Il2CppClass* m_pCastClass;
    Il2CppClass* m_pDeclareClass;
    Il2CppClass* m_pParentClass;
    void* m_pGenericClass;
    void* m_pTypeDefinition;
    void* m_pInteropData;
    void* m_pFields;
    void* m_pEvents;
    void* m_pProperties;
    void** m_pMethods;
    Il2CppClass** m_pNestedTypes;
    Il2CppClass** m_ImplementedInterfaces;
    void* m_pInterfaceOffsets;
    void* m_pStaticFields;
    void* m_pRGCTX;
};

struct Il2CppObject
{
    Il2CppClass* m_pClass = nullptr;
    void* m_pMonitor = nullptr;
};

#ifdef UNITY_VERSION_2022_3_8F1
struct Il2CppType
{
    void* data;
    unsigned int bits;
};
#else
struct Il2CppType
{
    union
    {
        void* m_pDummy;
        unsigned int m_uClassIndex;
        Il2CppType* m_pType;
        void* m_pArray;
        unsigned int m_uGenericParameterIndex;
        void* m_pGenericClass;
    };
    unsigned int m_uAttributes : 16;
    unsigned int m_uType : 8;
    unsigned int m_uMods : 6;
    unsigned int m_uByref : 1;
    unsigned int m_uPinned : 1;
};
#endif

struct Il2CppFieldInfo
{
    const char* m_pName;
    Il2CppType* m_pType;
    Il2CppClass* m_pParentClass;
    int m_iOffset;
    int m_iAttributeIndex;
    unsigned int m_uToken;
};

struct Il2CppParameterInfo
{
    const char* m_pName;
    int m_iPosition;
    unsigned int m_uToken;
    Il2CppType* m_pParameterType;
};

struct Il2CppMethodInfo
{
    void* m_pMethodPointer;
#ifdef UNITY_VERSION_2022_3_8F1
    void* m_pVirtualMethodPointer;
#endif
    void* m_pInvokerMethod;
    const char* m_pName;
    Il2CppClass* m_pClass;
    Il2CppType* m_pReturnType;
#ifdef UNITY_VERSION_2022_3_8F1
    Il2CppType** m_pParameters;
#else
    Il2CppParameterInfo* m_pParameters;
#endif

    union
    {
        void* m_pRGCTX;
        void* m_pMethodDefinition;
    };

    union
    {
        void* m_pGenericMethod;
        void* m_pGenericContainer;
    };

    unsigned int m_uToken;
    unsigned short m_uFlags;
    unsigned short m_uFlags2;
    unsigned short m_uSlot;
    unsigned char m_uArgsCount;
    unsigned char m_uGeneric : 1;
    unsigned char m_uInflated : 1;
    unsigned char m_uWrapperType : 1;
    unsigned char m_uMarshaledFromNative : 1;
};

struct Il2CppPropertyInfo
{
    Il2CppClass* m_pParentClass;
    const char* m_pName;
    Il2CppMethodInfo* m_pGet;
    Il2CppMethodInfo* m_pSet;
    unsigned int m_uAttributes;
    unsigned int m_uToken;
};

struct Il2CppArrayBounds
{
    uintptr_t m_uLength;
    int m_iLowerBound;
};

