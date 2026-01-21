//
//  Domain.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//

#pragma once
#include "../Data/Il2Cpp.h"
#include "../Globals.h"

namespace Domain {
    FORCEINLINE void* Get()
    {
        if (!Functions.DomainGet)
            return nullptr;
        
        return Functions.DomainGet();
    }

    FORCEINLINE Il2CppAssembly** GetAssemblies(size_t* m_Size)
    {
        if (!Functions.DomainGetAssemblies)
            return nullptr;
        
        return Functions.DomainGetAssemblies(Get(), m_Size);
    }
}
