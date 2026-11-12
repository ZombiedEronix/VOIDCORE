#include <fstream>
#include <unordered_set>

#include "Shader.hpp"
#include "Core/Debug.h"

using namespace VoidCore::RHI;

std::string PreprocessShaderSource(const std::string &filepath, std::unordered_set<std::string> &includedFiles)
{
    if (includedFiles.count(filepath) > 0)
        return "";
    includedFiles.insert(filepath);

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        Debug::LogError("Shader preprocessor error: " + filepath);
        return "";
    }

    std::stringstream result;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.rfind("#include ", 0) == 0)
        {
            size_t startQuote = line.find('"');
            size_t endQuote = line.rfind('"');

            if (startQuote != std::string::npos && endQuote != std::string::npos && startQuote < endQuote)
            {
                std::string includeFileName = line.substr(startQuote + 1, endQuote - startQuote - 1);

                size_t lastSlash = filepath.find_last_of("/\\");
                std::string directory = (lastSlash != std::string::npos) ? filepath.substr(0, lastSlash + 1) : "";
                std::string includeFullPath = directory + includeFileName;

                result << PreprocessShaderSource(includeFullPath, includedFiles) << "\n";
            }
        }
        else
        {
            result << line << "\n";
        }
    }
    return result.str();
}

bool Shader::SplitCombinedSource(const std::string &fullSrc, std::string &outVs, std::string &outFs, const std::string &filepath)
{
    std::stringstream fileStream(fullSrc);
    std::string line;

    std::stringstream vsStream, fsStream;
    std::stringstream *currentStream = nullptr;
    int sectionCount = 0;

    while (std::getline(fileStream, line))
    {
        if (line.find("#section vertex") != std::string::npos)
        {
            currentStream = &vsStream;
            sectionCount++;
            continue;
        }
        else if (line.find("#section fragment") != std::string::npos)
        {
            currentStream = &fsStream;
            sectionCount++;
            continue;
        }

        if (currentStream)
        {
            *currentStream << line << "\n";
        }
    }

    if (sectionCount < 2)
    {
        Debug::LogError("Shader split error in " + filepath + ": Missing '#section vertex' or '#section fragment'");
        return false;
    }

    outVs = vsStream.str();
    outFs = fsStream.str();
    return true;
}

Shader::Shader(const char *combinedShaderFile)
{
    std::unordered_set<std::string> includes;
    std::string fullSource = PreprocessShaderSource(combinedShaderFile, includes);
    CompileFromSource(fullSource, combinedShaderFile);
}

std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    Debug::LogError("Failed to load shader file: ", filename);
    return "";
}

void Shader::CompileFromSource(const std::string &fullSource, const char *debugName)
{
    auto &device = GetDevice();
    const ShaderFormat format = device.GetCaps().shaderFormat;

    if (format != ShaderFormat::GLSL)
    {
        Debug::LogError("Shader: backend needs ", (int)format, " shaders, only GLSL loader is implemented");
        return;
    }

    if (fullSource.empty())
        return;

    std::string vsSrc, fsSrc;
    if (!SplitCombinedSource(fullSource, vsSrc, fsSrc, debugName))
    {
        return;
    }

    ShaderDesc d;
    d.format = format;

    d.stage = ShaderStage::Vertex;
    d.code = vsSrc.data();
    d.codeSize = vsSrc.size();
    d.debugName = debugName;
    vs = device.CreateShader(d);

    d.stage = ShaderStage::Fragment;
    d.code = fsSrc.data();
    d.codeSize = fsSrc.size();
    d.debugName = debugName;
    fs = device.CreateShader(d);
}

std::shared_ptr<Shader> Shader::CreateFromSource(const std::string &fullSource, const char *debugName)
{
    auto shader = std::shared_ptr<Shader>(new Shader());
    shader->CompileFromSource(fullSource, debugName);
    return shader;
}
