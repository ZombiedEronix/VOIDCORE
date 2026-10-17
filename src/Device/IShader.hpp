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
        const void *code = nullptr; // текст (GLSL/MSL) или бинарник (SPIRV/DXBC)
        size_t codeSize = 0;
        const char *entryPoint = "main";
        const char *debugName = nullptr;
    };

    // Одна стадия (VS или FS). Никаких Use()/SetInt()/SetFloat() по имени:
    // параметры идут через uniform-буферы и текстурные слоты (см. RHI.hpp).
    class IShader : public IResource
    {
    public:
        virtual ShaderStage GetStage() const = 0;
    };
}
