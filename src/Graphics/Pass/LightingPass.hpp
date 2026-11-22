#pragma once
#include "Device/RHI.hpp"
#include "Graphics/Resources/Shader.hpp"
#include "Device/IBuffer.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Queue/LightCommand.hpp"
#include "RenderPass.hpp"
namespace VoidCore::Rendering::Deferred
{
    class LightingPass : public RenderPass
    {
    private:
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> litTexture;
        VoidCore::RHI::Ref<VoidCore::RHI::ISampler> gBufferSampler;
        VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> lightingPipeline;
        const Shader *lightingShader = nullptr;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer>
        lightBuffer;
        void CreateLightingPipeline();
        public:
        LightingPass();
        void OnResize(int width, int height) override;
        void Execute(VoidCore::RHI::ICommandList *cmd, Renderer *renderer);
    };
}
