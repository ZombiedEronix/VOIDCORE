#pragma once
#include <vector>
#include "Device/IResource.hpp"
#include "Device/RHITypes.hpp"

namespace VoidCore::RHI
{
    class IShader;

    struct VertexAttribute
    {
        uint32_t location = 0;   // layout(location = N) в шейдере
        Format format = Format::Unknown;
        uint32_t offset = 0;     // смещение внутри вершины, байт
        uint32_t bufferSlot = 0; // из какого vertex buffer слота читаем
    };

    struct VertexBufferLayout
    {
        uint32_t stride = 0;
        VertexInputRate inputRate = VertexInputRate::PerVertex;
    };

    struct RasterizerState
    {
        CullMode cullMode = CullMode::Back;
        FrontFace frontFace = FrontFace::CounterClockwise;
    };

    struct DepthState
    {
        bool testEnable = true;
        bool writeEnable = true;
        CompareOp compareOp = CompareOp::Less;
    };

    struct BlendState
    {
        bool enable = false;
        BlendFactor srcColor = BlendFactor::SrcAlpha;
        BlendFactor dstColor = BlendFactor::OneMinusSrcAlpha;
        BlendOp colorOp = BlendOp::Add;
        BlendFactor srcAlpha = BlendFactor::One;
        BlendFactor dstAlpha = BlendFactor::OneMinusSrcAlpha;
        BlendOp alphaOp = BlendOp::Add;
        ColorWriteMask writeMask = ColorWriteMask::All;
    };

    // ВСЁ состояние, которое раньше менялось через glEnable/glBlendFunc/glDepthMask,
    // теперь собрано в один неизменяемый объект. Так устроены Vulkan (VkPipeline),
    // Metal (MTLRenderPipelineState) и D3D11 (набор state-объектов).
    struct GraphicsPipelineDesc
    {
        IShader *vertexShader = nullptr;
        IShader *fragmentShader = nullptr;

        std::vector<VertexAttribute> vertexAttributes;
        std::vector<VertexBufferLayout> vertexBuffers; // индекс = bufferSlot

        PrimitiveTopology topology = PrimitiveTopology::TriangleList;
        RasterizerState rasterizer;
        DepthState depth;
        BlendState blend[kMaxColorTargets];

        // Форматы целей рендера — нужны Vulkan/Metal на этапе создания пайплайна.
        uint32_t colorTargetCount = 1;
        Format colorFormats[kMaxColorTargets] = {Format::RGBA8_UNORM};
        Format depthFormat = Format::D24_UNORM_S8_UINT; // Unknown = без глубины

        const char *debugName = nullptr;
    };

    class IPipeline : public IResource
    {
    };
}
