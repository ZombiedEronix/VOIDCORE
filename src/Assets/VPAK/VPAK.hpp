#pragma once
#include <cstdint>
#include <string_view>

namespace VoidCore::VPAK
{
#pragma pack(push, 1)
    struct VPAKHeader
    {
        char magic[4] = {'V', 'P', 'A', 'K'};
        uint32_t version = 1;
        uint32_t fileCount = 0;
        uint64_t tocOffset = 0;
    };

    struct PakEntry
    {
        uint64_t pathHash;
        uint64_t dataOffset;
        uint32_t compressedSize;
        uint32_t originalSize;
        uint8_t isCompressed;
    };
#pragma pack(pop)

    constexpr uint64_t HashAssetPath(std::string_view path)
    {
        uint64_t hash = 14695981039346656037ULL;
        for (char c : path)
        {
            if (c == '\\')
                c = '/';
            hash ^= static_cast<uint8_t>(c);
            hash *= 1099511628211ULL;
        }
        return hash;
    }
}
