#pragma once
#include <vector>
#include "Device/IResource.hpp"
#include "Device/RHITypes.hpp"

namespace VoidCore::RHI
{
    class IShader;

    struct VertexAttribute
    {
        uint32_t location = 0;   // layout(location = N) in the shader
        Format format = Format::Unknown;
        uint32_t offset = 0;     // offset inside the vertex, byte
        uint32_t bufferSlot = 0; // from which vertex buffer slot are we reading?
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

    // ALL state that was previously changed via glEnable/glBlendFunc/glDepthMask,
    // is now collected into one immutable object. This is how Vulkan (VkPipeline) works,
    // Metal (MTLRenderPipelineState) and D3D11 (a set of state objects).
    struct GraphicsPipelineDesc
    {
        IShader *vertexShader = nullptr;
        IShader *fragmentShader = nullptr;

        std::vector<VertexAttribute> vertexAttributes;
        std::vector<VertexBufferLayout> vertexBuffers; // index = bufferSlot

        PrimitiveTopology topology = PrimitiveTopology::TriangleList;
        RasterizerState rasterizer;
        DepthState depth;
        BlendState blend[kMaxColorTargets];

        // Render target formats - needed by Vulkan/Metal at the stage of creating the pipeline.
        uint32_t colorTargetCount = 1;
        Format colorFormats[kMaxColorTargets] = {Format::RGBA8_UNORM};
        Format depthFormat = Format::D24_UNORM_S8_UINT; // Unknown = no depth

        const char *debugName = nullptr;
    };

    class IPipeline : public IResource
    {
    };
}
