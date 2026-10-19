#include "LightingPass.hpp"
#include "GeometryPass.hpp"
#include "Assets/ResourceManager.h"

namespace VoidCore::Rendering::Deferred
{
    using namespace VoidCore::RHI;

    LightingPass::LightingPass()
    {
        const uint32_t width = 1920;
        const uint32_t height = 1080;

        lightBlock = new LightsBlock();

        BufferDesc cb;
        cb.size = sizeof(LightsBlock);
        cb.usage = BufferUsage::Uniform;
        cb.memory = MemoryType::Dynamic;
        cb.debugName = "FrameUBO";
        lightBuffer = GetDevice().CreateBuffer(cb);

        TextureDesc litDesc;
        litDesc.width = width;
        litDesc.height = height;
        litDesc.format = Format::RGBA8_UNORM;
        litDesc.usage = TextureUsage::RenderTarget | TextureUsage::Sampled;
        litDesc.debugName = "G_Lit_Output_Texture";
        litTexture = GetDevice().CreateTexture(litDesc);

        SamplerDesc sDesc;
        sDesc.minFilter = Filter::Nearest;
        sDesc.magFilter = Filter::Nearest;
        sDesc.addressU = AddressMode::ClampToEdge;
        sDesc.addressV = AddressMode::ClampToEdge;
        gBufferSampler = GetDevice().CreateSampler(sDesc);

        lightingShader = ResourceManager::GetShader("Assets/shaders/Deferred/lighting");
        CreateLightingPipeline();
    }

    void LightingPass::CreateLightingPipeline()
    {
        GraphicsPipelineDesc desc;
        desc.vertexShader = ResourceManager::GetShader("Assets/shaders/FullScreen/screen")->GetVS(); // Юзаем старый VS экрана
        desc.fragmentShader = lightingShader->GetFS();

        desc.vertexAttributes.clear();
        desc.vertexBuffers.clear();
        desc.depth.testEnable = false;
        desc.depth.writeEnable = false;
        desc.rasterizer.cullMode = CullMode::None;

        desc.colorTargetCount = 1;
        desc.colorFormats[0] = Format::RGBA8_UNORM;
        desc.depthFormat = Format::Unknown;

        desc.debugName = "DeferredLightingPipeline";
        lightingPipeline = GetDevice().CreatePipeline(desc);
    }

    LightingPass::~LightingPass() { delete lightingShader; }

    void LightingPass::Execute(ICommandList *cmd, Renderer *renderer, GeometryPass *gBuffer)
    {
        RenderPassDesc passDesc;
        passDesc.debugName = "Deferred Lighting Pass";
        passDesc.colorCount = 1;
        passDesc.colors[0].texture = litTexture.get();
        passDesc.colors[0].loadOp = LoadOp::Clear;
        passDesc.colors[0].clearColor[0] = 0.0f;
        passDesc.colors[0].clearColor[1] = 0.0f;
        passDesc.colors[0].clearColor[2] = 0.0f;
        passDesc.colors[0].clearColor[3] = 1.0f;
        passDesc.hasDepth = false;

        
        
        cmd->SetUniformBuffer(1, lightBuffer.get());

        lightBlock->lights[0].Rotation = glm::vec4(-80,0,0,1);
        lightBlock->currentLights = 1;

        cmd->UpdateBuffer(lightBuffer.get(), lightBlock, sizeof(LightsBlock));

        cmd->BeginRenderPass(passDesc);

        if (lightingPipeline)
        {
            cmd->SetPipeline(lightingPipeline.get());

            cmd->SetUniformBuffer(0, renderer->GetCameraBuffer());
            cmd->SetTexture(0, gBuffer->GetAlbedo(), gBufferSampler.get());
            cmd->SetTexture(1, gBuffer->GetNormal(), gBufferSampler.get());
            cmd->SetTexture(2, gBuffer->GetDepth(), gBufferSampler.get());

            cmd->Draw(3, 1, 0, 0);
        }

        cmd->EndRenderPass();
    }
}
