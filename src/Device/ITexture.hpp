#pragma once
#include "Device/IResource.hpp"
#include "Device/RHITypes.hpp"

namespace VoidCore::RHI
{
    struct TextureDesc
    {
        uint32_t width = 1;
        uint32_t height = 1;
        uint32_t mipLevels = 1; // 0 = полная цепочка
        Format format = Format::RGBA8_UNORM;
        TextureUsage usage = TextureUsage::Sampled;
        // Данные mip 0, плотно упакованные (rowPitch = width * FormatSize).
        // Если mipLevels != 1 — остальные уровни генерируются автоматически.
        const void *initialData = nullptr;
        const char *debugName = nullptr;
    };

    class ITexture : public IResource
    {
    public:
        virtual const TextureDesc &GetDesc() const = 0;
    };

    struct SamplerDesc
    {
        Filter minFilter = Filter::Linear;
        Filter magFilter = Filter::Linear;
        Filter mipFilter = Filter::Linear;
        AddressMode addressU = AddressMode::Repeat;
        AddressMode addressV = AddressMode::Repeat;
        float maxAnisotropy = 1.0f;
        const char *debugName = nullptr;
    };

    // Сэмплер — отдельный объект (так в D3D11/Vulkan/Metal; в GL это sampler object).
    // Параметры фильтрации больше НЕ живут внутри текстуры.
    class ISampler : public IResource
    {
    public:
        virtual const SamplerDesc &GetDesc() const = 0;
    };
}
