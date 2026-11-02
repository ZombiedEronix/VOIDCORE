#pragma once
#include "RenderPass.hpp"
#include "PostProcessEffect.hpp"
#include "Device/ITexture.hpp"
#include "Device/ICommandList.hpp"


#include <vector>
#include <memory>

#include "Graphics/Renderer.hpp"

namespace VoidCore::Rendering::Deferred
{
    class PostProcessPass : public RenderPass
    {
    private:
        std::vector<std::unique_ptr<PostProcessEffect>> m_Effects;

        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> m_PingPongRT[2];
        int m_CurrentTargetIdx = 0;

        void CreateBuffers(int width, int height);

    public:
        PostProcessPass();

        void AddEffect(std::unique_ptr<PostProcessEffect> effect);
        void OnResize(int width, int height) override;
        void Execute(VoidCore::RHI::ICommandList *cmd, Renderer *renderer) override;
    };
}
