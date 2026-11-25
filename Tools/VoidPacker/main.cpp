#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <iostream>
#include <vector>
#include <lz4.h>

#include "Assets/VPAK/VPAK.hpp"
#include "Shader/SPIRVBuilder.hpp"
#include "Shader/GLSL/Compiler.hpp"
#include "Shader/ShaderPreprocessor.hpp"

namespace fs = std::filesystem;
using namespace VoidCore;
using namespace VoidPacker;
fs::path engineAssets, gameAssets, outputPak;
std::unordered_map<std::string, fs::path> filesToPack;

bool ReadArguments(int argc, char **argv)
{
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
        return false;
    }
    return true;
}

void collectDir(const fs::path &dir)
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
}

int main(int argc, char **argv)
{
    if (ReadArguments(argc, argv) == false)
        return 1;

    collectDir(engineAssets);
    collectDir(gameAssets);

    fs::create_directories(outputPak.parent_path());
    std::ofstream out(outputPak, std::ios::binary);

    if (!out.is_open())
    {
        Debug::LogError("Failed to create output file: ", outputPak, "\n");
        return 1;
    }

    VPAK::Header header;
    header.fileCount = static_cast<uint32_t>(filesToPack.size());
    out.write(reinterpret_cast<const char *>(&header), sizeof(VPAK::Header));

    std::vector<VPAK::Entry> entries;
    entries.reserve(filesToPack.size());

    for (const auto &[relPath, fullPath] : filesToPack)
    {
        std::vector<char> rawData;
        std::string archivePath = relPath;

        if (fullPath.extension() == ".glsl")
        {
            Debug::Log("Building: ", relPath);

            std::string vsSrc, fsSrc;
            if (!ShaderPreprocessor::Preprocess(fullPath, gameAssets, engineAssets, vsSrc, fsSrc))
            {
                continue;
            }

            std::vector<uint32_t> vsBin = GLSL::Compiler::CompileShader(vsSrc, GLSL::ShaderStage::Vertex, relPath + " [VS]");
            std::vector<uint32_t> fsBin = GLSL::Compiler::CompileShader(fsSrc, GLSL::ShaderStage::Fragment, relPath + " [FS]");

            if (vsBin.empty() || fsBin.empty())
            {
                Debug::LogError("Failed to compile shader: ", relPath);
                return 1;
            }

            std::vector<uint8_t> vcspBin = SPIRV::Builder::BuildVCSPBinary(vsBin, fsBin);
            rawData.assign(vcspBin.begin(), vcspBin.end());

            archivePath = fs::path(relPath).replace_extension(".vcsp").generic_string();
        }
        else
        {
            std::ifstream inFile(fullPath, std::ios::binary | std::ios::ate);
            if (!inFile.is_open())
            {
                Debug::LogError("Failed to read file: ", fullPath);
                continue;
            }
            size_t size = static_cast<size_t>(inFile.tellg());
            inFile.seekg(0);
            rawData.resize(size);
            if (size > 0)
                inFile.read(rawData.data(), size);
        }

        uint32_t origSize = static_cast<uint32_t>(rawData.size());
        int maxDstSize = LZ4_compressBound(origSize);
        std::vector<char> compressedData(maxDstSize);

        int compSize = (origSize > 0)
                           ? LZ4_compress_default(rawData.data(), compressedData.data(), origSize, maxDstSize)
                           : 0;

        VPAK::Entry entry{};
        entry.pathHash = VPAK::HashAssetPath(archivePath);
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

    header.fileCount = static_cast<uint32_t>(entries.size());
    header.tocOffset = static_cast<uint64_t>(out.tellp());
    out.write(reinterpret_cast<const char *>(entries.data()), entries.size() * sizeof(VPAK::Entry));

    out.seekp(0);
    out.write(reinterpret_cast<const char *>(&header), sizeof(VPAK::Header));
    out.close();

    Debug::Log("Packed ", entries.size(), " assets into ", outputPak, "\n");
    return 0;
}
