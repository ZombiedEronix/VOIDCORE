#include <fstream>
#include <unordered_set>

#include "Shader.h"
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

Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    auto &device = GetDevice();
    const ShaderFormat format = device.GetCaps().shaderFormat;

    // TODO(Vulkan/D3D11/Metal): здесь выбирать файл под формат (.spv/.dxbc/.msl)
    // из результата оффлайн-компиляции. Пока бэкенд один — читаем GLSL как есть.
    if (format != ShaderFormat::GLSL)
    {
        Debug::LogError("Shader: backend needs ", (int)format, " shaders, only GLSL loader is implemented");
        return;
    }

    std::unordered_set<std::string> vsIncludes;
    std::unordered_set<std::string> fsIncludes;

    const std::string vsSrc = PreprocessShaderSource(vertexFile, vsIncludes);
    const std::string fsSrc = PreprocessShaderSource(fragmentFile, fsIncludes);


    ShaderDesc d;
    d.format = format;

    d.stage = ShaderStage::Vertex;
    d.code = vsSrc.data();
    d.codeSize = vsSrc.size();
    d.debugName = vertexFile;
    vs = device.CreateShader(d);

    d.stage = ShaderStage::Fragment;
    d.code = fsSrc.data();
    d.codeSize = fsSrc.size();
    d.debugName = fragmentFile;
    fs = device.CreateShader(d);
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
