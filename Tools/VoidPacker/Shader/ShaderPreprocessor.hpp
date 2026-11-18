#pragma once
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <regex>
#include <unordered_set>

#include "Core/Debug.h"

namespace fs = std::filesystem;

class ShaderPreprocessor
{
public:
    static bool Preprocess(const fs::path &filePath, const fs::path &gameAssets, const fs::path &engineAssets, std::string &vsOut, std::string &fsOut)
    {
        std::unordered_set<std::string> includedFiles;
        std::string fullSource = PreprocessRecursive(filePath, gameAssets, engineAssets, includedFiles);

        return SplitSections(fullSource, vsOut, fsOut);
    }

private:
    static std::string PreprocessRecursive(const fs::path &filePath, const fs::path &gameAssets, const fs::path &engineAssets, std::unordered_set<std::string> &includedFiles)
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
            Debug::LogError("Cannot open shader: ", filePath);
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
                    Debug::LogError("Unresolved #include \"", incName, "\" in ", filePath);
                    std::exit(1);
                }
                result += PreprocessRecursive(resolved, gameAssets, engineAssets, includedFiles) + "\n";
            }
            else
            {
                result += line + "\n";
            }
        }
        return result;
    }

    static bool SplitSections(const std::string &fullSource, std::string &vsOut, std::string &fsOut)
    {
        std::stringstream stream(fullSource);
        std::string line;
        std::stringstream vs, fs;
        std::stringstream *current = nullptr;
        bool hasVertex = false;
        bool hasFragment = false;

        while (std::getline(stream, line))
        {
            if (line.find("#section vertex") != std::string::npos)
            {
                current = &vs;
                hasVertex = true;
                continue;
            }
            if (line.find("#section fragment") != std::string::npos)
            {
                current = &fs;
                hasFragment = true;
                continue;
            }

            if (current)
            {
                *current << line << "\n";
            }
        }

        if (!hasVertex || !hasFragment)
            return false;

        vsOut = vs.str();
        fsOut = fs.str();
        return true;
    }

    static fs::path ResolveInclude(const std::string &incName, const fs::path &currentDir, const fs::path &gameAssets, const fs::path &engineAssets)
    {
        fs::path localPath = currentDir / incName;
        if (fs::exists(localPath) && fs::is_regular_file(localPath))
            return localPath;

        fs::path gameShaderPath = gameAssets / "shaders" / incName;
        if (fs::exists(gameShaderPath) && fs::is_regular_file(gameShaderPath))
            return gameShaderPath;

        fs::path engineShaderPath = engineAssets / "shaders" / incName;
        if (fs::exists(engineShaderPath) && fs::is_regular_file(engineShaderPath))
            return engineShaderPath;

        return {};
    }
};