#pragma once
#include <string>
#include "Device/RHI.hpp"
#include "Graphics/Resources/MaterialBuffer.hpp"
#include "Entity/Object.hpp"
std::string get_file_contents(const char *filename);

class Shader : public Object
{
private:
    VoidCore::RHI::Ref<VoidCore::RHI::IShader> vs;
    VoidCore::RHI::Ref<VoidCore::RHI::IShader> fs;
    MaterialBuffer materialLayout;

public:
    Shader(const void *data, size_t byteSize, const char *debugName);
    Shader() = default;
    VoidCore::RHI::IShader *GetVS() const { return vs.get(); }
    VoidCore::RHI::IShader *GetFS() const { return fs.get(); }
    const MaterialBuffer &GetMaterialLayout() const { return materialLayout; }
    bool IsValid() const { return vs && fs; }
};
