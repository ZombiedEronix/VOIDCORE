#pragma once
#include <memory>
#include <unordered_map>

#include "Device/RHI.hpp"
#include "UI/Structures/Font.hpp"
#include "UI/UIContext.hpp"

class Renderer;

namespace VoidCore::Rendering
{
    // Рисует UI поверх готового кадра (после PresentPass), прямо в back buffer.
    //
    // За кадр:  UpdateBuffer(vertices) + UpdateBuffer(indices) -> один render pass
    //           (LoadOp::Load, без depth, alpha blend) -> по DrawIndexed на батч.
    // Обычно батч один (фоны и текст лежат в одном атласе), т.е. весь UI = 1 draw call.
    class UIPass
    {
    public:
        UIPass();
        ~UIPass();

        void Execute(VoidCore::RHI::ICommandList *cmd, Renderer *renderer, const UIContext &ctx);

    private:
        void CreatePipeline();
        void EnsureBuffers(size_t vertexCount, size_t indexCount);
        VoidCore::RHI::ITexture *GetAtlas(const Font *font);

        VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> pipeline;
        VoidCore::RHI::Ref<VoidCore::RHI::ISampler> sampler;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> vertexBuffer;
        VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> indexBuffer;
        size_t vertexCapacity = 0; // в вершинах
        size_t indexCapacity = 0;  // в индексах

        // GPU-копии атласов шрифтов (создаются при первом использовании).
        std::unordered_map<const Font *, VoidCore::RHI::Ref<VoidCore::RHI::ITexture>> atlases;

        std::unique_ptr<Font> defaultFont;
    };
}
