#include "Graphics/Queue/GeometryRenderQueue.hpp"
#include "GeometryPass.hpp"

namespace VoidCore::Rendering::Deferred
{
    using namespace VoidCore::RHI;

    void GeometryPass::CreateBuffers(int width, int height)
    {

        TextureDesc albedoDesc;
        albedoDesc.width = width;
        albedoDesc.height = height;
        albedoDesc.format = Format::RGBA8_UNORM;
        albedoDesc.usage = TextureUsage::RenderTarget | TextureUsage::Sampled;
        albedoDesc.debugName = "GBuffer_Albedo";
        gAlbedoTexture = GetDevice().CreateTexture(albedoDesc);

        TextureDesc depthDesc;
        depthDesc.width = width;
        depthDesc.height = height;
        depthDesc.format = Format::D24_UNORM_S8_UINT;
        depthDesc.usage = TextureUsage::DepthStencil | TextureUsage::Sampled;
        depthDesc.debugName = "GBuffer_Depth";
        gDepthTexture = GetDevice().CreateTexture(depthDesc);

        TextureDesc normalDesc;

        normalDesc.width = width;
        normalDesc.height = height;
        normalDesc.format = Format::RGBA8_UNORM;
        normalDesc.usage = TextureUsage::RenderTarget | TextureUsage::Sampled;
        normalDesc.debugName = "GBuffer_Normal";
        gNormalTexture = GetDevice().CreateTexture(normalDesc);
    }

    GeometryPass::GeometryPass()
    {
        CreateBuffers(Window::GetWidth(), Window::GetHeight());
    }

    void GeometryPass::Execute(ICommandList *cmd, Renderer *renderer)
    {

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
        geomPass.colors[1].loadOp = LoadOp::DontCare;
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

        renderer->SetRenderTargetTexture("GBuffer_Albedo", gAlbedoTexture.get());
        renderer->SetRenderTargetTexture("GBuffer_Normal", gNormalTexture.get());
        renderer->SetRenderTargetTexture("GBuffer_Depth", gDepthTexture.get());
    }
};