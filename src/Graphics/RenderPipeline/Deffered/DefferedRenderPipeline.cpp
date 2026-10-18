#include "DefferedRenderPipeline.hpp"
#include "Graphics/RenderPipeline/Renderer.h"
#include "Graphics/RenderPipeline/OpaquePass.h"
#include <vector>

using namespace VoidCore::RHI;

DeferredRenderPipeline::DeferredRenderPipeline()
{
    opaquePass = new OpaquePass();

    /// Создание буфера ALBEDO
    VoidCore::RHI::TextureDesc gBufferDesc;
    gBufferDesc.width = 1920;
    gBufferDesc.height = 1080;
    gBufferDesc.format = Format::RGBA8_SRGB;
    gBufferDesc.usage = TextureUsage::RenderTarget | TextureUsage::Sampled;
    gBufferDesc.debugName = "G_Albedo_Texture";
    
    gAlbedoTexture = GetDevice().CreateTexture(gBufferDesc);
    /// Создание буфера ALBEDO
    
}



DeferredRenderPipeline::~DeferredRenderPipeline()
{
    delete opaquePass;
}

void DeferredRenderPipeline::Execute(Renderer *renderer)
{
    auto cmd = renderer->GetCommandList();
    if (!cmd)
        return;

    RenderPassDesc pass;
    pass.debugName = "Geometry";
    pass.colorCount = 1;

    pass.colors[0].loadOp = LoadOp::Clear;

    float clearColor[4] = {0.35f, 0.35f, 0.35f, 1.0f};
    for (int i = 0; i < 4; ++i)
        pass.colors[0].clearColor[i] = clearColor[i];

    pass.hasDepth = true;
    pass.depth.loadOp = LoadOp::Clear;
    pass.depth.clearDepth = 1.0f;

    cmd->BeginRenderPass(pass);
    cmd->SetUniformBuffer(0, renderer->GetCameraBuffer());

    cmd->SetVertexBuffer(1, renderer->GetInstanceBuffer());

    std::vector<GeometryRenderCommand>& queue = renderer->GetOpaqueQueue();
    opaquePass->Sort(queue);
    opaquePass->Execute(renderer, queue);

    cmd->EndRenderPass();
}