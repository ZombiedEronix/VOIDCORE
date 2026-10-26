#pragma once
#include "Graphics/Renderer.hpp"
#include "Assets/ResourceManager.h"
#include "Device/ICommandList.hpp"

namespace VoidCore::Rendering::Deferred
{
    class PresentPass
    {
    private:
        Shader *screenShader = nullptr;
        VoidCore::RHI::Ref<VoidCore::RHI::ISampler> presentSampler;
        VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> screenPipeline;
    public:
        void CreateScreenPipeline();
        PresentPass();
        ~PresentPass();
        void Execute(VoidCore::RHI::ICommandList *cmd, VoidCore::RHI::ITexture *texture);
    };
}