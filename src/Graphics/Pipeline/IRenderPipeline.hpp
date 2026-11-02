#pragma once
#include "Graphics/Renderer.hpp"
#include "Graphics/Pass/RenderPassEvent.hpp"
#include "Graphics/Pass/RenderPass.hpp"
namespace VoidCore::Rendering
{
    class IRenderPipeline
    {
    public:
        virtual ~IRenderPipeline() = default;
        virtual void AddPass(std::unique_ptr<RenderPass> pass, RenderPassEvent renderEvent) = 0;
        virtual void Execute(Renderer *renderer) = 0;
    };
}