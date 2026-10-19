#pragma once
#include "Device/RHI.hpp"
#include "Graphics/Shader.h"
#include "Graphics/VertexLayouts.h"
#include "Assets/Texture.h"
#include "Core/Debug.h"

enum class RenderQueue {
	Opaque,
	Masked,
	Transparent
};

class Material
{
public:
	Shader *shader = nullptr;
	RenderQueue queueType = RenderQueue::Opaque;

	void SetTexture(uint32_t slot, Texture *tex)
	{
		if (slot < VoidCore::RHI::kMaxTextureSlots)
			textures[slot] = tex;
	}

	// Привязывает pipeline и текстуры. false — материал невалиден, рисовать нельзя.
	bool Apply(VoidCore::RHI::ICommandList *cmd)
	{
		if (!pipeline && !buildFailed)
			Build();
		if (!pipeline)
			return false;

		cmd->SetPipeline(pipeline.get());
		for (uint32_t slot = 0; slot < VoidCore::RHI::kMaxTextureSlots; ++slot)
		{
			if (textures[slot])
				cmd->SetTexture(slot, textures[slot]->GetRHI(), textures[slot]->GetSampler());
		}
		return true;
	}

private:

	// Всё, что раньше делали glEnable/glBlendFunc/glDepthMask при каждой отрисовке,
	// теперь запекается в pipeline один раз. queueType после первого Apply не менять.
	void Build()
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
		SetupMeshInputLayout(d);

		d.colorTargetCount = 1;
		d.colorFormats[0] = Format::RGBA8_UNORM;
		d.depthFormat = Format::D24_UNORM_S8_UINT;

		if (queueType == RenderQueue::Transparent)
		{
			d.blend[0].enable = true;
			d.depth.writeEnable = false;
		}
		d.depthFormat = Format::D24_UNORM_S8_UINT;
		d.depth.testEnable = true;
		d.depth.writeEnable = true;
		d.depth.compareOp = CompareOp::Less;
		
		d.debugName = "MaterialPipeline";

		pipeline = GetDevice().CreatePipeline(d);
		buildFailed = (pipeline == nullptr);
		if (buildFailed) Debug::LogError("FAILED TO BUILD PIPELINE!");
	}

	Texture *textures[VoidCore::RHI::kMaxTextureSlots] = {};
	VoidCore::RHI::Ref<VoidCore::RHI::IPipeline> pipeline;
	bool buildFailed = false;
};
