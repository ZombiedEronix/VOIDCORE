#include "Graphics/Pass/UIPass.hpp"

#include <algorithm>
#include <cstddef>

#include "Assets/ResourceManager.h"
#include "Core/Debug.h"
#include "Graphics/Renderer.hpp"

namespace VoidCore::Rendering
{
    using namespace VoidCore::RHI;

    static constexpr const char *kDefaultFontPath = "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf";
    static constexpr float kDefaultFontSize = 20.0f;
    static constexpr const char *kShaderBase = "Assets/shaders/UI/Rectangle"; // .vert + .frag

    UIPass::UIPass()
    {
        defaultFont = std::make_unique<Font>(kDefaultFontPath, kDefaultFontSize);
        Font::SetDefault(defaultFont.get());

        SamplerDesc sd;
        sd.minFilter = Filter::Linear;
        sd.magFilter = Filter::Linear;
        sd.mipFilter = Filter::Nearest;
        sd.addressU = AddressMode::ClampToEdge;
        sd.addressV = AddressMode::ClampToEdge;
        sd.debugName = "UISampler";
        sampler = GetDevice().CreateSampler(sd);

        defaultShader = ResourceManager::GetShader(kShaderBase);
        if (!defaultShader || !defaultShader->IsValid())
        {
            Debug::LogError("UIPass: shader is invalid: ", kShaderBase);
        }

        defaultMaterial = std::make_unique<UIMaterial>();
        defaultMaterial->shader = defaultShader;

        EnsureBuffers(0, 0);
    }

    UIPass::~UIPass()
    {
        if (Font::GetDefault() == defaultFont.get())
            Font::SetDefault(nullptr);
    }

    ITexture *UIPass::GetAtlas(const Font *font)
    {
        auto it = atlases.find(font);
        if (it != atlases.end())
            return it->second.get();

        TextureDesc d;
        d.width = static_cast<uint32_t>(font->AtlasWidth());
        d.height = static_cast<uint32_t>(font->AtlasHeight());
        d.mipLevels = 1;
        d.format = Format::RGBA8_UNORM;
        d.usage = TextureUsage::Sampled;
        d.initialData = font->AtlasPixels();
        d.debugName = "UIFontAtlas";
        auto tex = GetDevice().CreateTexture(d);
        ITexture *raw = tex.get();
        atlases.emplace(font, std::move(tex));
        return raw;
    }

    UIMaterial *UIPass::GetFontMaterial(const Font *font)
    {
        auto it = fontMaterials.find(font);
        if (it != fontMaterials.end())
            return it->second.get();

        auto mat = std::make_unique<UIMaterial>();
        mat->shader = defaultShader;
        mat->SetTexture(0, GetAtlas(font), sampler.get());
        
        UIMaterial *raw = mat.get();
        fontMaterials.emplace(font, std::move(mat));
        return raw;
    }

    UIMaterial *UIPass::GetDefaultMaterial()
    {
        return defaultMaterial.get();
    }

    void UIPass::EnsureBuffers(size_t vertexCount, size_t indexCount)
    {
        if (!vertexBuffer || vertexCount > vertexCapacity)
        {
            vertexCapacity = std::max<size_t>(std::max<size_t>(vertexCount * 2, 4096), vertexCapacity);
            BufferDesc d;
            d.size = vertexCapacity * sizeof(UIVertex);
            d.usage = BufferUsage::Vertex;
            d.memory = MemoryType::Dynamic;
            d.debugName = "UIVertexBuffer";
            vertexBuffer = GetDevice().CreateBuffer(d);
        }
        if (!indexBuffer || indexCount > indexCapacity)
        {
            indexCapacity = std::max<size_t>(std::max<size_t>(indexCount * 2, 6144), indexCapacity);
            BufferDesc d;
            d.size = indexCapacity * sizeof(uint32_t);
            d.usage = BufferUsage::Index;
            d.memory = MemoryType::Dynamic;
            d.debugName = "UIIndexBuffer";
            indexBuffer = GetDevice().CreateBuffer(d);
        }
    }

    void UIPass::Execute(ICommandList *cmd, Renderer *renderer, const UIContext &ctx)
    {
        if (!cmd || ctx.indices.empty())
            return;

        EnsureBuffers(ctx.vertices.size(), ctx.indices.size());
        cmd->UpdateBuffer(vertexBuffer.get(), ctx.vertices.data(), ctx.vertices.size() * sizeof(UIVertex));
        cmd->UpdateBuffer(indexBuffer.get(), ctx.indices.data(), ctx.indices.size() * sizeof(uint32_t));

        RenderPassDesc pass;
        pass.debugName = "UI";
        pass.colorCount = 1;
        pass.colors[0].texture = nullptr;
        pass.colors[0].loadOp = LoadOp::Load;
        pass.hasDepth = false;

        cmd->BeginRenderPass(pass);

        cmd->SetUniformBuffer(0, renderer->GetCameraBuffer());
        cmd->SetVertexBuffer(0, vertexBuffer.get());
        cmd->SetIndexBuffer(indexBuffer.get(), IndexType::UInt32);

        for (const UIBatch &b : ctx.batches)
        {
            if (b.material && b.material->Apply(cmd, sampler.get()))
            {
                cmd->DrawIndexed(b.indexCount, 1, b.indexOffset, 0, 0);
            }
        }

        cmd->EndRenderPass();
    }
}
