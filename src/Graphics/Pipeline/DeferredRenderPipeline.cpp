#include "DefferedRenderPipeline.hpp"
#include "Core/Debug.h"

#include "../Pass/RenderPass.hpp"
#include "../Pass/RenderPassEvent.hpp"

#include "../Pass/GeometryPass.hpp"
#include "../Pass/LightingPass.hpp"
#include "../Pass/PostProcessPass.hpp"
#include "../Pass/PresentPass.hpp"
#include "../Pass/UIPass.hpp"

namespace VoidCore::Rendering::Deferred
{
    using namespace VoidCore::RHI;

    DeferredRenderPipeline::DeferredRenderPipeline()
    {
        AddPass(std::make_unique<GeometryPass>(), RenderPassEvent::BeforeGeometry);
        AddPass(std::make_unique<LightingPass>(), RenderPassEvent::AfterGeometry);
        AddPass(std::make_unique<PostProcessPass>(), RenderPassEvent::BeforeUI);
        AddPass(std::make_unique<UIPass>(), RenderPassEvent::BeforeUI);
        AddPass(std::make_unique<PresentPass>(), RenderPassEvent::AfterUI);
    }

    void DeferredRenderPipeline::OnResize(int width, int height)
    {
    }

    void DeferredRenderPipeline::AddPass(std::unique_ptr<RenderPass> pass, RenderPassEvent renderEvent)
    {
        m_RenderPasses.insert({renderEvent, std::move(pass)});
    }

    void DeferredRenderPipeline::Execute(Renderer *renderer)
    {
        auto cmd = renderer->GetCommandList();
        if (!cmd)
            return;

        for (auto &[renderEvent, pass] : m_RenderPasses)
        {
            if (pass)
            {
                pass->Execute(cmd, renderer);
            }
        }
    }
}