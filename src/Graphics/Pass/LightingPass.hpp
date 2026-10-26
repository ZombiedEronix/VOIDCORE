#pragma once
#include "Device/RHI.hpp"
#include "Graphics/Resources/Shader.hpp"
#include "Device/IBuffer.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Queue/LightCommand.hpp"

namespace VoidCore::Rendering::Deferred
{
    class LightingPass
    {
    private:
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> litTexture;
        VoidCore::RHI::Ref<VoidCore::RHI::ISampler> gBufferSampler;
        VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> lightingPipeline;
        Shader *lightingShader = nullptr;
        void CreateBuffers(int width, int height);
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer>
            lightBuffer;

        void CreateLightingPipeline();

    public:
        LightingPass();
        ~LightingPass();

        VoidCore::RHI::ITexture *GetResult() { return litTexture.get(); }
        void Execute(VoidCore::RHI::ICommandList *cmd, class Renderer *renderer, class GeometryPass *gBuffer);
    };
}
