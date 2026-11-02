#pragma once
#include "Graphics/Renderer.hpp"
#include "Assets/ResourceManager.h"
#include "Device/ICommandList.hpp"
#include "RenderPass.hpp"

namespace VoidCore::Rendering::Deferred
{
    class PresentPass : public RenderPass
    {
    private:
        Shader *shader = nullptr;
        void CreateScreenPipeline();
        VoidCore::RHI::Ref<VoidCore::RHI::ISampler> presentSampler;
        VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> screenPipeline;

    public:
        PresentPass();
        void Execute(VoidCore::RHI::ICommandList *cmd, Renderer *renderer) override;
    };
}