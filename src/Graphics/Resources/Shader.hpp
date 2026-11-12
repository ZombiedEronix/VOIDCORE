#pragma once
#include <string>
#include "Device/RHI.hpp"

std::string get_file_contents(const char *filename);

class Shader
{
private:
    VoidCore::RHI::Ref<VoidCore::RHI::IShader> vs;
    VoidCore::RHI::Ref<VoidCore::RHI::IShader> fs;

    void CompileFromSource(const std::string &fullSource, const char *debugName);

public:
    Shader(const char *shaderFile);
    Shader() = default;
    static std::shared_ptr<Shader> CreateFromSource(const std::string &fullSource, const char *debugName);

    bool SplitCombinedSource(const std::string &fullSrc, std::string &outVs, std::string &outFs, const std::string &filepath);
    VoidCore::RHI::IShader *GetVS() const { return vs.get(); }
    VoidCore::RHI::IShader *GetFS() const { return fs.get(); }
    bool IsValid() const { return vs && fs; }
};
