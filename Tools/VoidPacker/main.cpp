#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <lz4.h>

#include "Assets/VPAK/VPAK.hpp"

namespace fs = std::filesystem;
using namespace VoidCore::VPAK;

fs::path ResolveInclude(const std::string &incName, const fs::path &currentDir, const fs::path &gameAssets, const fs::path &engineAssets)
{
    fs::path localPath = currentDir / incName;
    if (fs::exists(localPath) && fs::is_regular_file(localPath))
        return localPath;

    fs::path engineShaderPath = engineAssets / "shaders" / incName;
    if (fs::exists(engineShaderPath) && fs::is_regular_file(engineShaderPath))
        return engineShaderPath;

    return {};
}

std::string PreprocessShader(const fs::path &filePath,
                             const fs::path &gameAssets,
                             const fs::path &engineAssets,
                             std::unordered_set<std::string> &includedFiles)
{
    std::string canonical = fs::weakly_canonical(filePath).string();
    if (includedFiles.count(canonical))
    {
        return "";
    }
    includedFiles.insert(canonical);

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "[VoidPacker] ERROR: Cannot open shader: " << filePath << "\n";
        std::exit(1);
    }

    std::string result;
    std::string line;
    std::regex includeRegex(R"(^\s*#include\s+["<]([^">]+)[">])");

    while (std::getline(file, line))
    {
        if (line.find("#section") != std::string::npos)
        {
            includedFiles.clear();
            includedFiles.insert(canonical);
            result += line + "\n";
            continue;
        }

        std::smatch match;
        if (std::regex_search(line, match, includeRegex))
        {
            std::string incName = match[1].str();
            fs::path resolved = ResolveInclude(incName, filePath.parent_path(), gameAssets, engineAssets);
            if (resolved.empty())
            {
                std::cerr << "[VoidPacker] ERROR: Unresolved #include \"" << incName << "\" in " << filePath << "\n";
                std::exit(1);
            }
            result += PreprocessShader(resolved, gameAssets, engineAssets, includedFiles) + "\n";
        }
        else
        {
            result += line + "\n";
        }
    }
    return result;
}

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
        std::cerr << "Usage: VoidPacker --engine-assets <dir> --game-assets <dir> --output <file.vpak>\n";
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
        std::cerr << "[VoidPacker] Failed to create output file: " << outputPak << "\n";
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
            std::string processed = PreprocessShader(fullPath, gameAssets, engineAssets, included);
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

    std::cout << "[VoidPacker] Packed " << entries.size() << " assets into " << outputPak << "\n";
    return 0;
}
