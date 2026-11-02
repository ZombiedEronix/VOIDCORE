#include "Graphics/Pass/UIPass.hpp"

#include <algorithm>
#include <cstddef>

#include "Assets/ResourceManager.h"
#include "Core/Debug.h"
#include "Graphics/Renderer.hpp"

namespace VoidCore::Rendering
{
    using namespace VoidCore::RHI;

    UIPass::UIPass()
    {

        SamplerDesc sd;
        sd.minFilter = Filter::Linear;
        sd.magFilter = Filter::Linear;
        sd.mipFilter = Filter::Nearest;
        sd.addressU = AddressMode::ClampToEdge;
        sd.addressV = AddressMode::ClampToEdge;
        sd.debugName = "UISampler";
        sampler = GetDevice().CreateSampler(sd);
        CreateTexture();
        EnsureBuffers(0, 0);
    }

    void UIPass::CreateTexture()
    {
        TextureDesc uiDesc;
        uiDesc.width = Window::GetWidth();
        uiDesc.height = Window::GetHeight();
        uiDesc.format = Format::RGBA8_UNORM;
        uiDesc.usage = TextureUsage::RenderTarget | TextureUsage::Sampled;
        uiDesc.debugName = "GBuffer_UI";
        uiTexture = GetDevice().CreateTexture(uiDesc);
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
        pass.colors[0].texture = uiTexture.get();
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

        renderer->SetRenderTargetTexture("GBuffer_UI", uiTexture.get());
        cmd->EndRenderPass();
    }
}
