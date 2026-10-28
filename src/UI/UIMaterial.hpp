#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Device/RHI.hpp"
#include "Graphics/Resources/Shader.hpp"
#include "UI/Structures/UIVertex.hpp"
#include "Assets/Texture.h"
#include "Core/Debug.h"

class UIMaterial
{
public:
    Shader *shader = nullptr;

    void SetTexture(uint32_t slot, VoidCore::RHI::ITexture *tex, VoidCore::RHI::ISampler *sampler = nullptr)
    {
        if (slot < VoidCore::RHI::kMaxTextureSlots)
        {
            textures[slot] = tex;
            samplers[slot] = sampler;
        }
    }

    void SetTexture(uint32_t slot, Texture *tex)
    {
        if (slot < VoidCore::RHI::kMaxTextureSlots)
        {
            textures[slot] = tex ? tex->GetRHI() : nullptr;
            samplers[slot] = tex ? tex->GetSampler() : nullptr;
        }
    }

    void SetFloat(const std::string &name, float value) { floatParams[name] = value; }
    void SetVector(const std::string &name, const glm::vec4 &value) { vectorParams[name] = value; }
    void SetColor(const std::string &name, const Color &color) { vectorParams[name] = glm::vec4(color.r, color.g, color.b, color.a); }

    float GetFloat(const std::string &name) const {
        auto it = floatParams.find(name);
        return it != floatParams.end() ? it->second : 0.0f;
    }

    glm::vec4 GetVector(const std::string &name) const {
        auto it = vectorParams.find(name);
        return it != vectorParams.end() ? it->second : glm::vec4(0.0f);
    }

    bool Apply(VoidCore::RHI::ICommandList *cmd, VoidCore::RHI::ISampler *defaultSampler) const
    {
        const_cast<UIMaterial*>(this)->EnsureBuilt();
        if (!pipeline)
            return false;

        cmd->SetPipeline(pipeline.get());
        for (uint32_t slot = 0; slot < VoidCore::RHI::kMaxTextureSlots; ++slot)
        {
            if (textures[slot])
            {
                cmd->SetTexture(slot, textures[slot], samplers[slot] ? samplers[slot] : defaultSampler);
            }
        }
        return true;
    }

    VoidCore::RHI::IPipeline *GetPipeline()
    {
        EnsureBuilt();
        return pipeline.get();
    }

private:
    void EnsureBuilt()
    {
        if (!pipeline && !buildFailed)
            Build();
    }

    void Build()
    {
        using namespace VoidCore::RHI;
        if (!shader || !shader->IsValid())
        {
            buildFailed = true;
            return;
        }

        GraphicsPipelineDesc d;
        d.vertexShader = shader->GetVS();
        d.fragmentShader = shader->GetFS();

        d.vertexBuffers = {{sizeof(UIVertex), VertexInputRate::PerVertex}};
        d.vertexAttributes = {
            {0, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, pos), 0},
            {1, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, uv), 0},
            {2, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, color), 0},
            {3, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, size), 0},
            {4, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, corners), 0},
            {5, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, bounds), 0},
            {6, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, bColor), 0}
        };

        d.depth.testEnable = false;
        d.depth.writeEnable = false;
        d.rasterizer.cullMode = CullMode::None;
        d.blend[0].enable = true;

        d.colorTargetCount = 1;
        d.colorFormats[0] = Format::RGBA8_UNORM;
        d.depthFormat = Format::Unknown;

        d.debugName = "UIMaterialPipeline";
        pipeline = GetDevice().CreatePipeline(d);
        buildFailed = (pipeline == nullptr);
    }

    VoidCore::RHI::ITexture *textures[VoidCore::RHI::kMaxTextureSlots] = {};
    VoidCore::RHI::ISampler *samplers[VoidCore::RHI::kMaxTextureSlots] = {};
    
    std::unordered_map<std::string, float> floatParams;
    std::unordered_map<std::string, glm::vec4> vectorParams;

    VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> pipeline;
    bool buildFailed = false;
};
