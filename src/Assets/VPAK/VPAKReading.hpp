#pragma once
#include "VPAK.hpp"
#include <lz4.h>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>

namespace VoidCore::VPAK
{
    class AssetPakReader
    {
    public:
        bool Mount(const std::string &pakPath)
        {
            m_File.open(pakPath, std::ios::binary);
            if (!m_File.is_open())
                return false;

            VPAKHeader header{};
            m_File.read(reinterpret_cast<char *>(&header), sizeof(VPAKHeader));

            if (std::memcmp(header.magic, "VPAK", 4) != 0 || header.version != 1)
                return false;

            m_File.seekg(static_cast<std::streamoff>(header.tocOffset), std::ios::beg);
            std::vector<PakEntry> entries(header.fileCount);
            m_File.read(reinterpret_cast<char *>(entries.data()), header.fileCount * sizeof(PakEntry));

            m_Entries.clear();
            for (const auto &entry : entries)
            {
                m_Entries[entry.pathHash] = entry;
            }

            return true;
        }

        bool ReadFile(std::string_view assetPath, std::vector<uint8_t> &outData)
        {
            if (assetPath.starts_with("Assets/"))
                assetPath.remove_prefix(7);
            uint64_t hash = HashAssetPath(assetPath);
            auto it = m_Entries.find(hash);
            if (it == m_Entries.end())
                return false;

            const PakEntry &entry = it->second;
            outData.resize(entry.originalSize);

            m_File.seekg(static_cast<std::streamoff>(entry.dataOffset), std::ios::beg);

            if (entry.isCompressed)
            {
                std::vector<char> compressed(entry.compressedSize);
                m_File.read(compressed.data(), entry.compressedSize);

                int decompressedBytes = LZ4_decompress_safe(
                    compressed.data(),
                    reinterpret_cast<char *>(outData.data()),
                    static_cast<int>(entry.compressedSize),
                    static_cast<int>(entry.originalSize));

                return decompressedBytes == static_cast<int>(entry.originalSize);
            }
            else
            {
                m_File.read(reinterpret_cast<char *>(outData.data()), entry.originalSize);
                return m_File.good();
            }
        }

        bool ReadTextFile(std::string_view assetPath, std::string &outText)
        {
            std::vector<uint8_t> buffer;
            if (!ReadFile(assetPath, buffer))
                return false;

            outText.assign(reinterpret_cast<const char *>(buffer.data()), buffer.size());
            return true;
        }

    private:
        std::ifstream m_File;
        std::unordered_map<uint64_t, PakEntry> m_Entries;
    };
}
