#pragma once
#include <cstddef>
#include "Device/IResource.hpp"
#include "Device/RHITypes.hpp"

namespace VoidCore::RHI
{
    struct ShaderDesc
    {
        ShaderStage stage = ShaderStage::Vertex;
        ShaderFormat format = ShaderFormat::GLSL;
        const void *code = nullptr; // text (GLSL/MSL) or binary (SPIRV/DXBC)
        size_t codeSize = 0;
        const char *entryPoint = "main";
        const char *debugName = nullptr;
    };

    // One stage (VS or FS). No Use()/SetInt()/SetFloat() by name:
    // parameters go through uniform buffers and texture slots (see RHI.hpp).
    class IShader : public IResource
    {
    public:
        virtual ShaderStage GetStage() const = 0;
    };
}
