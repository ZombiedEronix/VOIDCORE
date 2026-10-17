#include "Shader.h"
#include <fstream>
#include "Core/Debug.h"

using namespace VoidCore::RHI;

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

    const std::string vsSrc = get_file_contents(vertexFile);
    const std::string fsSrc = get_file_contents(fragmentFile);

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
