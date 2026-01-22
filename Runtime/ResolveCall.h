//
//  ResolveCall.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once
#include "../Utils/Macros.h"
#include "../Globals.h"
#include "../Utils/Logger.hpp"

FORCEINLINE void* ResolveCall(const char* m_Name)
{
    if (!Functions.ResolveFunction)
        return nullptr;
    
    LOG_DEBUG("Resolving function: %s", m_Name);
    return Functions.ResolveFunction(m_Name);
}

template<typename FunctionType>
FORCEINLINE void ResolveFunction(FunctionType& Function, void* Address)
{
    Function = FunctionType(Address);
}
