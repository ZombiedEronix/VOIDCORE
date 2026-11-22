#pragma once
#include <memory>
#include <unordered_map>

#include "Device/RHI.hpp"
#include "Assets/Font.hpp"
#include "UI/UIContext.hpp"
#include "RenderPass.hpp"

class Renderer;

namespace VoidCore::Rendering
{
    class UIPass : public RenderPass
    {
    public:
        UIPass();
        void Execute(VoidCore::RHI::ICommandList *cmd, Renderer *renderer);
        void OnResize(int width, int height) override;

    private:
        void EnsureBuffers(size_t vertexCount, size_t indexCount);
        
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> uiTexture;

        VoidCore::RHI::Ref<VoidCore::RHI::ISampler> sampler;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> vertexBuffer;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> indexBuffer;
        size_t vertexCapacity = 0;
        size_t indexCapacity = 0;
    };
}
