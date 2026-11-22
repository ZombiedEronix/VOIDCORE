#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Device/RHI.hpp"
#include "Graphics/Resources/Shader.hpp"
#include "Assets/Texture.h"
#include "Core/Debug.h"
#include "UI/Structures/UIVertex.hpp"
#include "Graphics/Resources/VertexLayouts.hpp"
#include "Entity/Object.hpp"
#include <cstring>

class Material : public Object
{
public:
    Material(const Shader *shader, VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> pipeline)
        : shader(shader), pipeline(pipeline)
    {
        if (!shader)
            return;

        const auto &layout = shader->GetMaterialLayout();
        if (layout.IsValid())
        {
            cpuBuffer.resize(layout.totalSize, 0);

            VoidCore::RHI::BufferDesc bd;
            bd.size = layout.totalSize;
            bd.usage = VoidCore::RHI::BufferUsage::Uniform;
            bd.memory = VoidCore::RHI::MemoryType::Dynamic;
            bd.debugName = "MaterialUniformBuffer";
            uniformBuffer = VoidCore::RHI::GetDevice().CreateBuffer(bd);
        }
    }

    const Shader *GetShader() const { return shader; }

    static VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> CreateUIPipeline(const Shader *shader)
    {
        using namespace VoidCore::RHI;
        GraphicsPipelineDesc d;
        d.vertexShader = shader->GetVS();
        d.fragmentShader = shader->GetFS();

        d.vertexBuffers = {{sizeof(UIVertex), VertexInputRate::PerVertex}};
        d.vertexAttributes = {
            {0, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, pos), 0},
            {1, Format::RG32_FLOAT, (uint32_t)offsetof(UIVertex, uv), 0},
            {2, Format::RGBA32_FLOAT, (uint32_t)offsetof(UIVertex, rect), 0},
        };

        d.depth.testEnable = false;
        d.depth.writeEnable = false;
        d.rasterizer.cullMode = CullMode::None;

        d.blend[0].enable = true;
        d.blend[0].srcColor = BlendFactor::SrcAlpha;
        d.blend[0].dstColor = BlendFactor::OneMinusSrcAlpha;
        d.blend[0].colorOp = BlendOp::Add;

        d.blend[0].srcAlpha = BlendFactor::One;
        d.blend[0].dstAlpha = BlendFactor::OneMinusSrcAlpha;
        d.blend[0].alphaOp = BlendOp::Add;

        d.colorTargetCount = 1;
        d.colorFormats[0] = Format::RGBA8_UNORM;
        d.depthFormat = Format::Unknown;
        d.debugName = "UIMaterialPipeline";

        return GetDevice().CreatePipeline(d);
    }

    static VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> CreateLitPipeline(const Shader *shader)
    {
        using namespace VoidCore::RHI;
        GraphicsPipelineDesc d;
        d.vertexShader = shader->GetVS();
        d.fragmentShader = shader->GetFS();

        SetupMeshInputLayout(d);

        d.colorTargetCount = 1;
        d.colorFormats[0] = Format::RGBA8_UNORM;
        d.depthFormat = Format::D24_UNORM_S8_UINT;

        d.depth.testEnable = true;
        d.depth.writeEnable = true;
        d.depth.compareOp = CompareOp::Less;
        d.rasterizer.cullMode = CullMode::Back;
        d.debugName = "MeshMaterialPipeline";

        return GetDevice().CreatePipeline(d);
    }

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

    void SetFloat(const std::string &name, float value)
    {
        if (!shader || cpuBuffer.empty())
            return;

        const auto &fields = shader->GetMaterialLayout().fields;
        auto it = fields.find(name);
        if (it != fields.end() && it->second.size >= sizeof(float))
        {
            std::memcpy(cpuBuffer.data() + it->second.offset, &value, sizeof(float));
            isDirty = true;
        }
    }

    void SetVector(const std::string &name, const glm::vec4 &value)
    {
        if (!shader || cpuBuffer.empty())
            return;

        const auto &fields = shader->GetMaterialLayout().fields;
        auto it = fields.find(name);
        if (it != fields.end())
        {
            size_t copySize = std::min<size_t>(it->second.size, sizeof(glm::vec4));
            std::memcpy(cpuBuffer.data() + it->second.offset, &value, copySize);
            isDirty = true;
        }
    }

    void SetColor(const std::string &name, const Color &color)
    {
        SetVector(name, glm::vec4(color.r, color.g, color.b, color.a));
    }

    void Upload(VoidCore::RHI::ICommandList *cmd)
    {
        if (uniformBuffer && !cpuBuffer.empty())
        {
            if (isDirty)
            {
                cmd->UpdateBuffer(uniformBuffer.get(), cpuBuffer.data(), cpuBuffer.size());
                isDirty = false;
            }
        }
    }

    // ALARM
    Material *Clone() const override
    {
        auto copy = new Material(shader, pipeline);

        if (!cpuBuffer.empty() && copy->cpuBuffer.size() == cpuBuffer.size())
        {
            copy->cpuBuffer = cpuBuffer;
            copy->isDirty = true;
        }

        for (uint32_t slot = 0; slot < VoidCore::RHI::kMaxTextureSlots; ++slot)
        {
            copy->assetTextures[slot] = assetTextures[slot];
            copy->rhiTextures[slot] = rhiTextures[slot];
            copy->rhiSamplers[slot] = rhiSamplers[slot];
        }

        return copy;
    }

    bool Apply(VoidCore::RHI::ICommandList *cmd, VoidCore::RHI::ISampler *defaultSampler = nullptr) const
    {
        if (!pipeline)
            return false;

        cmd->SetPipeline(pipeline.get());

        if (uniformBuffer && !cpuBuffer.empty())
        {
            cmd->SetUniformBuffer(shader->GetMaterialLayout().binding, uniformBuffer.get());
        }

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

    VoidCore::RHI::IPipeline *GetPipeline() const { return pipeline.get(); }

private:
    const Shader *shader = nullptr;

    Texture *assetTextures[VoidCore::RHI::kMaxTextureSlots] = {};
    VoidCore::RHI::ITexture *rhiTextures[VoidCore::RHI::kMaxTextureSlots] = {};
    VoidCore::RHI::ISampler *rhiSamplers[VoidCore::RHI::kMaxTextureSlots] = {};

    std::unordered_map<std::string, float> floatParams;
    std::unordered_map<std::string, glm::vec4> vectorParams;
    VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> uniformBuffer;

    std::vector<uint8_t> cpuBuffer;
    mutable bool isDirty = true;

    VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> pipeline;
};
