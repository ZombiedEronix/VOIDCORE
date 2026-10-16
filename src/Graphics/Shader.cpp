#include "Shader.h"
#include <fstream>
#include "Device/RenderFactory.hpp"
#include "Core/Debug.h"

Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    std::string vs = get_file_contents(vertexFile);
    std::string fs = get_file_contents(fragmentFile);
    impl = VoidCore::RHI::RenderFactory::CreateShader(vs, fs);
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

void Shader::Load()
{
    if (impl) impl->Use();
}

void Shader::SetInt(const std::string &name, int value)
{
    if (impl) impl->SetInt(name, value);
}

void Shader::SetFloat(const std::string &name, float value)
{
    if (impl) impl->SetFloat(name, value);
}

unsigned int Shader::GetID() const
{
    return impl ? impl->GetID() : 0;
}
