#pragma once
#include "Graphics/Renderer.hpp"

#include "Device/RHI.hpp"

namespace VoidCore::Rendering::Deferred
{
    class GeometryPass
    {
        private:
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> gAlbedoTexture;
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> gDepthTexture;
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> gNormalTexture;
        void CreateBuffers(int width, int height);

        public:
        GeometryPass();

        VoidCore::RHI::ITexture* GetAlbedo();
        VoidCore::RHI::ITexture* GetDepth();
        VoidCore::RHI::ITexture* GetNormal();
        

        void Execute(Renderer *commandList);
    };
}