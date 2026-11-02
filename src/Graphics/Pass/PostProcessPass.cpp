#include "PostProcessPass.hpp"

namespace VoidCore::Rendering::Deferred
{

    void PostProcessPass::Execute(VoidCore::RHI::ICommandList *cmd, Renderer *renderer)
    {
        auto *inputTexture = renderer->GetRenderTargetTexture("GBuffer_Lit");
        if (!inputTexture || m_Effects.empty())
        {
            renderer->SetRenderTargetTexture("GBuffer_RenderResult", inputTexture);
            return;
        }

        RHI::ITexture *currentSource = inputTexture;

        m_CurrentTargetIdx = 0;

        for (size_t i = 0; i < m_Effects.size(); ++i)
        {
            RHI::ITexture *currentDestination = m_PingPongRT[m_CurrentTargetIdx].get();

            m_Effects[i]->Apply(cmd, currentSource, currentDestination);

            currentSource = currentDestination;

            m_CurrentTargetIdx = 1 - m_CurrentTargetIdx;
        }

        renderer->SetRenderTargetTexture("GBuffer_RenderResult", currentSource);
    }
}