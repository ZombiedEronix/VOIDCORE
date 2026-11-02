#pragma once
#include "IRenderPipeline.hpp"
#include "Device/RHI.hpp"
#include <memory>
#include <map>


namespace VoidCore::Rendering::Deferred
{
    class DeferredRenderPipeline : public IRenderPipeline
    {
    private:
        std::multimap<RenderPassEvent, std::unique_ptr<RenderPass>> m_RenderPasses;
        void CreateScreenPipeline();

    public:
        DeferredRenderPipeline();
        void OnResize(int width, int height);
        void AddPass(std::unique_ptr<RenderPass> pass, RenderPassEvent renderEvent) override;
        void Execute(Renderer *renderer) override;
    };
}