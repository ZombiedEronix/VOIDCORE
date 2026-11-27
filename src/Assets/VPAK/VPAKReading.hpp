#pragma once

#include "VPAK.hpp"
#include <cstdint>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace VoidCore::VPAK
{
    class AssetPakReader
    {
    public:
        bool Mount(const std::string &pakPath);
        bool ReadFile(std::string_view assetPath, std::vector<uint8_t> &outData);
        bool ReadTextFile(std::string_view assetPath, std::string &outText);

    private:
        std::ifstream m_File;
        std::unordered_map<uint64_t, Entry> m_Entries;
    };
}
