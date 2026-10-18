#pragma once
#include "Graphics/RenderPipeline/IRenderPipeline.hpp"
#include "Device/RHI.hpp"
#include <memory>

class OpaquePass;
class LightingPass;
class PostProcessPass;
class UIPass;

class DeferredRenderPipeline : public IRenderPipeline
{
private:
    VoidCore::RHI::Ref<VoidCore::RHI::ITexture> gAlbedoTexture;
    size_t currentBufferCapacity = 1024;

    OpaquePass *opaquePass;
public:
    DeferredRenderPipeline();
    ~DeferredRenderPipeline() override;

    void Execute(Renderer *renderer) override;
};