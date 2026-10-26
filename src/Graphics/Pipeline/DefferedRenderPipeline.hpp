#pragma once
#include "IRenderPipeline.hpp"
#include "Device/RHI.hpp"
#include <memory>

#include "../Resources/Shader.hpp"
#include "../Pass/GeometryPass.hpp"
#include "../Pass/LightingPass.hpp"
#include "../Pass/PresentPass.hpp"

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