#include "PresentPass.hpp"
#include "Device/RHI.hpp"

namespace VoidCore::Rendering::Deferred
{
    using namespace VoidCore::RHI;

    PresentPass::PresentPass()
    {
        executionEvent = RenderPassEvent::BeforeUI;
        SamplerDesc sDesc;
        sDesc.minFilter = Filter::Nearest;
        sDesc.magFilter = Filter::Nearest;
        sDesc.addressU = AddressMode::ClampToEdge;
        sDesc.addressV = AddressMode::ClampToEdge;
        presentSampler = GetDevice().CreateSampler(sDesc);

        shader = ResourceManager::GetShader("Assets/shaders/FullScreen/screen");
        CreateScreenPipeline();
    }

    void PresentPass::CreateScreenPipeline()
    {
        if (!shader || !shader->IsValid())
        {
            Debug::LogError("DeferredRenderPipeline: screenShader is invalid!");
            return;
        }

        GraphicsPipelineDesc desc;
        desc.vertexShader = shader->GetVS();
        desc.fragmentShader = shader->GetFS();

        desc.vertexAttributes.clear();
        desc.vertexBuffers.clear();

        desc.depth.testEnable = false;
        desc.depth.writeEnable = false;

        desc.rasterizer.cullMode = CullMode::None;

        desc.colorTargetCount = 1;
        desc.colorFormats[0] = Format::RGBA8_UNORM;
        desc.depthFormat = Format::Unknown;

        desc.debugName = "ScreenQuadPipeline";
        screenPipeline = GetDevice().CreatePipeline(desc);
    }

    void PresentPass::Execute(ICommandList *cmd, Renderer *renderer)
    {

        RenderPassDesc presentPass;
        presentPass.debugName = "Present to Screen";
        presentPass.colorCount = 1;
        presentPass.colors[0].texture = nullptr;
        presentPass.colors[0].loadOp = LoadOp::Clear;
        presentPass.colors[0].clearColor[0] = 0.0f;
        presentPass.colors[0].clearColor[1] = 0.0f;
        presentPass.colors[0].clearColor[2] = 0.0f;
        presentPass.colors[0].clearColor[3] = 1.0f;

        presentPass.hasDepth = false;

        cmd->BeginRenderPass(presentPass);

        if (screenPipeline)
        {
            cmd->SetPipeline(screenPipeline.get());
            cmd->SetTexture(0, renderer->GetRenderTargetTexture("GBuffer_RenderResult"), presentSampler.get());
            cmd->SetTexture(1, renderer->GetRenderTargetTexture("GBuffer_UI"), presentSampler.get());
            cmd->Draw(3, 1, 0, 0);
        }

        cmd->EndRenderPass();
    }
}