#pragma once
#include <string>
#include "Device/RHI.hpp"

std::string get_file_contents(const char *filename);

class Shader
{
private:
    VoidCore::RHI::Ref<VoidCore::RHI::IShader> vs;
    VoidCore::RHI::Ref<VoidCore::RHI::IShader> fs;

public:
    Shader(const char *shaderFile);
    bool SplitCombinedSource(const std::string &fullSrc, std::string &outVs, std::string &outFs, const std::string &filepath);
    VoidCore::RHI::IShader *GetVS() const { return vs.get(); }
    VoidCore::RHI::IShader *GetFS() const { return fs.get(); }
    bool IsValid() const { return vs && fs; }
};
