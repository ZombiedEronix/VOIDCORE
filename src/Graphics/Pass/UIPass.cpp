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

        CreatePipeline();
        EnsureBuffers(0, 0);
    }

    UIPass::~UIPass()
    {
        if (Font::GetDefault() == defaultFont.get())
            Font::SetDefault(nullptr);
    }

    void UIPass::CreatePipeline()
    {
        Shader *shader = ResourceManager::GetShader(kShaderBase);
        if (!shader || !shader->IsValid())
        {
            Debug::LogError("UIPass: shader is invalid: ", kShaderBase);
            return;
        }

        GraphicsPipelineDesc desc;
        desc.vertexShader = shader->GetVS();
        desc.fragmentShader = shader->GetFS();

        desc.vertexBuffers = {{sizeof(UIVertex), VertexInputRate::PerVertex}};
        desc.vertexAttributes = {
            {0, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, pos), 0},

            {1, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, uv), 0},

            {2, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, color), 0},

            {3, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, size), 0},

            {4, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, corners), 0},

            {5, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, bounds), 0},

            {6, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, bColor), 0}};

        desc.depth.testEnable = false;
        desc.depth.writeEnable = false;
        desc.rasterizer.cullMode = CullMode::None;

        desc.blend[0].enable = true;

        desc.colorTargetCount = 1;
        desc.colorFormats[0] = Format::RGBA8_UNORM;
        desc.depthFormat = Format::Unknown;

        desc.debugName = "UIPipeline";
        pipeline = GetDevice().CreatePipeline(desc);
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

    void UIPass::Execute(ICommandList *cmd, Renderer *renderer, const UIContext &ctx)
    {
        if (!cmd || !pipeline || ctx.indices.empty())
            return;

        EnsureBuffers(ctx.vertices.size(), ctx.indices.size());
        cmd->UpdateBuffer(vertexBuffer.get(), ctx.vertices.data(), ctx.vertices.size() * sizeof(UIVertex));
        cmd->UpdateBuffer(indexBuffer.get(), ctx.indices.data(), ctx.indices.size() * sizeof(uint32_t));
        for (const UIBatch &b : ctx.batches)
            GetAtlas(b.font);

        RenderPassDesc pass;
        pass.debugName = "UI";
        pass.colorCount = 1;
        pass.colors[0].texture = nullptr;
        pass.colors[0].loadOp = LoadOp::Load;
        pass.hasDepth = false;

        cmd->BeginRenderPass(pass);

        cmd->SetPipeline(pipeline.get());
        cmd->SetUniformBuffer(0, renderer->GetCameraBuffer());
        cmd->SetVertexBuffer(0, vertexBuffer.get());
        cmd->SetIndexBuffer(indexBuffer.get(), IndexType::UInt32);

        for (const UIBatch &b : ctx.batches)
        {
            cmd->SetTexture(0, GetAtlas(b.font), sampler.get());
            cmd->DrawIndexed(b.indexCount, 1, b.indexOffset, 0, 0);
        }

        cmd->EndRenderPass();
    }
}
