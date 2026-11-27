#pragma once

#include "RenderPass.hpp"

namespace VoidCore::RHI
{
    class ICommandList;
}

namespace VoidCore::Rendering
{
    class Renderer;
}

namespace VoidCore::Rendering::Deferred
{
    class GeometryPass : public RenderPass
    {
        private:
        VoidCore::RHI::VRef<VoidCore::RHI::ITexture> gAlbedoTexture;
        VoidCore::RHI::VRef<VoidCore::RHI::ITexture> gDepthTexture;
        VoidCore::RHI::VRef<VoidCore::RHI::ITexture> gNormalTexture;
        
        public:
        GeometryPass();
        
        void OnResize(int width, int height) override;
        void Execute(RHI::ICommandList *cmd, Rendering::Renderer *renderer) override;
    };
}