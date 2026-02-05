//
//  DumpTypeInfo.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 2/5/26.
//

#pragma once
#include "Helpers.h"

static bool TryParseClass(void* potentialClass, std::string& outName, std::string& outNamespace)
{

    if ((uintptr_t)potentialClass < 0x10000)
        return false;
    
    if ((uintptr_t)potentialClass % sizeof(void*) != 0)
        return false;
    
    if (IMemoryUtils::Get()->IsBadReadPtr(potentialClass))
        return false;

    Il2CppClass* Klass = (Il2CppClass*)potentialClass;

    void* ImagePtr = Klass->m_pImage;
    if (!ImagePtr || IMemoryUtils::Get()->IsBadReadPtr(ImagePtr))
        return false;
    
    char* ImgNamePtr = *(char**)ImagePtr;
    if (!ImgNamePtr || IMemoryUtils::Get()->IsBadReadPtr(ImgNamePtr))
        return false;
    
    if (!isprint(ImgNamePtr[0]))
        return false;

    if (IMemoryUtils::Get()->IsBadReadPtr((void*)Klass->m_pName))
        return false;
    
    const char* Name = Klass->m_pName;
    if (!Name || Name[0] == '\0')
        return false;

    // Filter "Junk" names (Compiler generated, etc)
    if (!isalpha(Name[0]) && Name[0] != '_')
        return false;
    
    if (strchr(Name, '$'))
        return false;
    
    if (strstr(Name, "<"))
        return false;

    if (IMemoryUtils::Get()->IsBadReadPtr((void*)Klass->m_pNamespace))
        return false;

    // Success
    outName = Name;
    outNamespace = Klass->m_pNamespace ? Klass->m_pNamespace : "";
    return true;
}

static std::vector<MemRange> GetSearchRanges()
{
    std::vector<MemRange> ranges;
    IMemoryUtils::DyldInfo Framework = IMemoryUtils::GetDyldInfo("UnityFramework");
    const mach_header_64* header = Framework.Header;
    intptr_t slide = Framework.VMAddrSlide;
    
    if (!header) return ranges;

    const struct load_command* lc = (const struct load_command*)(header + 1);
    for (uint32_t i = 0; i < header->ncmds; i++)
    {
        if (lc->cmd == LC_SEGMENT_64) {
            const struct segment_command_64* seg = (const struct segment_command_64*)lc;
            if (SegNameEquals("__DATA", seg->segname) || SegNameEquals("__DATA_CONST", seg->segname) || SegNameEquals("__DATA_DIRTY", seg->segname)) {
                const struct section_64* sect = (const struct section_64*)(seg + 1);
                for (uint32_t j = 0; j < seg->nsects; j++) {
                    if (SegNameEquals("__data", sect[j].sectname) || SegNameEquals("__bss", sect[j].sectname) || SegNameEquals("__common", sect[j].sectname)) {
                        MemRange range;
                        range.start = (uintptr_t)sect[j].addr + slide;
                        range.end = range.start + sect[j].size;
                        range.headerOffset = (uintptr_t)header;
                        ranges.push_back(range);
                    }
                }
            }
        }
        lc = (const struct load_command*)((uintptr_t)lc + lc->cmdsize);
    }
    return ranges;
}

static void DumpTypeInfos()
{
    std::vector<MemRange> ranges = GetSearchRanges();
    std::vector<std::string> Data;
    std::set<std::string> GeneratedNames;

    Data.push_back("#pragma once\n");
    Data.push_back("#include <cstdint>\n\n");
    Data.push_back("namespace TypeInfos\n{\n");

    for (const auto& range : ranges)
    {
        for (uintptr_t ptr = range.start; ptr < range.end; ptr += sizeof(void*))
        {
            if (IMemoryUtils::Get()->IsBadReadPtr((void*)ptr))
                continue;
            
            void* candidatePtr = *(void**)ptr;

            // Fast Pre-Check
            if ((uintptr_t)candidatePtr < 0x10000)
                continue;

            std::string Name, Namespace;
            if (TryParseClass(candidatePtr, Name, Namespace))
            {
                std::string VarName = Name;
                if (!Namespace.empty())
                    VarName = Namespace + "_" + Name;
                
                VarName = SanitizeTypeInfo(VarName);
                if (GeneratedNames.count(VarName))
                    continue;
                
                GeneratedNames.insert(VarName);

                uintptr_t offset = ptr - range.headerOffset;
                std::string Output = "    constexpr uintptr_t " + VarName + "_TypeInfo = 0x" + ToHex(offset) + ";\n";
                Data.push_back(Output);
            }
        }
    }

    Data.push_back("}\n");
    DumpToFile("TypeInfos.h", Data);
}
