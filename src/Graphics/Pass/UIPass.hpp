#pragma once
#include <memory>
#include <unordered_map>

#include "Device/RHI.hpp"
#include "UI/Structures/Font.hpp"
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

    private:
        void EnsureBuffers(size_t vertexCount, size_t indexCount);
        void CreateTexture();
        VoidCore::RHI::Ref<VoidCore::RHI::ITexture> uiTexture;

        VoidCore::RHI::Ref<VoidCore::RHI::ISampler> sampler;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> vertexBuffer;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> indexBuffer;
        size_t vertexCapacity = 0;
        size_t indexCapacity = 0;
    };
}
