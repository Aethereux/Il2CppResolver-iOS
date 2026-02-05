//
//  Helpers.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 2/5/26.
//

#pragma once
#include "../Utils/Memory.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>


struct MemRange {
    uintptr_t start;
    uintptr_t end;
    uintptr_t headerOffset;
};

static std::filesystem::path GetDocumentsPath()
{
    return std::filesystem::path(getenv("HOME")) / "Documents";
}

static void DumpToFile(const char* FileName, const std::vector<std::string>& Data)
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

static std::string ToHex(uintptr_t value)
{
    std::stringstream ss;
    ss << std::hex << value;
    return ss.str();
}

static bool SegNameEquals(const char* target, const char* source) {
    return strncmp(target, source, 16) == 0;
}


static std::string SanitizeTypeInfo(std::string name) {
    // 1. Explicitly Handle Arrays so they don't look like duplicates
    // "Camera[]" -> "Camera_Array"
    size_t pos = 0;
    while ((pos = name.find("[]", pos)) != std::string::npos) {
         name.replace(pos, 2, "_Array");
         pos += 6; // Move past replacement
    }

    // 2. Replace bad chars
    std::replace(name.begin(), name.end(), '.', '_');
    std::replace(name.begin(), name.end(), '/', '_');
    std::replace(name.begin(), name.end(), '`', '_');
    std::replace(name.begin(), name.end(), ' ', '_');
    std::replace(name.begin(), name.end(), '<', '_');
    std::replace(name.begin(), name.end(), '>', '_');
    std::replace(name.begin(), name.end(), ',', '_');
    
    // Remove pointer stars if any
    name.erase(std::remove(name.begin(), name.end(), '*'), name.end());
    
    return name;
}

static std::string SanitizeClass(std::string name)
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
