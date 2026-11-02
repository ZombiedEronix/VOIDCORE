#pragma once
#include "Graphics/Renderer.hpp"

#include "Device/RHI.hpp"
#include "RenderPass.hpp"

namespace VoidCore::Rendering::Deferred
{
    class GeometryPass : public RenderPass
    {
        private:
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> gAlbedoTexture;
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> gDepthTexture;
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> gNormalTexture;
        void CreateBuffers(int width, int height);

        public:
        GeometryPass();

        void Execute(VoidCore::RHI::ICommandList *cmd, Renderer *renderer) override;
    };
}