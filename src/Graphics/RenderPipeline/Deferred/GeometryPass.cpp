#include "GeometryPass.hpp"
#include "Graphics/RenderPipeline/GeometryRenderQueue.hpp"

namespace VoidCore::Rendering::Deferred
{
    using namespace VoidCore::RHI;
    GeometryPass::GeometryPass()
    {

        const uint32_t width = 1920;
        const uint32_t height = 1080;

        TextureDesc albedoDesc;
        albedoDesc.width = width;
        albedoDesc.height = height;
        albedoDesc.format = Format::RGBA8_UNORM;
        albedoDesc.usage = TextureUsage::RenderTarget | TextureUsage::Sampled;
        albedoDesc.debugName = "G_Albedo_Texture";
        gAlbedoTexture = GetDevice().CreateTexture(albedoDesc);

        TextureDesc depthDesc;
        depthDesc.width = width;
        depthDesc.height = height;
        depthDesc.format = Format::D24_UNORM_S8_UINT;
        depthDesc.usage = TextureUsage::DepthStencil | TextureUsage::Sampled;
        depthDesc.debugName = "G_Depth_Texture";
        gDepthTexture = GetDevice().CreateTexture(depthDesc);

        TextureDesc normalDesc;

        normalDesc.width = width;
        normalDesc.height = height;
        normalDesc.format = Format::RGBA8_UNORM;
        normalDesc.usage = TextureUsage::RenderTarget | TextureUsage::Sampled;
        normalDesc.debugName = "G_Normal_Texture";
        gNormalTexture = GetDevice().CreateTexture(normalDesc);
    }


    VoidCore::RHI::ITexture *GeometryPass::GetAlbedo()
    {
        return gAlbedoTexture.get();
    }

    VoidCore::RHI::ITexture *GeometryPass::GetDepth()
    {
        return gDepthTexture.get();
    }

    VoidCore::RHI::ITexture *GeometryPass::GetNormal()
    {
        return gNormalTexture.get();
    }

    void GeometryPass::Execute(Renderer *renderer)
    {
        auto cmd = renderer->GetCommandList();

        RenderPassDesc geomPass;
        geomPass.debugName = "G-Buffer Geometry Pass";
        geomPass.colorCount = 2;

        geomPass.colors[0].texture = gAlbedoTexture.get();
        geomPass.colors[0].loadOp = LoadOp::Clear;
        geomPass.colors[0].clearColor[0] = 0.1f;
        geomPass.colors[0].clearColor[1] = 0.1f;
        geomPass.colors[0].clearColor[2] = 0.1f;
        geomPass.colors[0].clearColor[3] = 1.0f;

        geomPass.colors[1].texture = gNormalTexture.get();
        geomPass.colors[1].loadOp = LoadOp::Clear;
        geomPass.colors[1].clearColor[0] = 0.1f;
        geomPass.colors[1].clearColor[1] = 0.1f;
        geomPass.colors[1].clearColor[2] = 0.1f;
        geomPass.colors[1].clearColor[3] = 1.0f;

        geomPass.hasDepth = true;
        geomPass.depth.texture = gDepthTexture.get();

        geomPass.depth.loadOp = LoadOp::Clear;
        geomPass.depth.clearDepth = 1.0f;

        cmd->BeginRenderPass(geomPass);

        cmd->SetUniformBuffer(0, renderer->GetCameraBuffer());
        cmd->SetVertexBuffer(1, renderer->GetInstanceBuffer());

        GeometryRenderQueue *queue = renderer->GetOpaqueQueue();

        queue->Sort();
        queue->Execute(renderer);

        cmd->EndRenderPass();
    }
};