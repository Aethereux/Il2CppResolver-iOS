//
//  Image.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/21/26.
//
#pragma once
#include "../Data/Il2Cpp.h"
#include "Domain.h"
#include "../Utils/StringUtils.h"
#include "../Globals.h"

namespace Image
{
    FORCEINLINE Il2CppImage* GetByName(const char *image)
    {
        if (!Functions.AssembliesGetImage || !Functions.ImageGetName)
            return nullptr;
        // Retrieve the assemblies in the current domain
        size_t assemblyCount = 0;
        Il2CppAssembly **Assemblies = Domain::GetAssemblies(&assemblyCount);
        // Iterate over each assembly
        for (size_t i = 0; i < assemblyCount; ++i)
        {
            // Get the image from the current assembly
            void *img = Functions.AssembliesGetImage(Assemblies[i]);
            // Get the image name
            const char* imgName = Functions.ImageGetName(img);
            // Compare the image name with the one we are looking for
            if (UTF8Utils::Strcmp(imgName, image) == 0)
            {
                return reinterpret_cast<Il2CppImage*>(img); // Return the image cast to il2cppImage*
            }
        }
        
        // If no match is found, return nullptr
        return nullptr;
    }
}
