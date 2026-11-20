#pragma once
#include "Device/IResource.hpp"
#include "Device/RHITypes.hpp"

namespace VoidCore::RHI
{
    struct TextureDesc
    {
        uint32_t width = 1;
        uint32_t height = 1;
        uint32_t mipLevels = 1; // 0 = complete chain
        Format format = Format::RGBA8_UNORM;
        TextureUsage usage = TextureUsage::Sampled;
        // mip 0 data, densely packed (rowPitch = width * FormatSize).
        // If mipLevels != 1, the remaining levels are generated automatically.
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

    // A sampler is a separate object (as in D3D11/Vulkan/Metal; in GL it is a sampler object).
    // Filtering options no longer live inside the texture.
    class ISampler : public IResource
    {
    public:
        virtual const SamplerDesc &GetDesc() const = 0;
    };
}
