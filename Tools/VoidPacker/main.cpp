#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <iostream>
#include <vector>

#include <lz4.h>

#include "Assets/VPAK/VPAK.hpp"
#include "Shader/ShaderPreprocessor.hpp"

namespace fs = std::filesystem;
using namespace VoidCore::VPAK;


int main(int argc, char **argv)
{
    fs::path engineAssets, gameAssets, outputPak;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--engine-assets" && i + 1 < argc)
            engineAssets = argv[++i];
        else if (arg == "--game-assets" && i + 1 < argc)
            gameAssets = argv[++i];
        else if (arg == "--output" && i + 1 < argc)
            outputPak = argv[++i];
    }

    if (outputPak.empty())
    {
        Debug::LogError("Usage: VoidPacker --engine-assets <dir> --game-assets <dir> --output <file.vpak>\n");
        return 1;
    }

    std::unordered_map<std::string, fs::path> filesToPack;
    auto collectDir = [&](const fs::path &dir)
    {
        if (dir.empty() || !fs::exists(dir))
            return;
        for (const auto &entry : fs::recursive_directory_iterator(dir))
        {
            if (entry.is_regular_file())
            {
                std::string relPath = fs::relative(entry.path(), dir).generic_string();
                filesToPack[relPath] = entry.path();
            }
        }
    };

    collectDir(engineAssets);
    collectDir(gameAssets);

    fs::create_directories(outputPak.parent_path());
    std::ofstream out(outputPak, std::ios::binary);

    if (!out.is_open())
    {
        Debug::LogError("Failed to create output file: ", outputPak, "\n");
        return 1;
    }

    VPAKHeader header;
    header.fileCount = static_cast<uint32_t>(filesToPack.size());
    out.write(reinterpret_cast<const char *>(&header), sizeof(VPAKHeader));

    std::vector<PakEntry> entries;
    entries.reserve(filesToPack.size());

    for (const auto &[relPath, fullPath] : filesToPack)
    {
        std::vector<char> rawData;

        if (fullPath.extension() == ".glsl")
        {
            std::unordered_set<std::string> included;
            std::string processed = ShaderPreprocessor::Preprocess(fullPath, gameAssets, engineAssets, included);
            rawData.assign(processed.begin(), processed.end());
        }
        else
        {
            std::ifstream inFile(fullPath, std::ios::binary | std::ios::ate);
            size_t size = inFile.tellg();
            inFile.seekg(0);
            rawData.resize(size);
            inFile.read(rawData.data(), size);
        }

        uint32_t origSize = static_cast<uint32_t>(rawData.size());
        int maxDstSize = LZ4_compressBound(origSize);
        std::vector<char> compressedData(maxDstSize);

        int compSize = 0;
        if (origSize > 0)
        {
            compSize = LZ4_compress_default(rawData.data(), compressedData.data(), origSize, maxDstSize);
        }

        PakEntry entry{};
        entry.pathHash = HashAssetPath(relPath);
        entry.dataOffset = static_cast<uint64_t>(out.tellp());
        entry.originalSize = origSize;

        if (compSize > 0 && static_cast<uint32_t>(compSize) < origSize)
        {
            entry.compressedSize = static_cast<uint32_t>(compSize);
            entry.isCompressed = 1;
            out.write(compressedData.data(), compSize);
        }
        else
        {
            entry.compressedSize = origSize;
            entry.isCompressed = 0;
            if (origSize > 0)
                out.write(rawData.data(), origSize);
        }

        entries.push_back(entry);
    }

    header.tocOffset = static_cast<uint64_t>(out.tellp());
    out.write(reinterpret_cast<const char *>(entries.data()), entries.size() * sizeof(PakEntry));

    out.seekp(0);
    out.write(reinterpret_cast<const char *>(&header), sizeof(VPAKHeader));

    Debug::Log("Packed ", entries.size(), " assets into ", outputPak, "\n");
    return 0;
}
