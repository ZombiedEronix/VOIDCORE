#pragma once
#include "Graphics/RenderPipeline/IRenderPipeline.hpp"
#include "Device/RHI.hpp"
#include <memory>
#include "Graphics/Shader.h"
#include "PresentPass.hpp"
#include "LightingPass.hpp"
#include "GeometryPass.hpp"

namespace VoidCore::Rendering::Deferred
{
    class DeferredRenderPipeline : public IRenderPipeline
    {
    private:
        GeometryPass *geometryPass;
        LightingPass *lightingPass;
        PresentPass *presentPass;

        void CreateScreenPipeline();

    public:
        DeferredRenderPipeline();
        ~DeferredRenderPipeline() override;
        void OnResize(int width, int height);
        void Execute(Renderer *renderer) override;
    };
}