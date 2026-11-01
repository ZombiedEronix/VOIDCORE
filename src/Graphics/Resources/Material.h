#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Device/RHI.hpp"
#include "Shader.hpp"
#include "VertexLayouts.hpp"
#include "Assets/Texture.h"
#include "UI/Structures/UIVertex.hpp"
#include "Core/Debug.h"

enum class RenderQueue
{
    Opaque,
    Masked,
    Transparent
};

enum class MaterialType
{
    Mesh3D,
    UI2D
};

class Material
{
public:
    Shader *shader = nullptr;
    RenderQueue queueType = RenderQueue::Opaque;
    MaterialType materialType = MaterialType::Mesh3D;

    void SetTexture(uint32_t slot, Texture *tex)
    {
        if (slot < VoidCore::RHI::kMaxTextureSlots)
        {
            assetTextures[slot] = tex;
            rhiTextures[slot] = tex ? tex->GetRHI() : nullptr;
            rhiSamplers[slot] = tex ? tex->GetSampler() : nullptr;
        }
    }

    void SetTexture(uint32_t slot, VoidCore::RHI::ITexture *tex, VoidCore::RHI::ISampler *sampler = nullptr)
    {
        if (slot < VoidCore::RHI::kMaxTextureSlots)
        {
            assetTextures[slot] = nullptr;
            rhiTextures[slot] = tex;
            rhiSamplers[slot] = sampler;
        }
    }

    void SetFloat(const std::string &name, float value) { floatParams[name] = value; }
    void SetVector(const std::string &name, const glm::vec4 &value) { vectorParams[name] = value; }
    void SetColor(const std::string &name, const Color &color) { vectorParams[name] = glm::vec4(color.r, color.g, color.b, color.a); }

    float GetFloat(const std::string &name) const
    {
        auto it = floatParams.find(name);
        return it != floatParams.end() ? it->second : 0.0f;
    }

    glm::vec4 GetVector(const std::string &name) const
    {
        auto it = vectorParams.find(name);
        return it != vectorParams.end() ? it->second : glm::vec4(0.0f);
    }

    bool Apply(VoidCore::RHI::ICommandList *cmd, VoidCore::RHI::ISampler *defaultSampler = nullptr) const
    {
        if (!pipeline && !buildFailed)
            Build();
        if (!pipeline)
            return false;

        cmd->SetPipeline(pipeline.get());
        for (uint32_t slot = 0; slot < VoidCore::RHI::kMaxTextureSlots; ++slot)
        {
            if (rhiTextures[slot])
            {
                auto sampler = rhiSamplers[slot] ? rhiSamplers[slot] : defaultSampler;
                cmd->SetTexture(slot, rhiTextures[slot], sampler);
            }
            else if (assetTextures[slot])
            {
                cmd->SetTexture(slot, assetTextures[slot]->GetRHI(), assetTextures[slot]->GetSampler());
            }
        }
        return true;
    }

    VoidCore::RHI::IPipeline *GetPipeline() const
    {
        if (!pipeline && !buildFailed)
            Build();
        return pipeline.get();
    }

private:
    void Build() const
    {
        using namespace VoidCore::RHI;
        if (!shader || !shader->IsValid())
        {
            Debug::LogError("Material: shader is missing or failed to compile");
            buildFailed = true;
            return;
        }

        GraphicsPipelineDesc d;
        d.vertexShader = shader->GetVS();
        d.fragmentShader = shader->GetFS();

        if (materialType == MaterialType::UI2D)
        {
            d.vertexBuffers = {{sizeof(UIVertex), VertexInputRate::PerVertex}};
            d.vertexAttributes = {
                {0, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, pos), 0},
                {1, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, uv), 0},
                {2, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, rect), 0},
                {3, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, color), 0},
                {4, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, cornerSize), 0},
                {5, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, borderColor), 0},
                {6, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, borderThickness), 0},
            };

            d.depth.testEnable = false;
            d.depth.writeEnable = false;
            d.rasterizer.cullMode = CullMode::None;
            d.blend[0].enable = true;

            d.colorTargetCount = 1;
            d.colorFormats[0] = Format::RGBA8_UNORM;
            d.depthFormat = Format::Unknown;
            d.debugName = "UIMaterialPipeline";
        }
        else
#ifndef BUILD_WITHOUT_3D
        {
            SetupMeshInputLayout(d);

            d.colorTargetCount = 1;
            d.colorFormats[0] = Format::RGBA8_UNORM;
            d.depthFormat = Format::D24_UNORM_S8_UINT;

            if (queueType == RenderQueue::Transparent)
            {
                d.blend[0].enable = true;
                d.depth.writeEnable = false;
            }
            d.depth.testEnable = true;
            d.depth.writeEnable = true;
            d.depth.compareOp = CompareOp::Less;
            d.debugName = "MeshMaterialPipeline";
        }
#else
        {
            //NOTHING
        }
#endif

        pipeline = GetDevice().CreatePipeline(d);
        buildFailed = (pipeline == nullptr);
        if (buildFailed)
            Debug::LogError("FAILED TO BUILD PIPELINE!");
    }

    Texture *assetTextures[VoidCore::RHI::kMaxTextureSlots] = {};
    VoidCore::RHI::ITexture *rhiTextures[VoidCore::RHI::kMaxTextureSlots] = {};
    VoidCore::RHI::ISampler *rhiSamplers[VoidCore::RHI::kMaxTextureSlots] = {};

    std::unordered_map<std::string, float> floatParams;
    std::unordered_map<std::string, glm::vec4> vectorParams;

    mutable VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> pipeline;
    mutable bool buildFailed = false;
};