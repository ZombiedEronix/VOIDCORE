#pragma once
#include <memory>
#include <unordered_map>

#include "Device/RHI.hpp"
#include "UI/Structures/Font.hpp"
#include "UI/UIContext.hpp"
#include "UI/UIMaterial.hpp"

class Renderer;

namespace VoidCore::Rendering
{
    class UIPass
    {
    public:
        UIPass();
        ~UIPass();

        void Execute(VoidCore::RHI::ICommandList *cmd, Renderer *renderer, const UIContext &ctx);

        UIMaterial *GetFontMaterial(const Font *font);
        UIMaterial *GetDefaultMaterial();
        Font *GetDefaultFont() const { return defaultFont.get(); }

    private:
        void EnsureBuffers(size_t vertexCount, size_t indexCount);
        VoidCore::RHI::ITexture *GetAtlas(const Font *font);

        VoidCore::RHI::Ref<VoidCore::RHI::ISampler> sampler;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> vertexBuffer;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> indexBuffer;
        size_t vertexCapacity = 0;
        size_t indexCapacity = 0;

        std::unordered_map<const Font *, VoidCore::RHI::Ref<VoidCore::RHI::ITexture>> atlases;
        std::unordered_map<const Font *, std::unique_ptr<UIMaterial>> fontMaterials;
        std::unique_ptr<UIMaterial> defaultMaterial;
        std::unique_ptr<Font> defaultFont;
        Shader *defaultShader = nullptr;
    };
}
