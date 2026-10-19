#include "DefferedRenderPipeline.hpp"
#include "Graphics/RenderPipeline/Renderer.h"
#include "Core/Debug.h"

namespace VoidCore::Rendering::Deferred
{
    using namespace VoidCore::RHI;
    DeferredRenderPipeline::DeferredRenderPipeline()
    {
        lightingPass = new LightingPass();
        geometryPass = new GeometryPass();
        presentPass = new PresentPass();
    }

    DeferredRenderPipeline::~DeferredRenderPipeline()
    {
        delete presentPass;
        delete geometryPass;
    }

    void DeferredRenderPipeline::Execute(Renderer *renderer)
    {
        auto cmd = renderer->GetCommandList();
        if (!cmd)
            return;
            geometryPass->Execute(renderer);
            lightingPass->Execute(cmd, renderer, geometryPass);
            presentPass->Execute(cmd, lightingPass->GetResult());
    }
}