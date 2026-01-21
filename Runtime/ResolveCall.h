//
//  ResolveCall.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once
#include "../Utils/Macros.h"
#include "../Globals.h"

FORCEINLINE void* ResolveCall(const char* m_Name)
{
    if (!Functions.ResolveFunction)
        return nullptr;
    
    return Functions.ResolveFunction(m_Name);
}

template<typename FunctionType>
FORCEINLINE void ResolveFunction(FunctionType& Function, void* Address)
{
    Function = FunctionType(Address);
}
