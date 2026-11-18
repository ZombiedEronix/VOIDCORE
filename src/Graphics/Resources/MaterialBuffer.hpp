#pragma once
#include "UniformInfo.hpp"
#include <unordered_map>
#include <string>

struct MaterialBuffer
{
    uint32_t binding = 1;
    uint32_t totalSize = 0;
    std::unordered_map<std::string, UniformInfo> fields;

    bool IsValid() const { return totalSize > 0; }
};