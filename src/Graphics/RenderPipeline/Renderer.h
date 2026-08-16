#pragma once
#include <vector>
#include "glad.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Assets/Mesh.h"
#include "Graphics/Material.h"
#include "Components/Camera.h"
#include "Core/EngineTime.h"

#include "Graphics/RenderPipeline/RenderCommand.h"
#include "Graphics/RenderPipeline/LightCommand.h"

#include "Graphics/RenderPipeline/OpaquePass.h"
#include "Graphics/RenderPipeline/TransparentPass.h"
#include "Graphics/RenderPipeline/ForwardPass.h"

struct RenderStats {
	uint32_t drawCalls = 0;
	uint32_t instanceCount = 0;
	uint32_t verticesCount = 0;
	float frameTimeMs = 0.0f;

	void Reset() {
		drawCalls = 0;
		instanceCount = 0;
		verticesCount = 0;
	}
};

class Renderer
{
private:
	inline static Renderer* instance = nullptr;

	unsigned int uboCamera;
	Camera* currentCamera;

	unsigned int instanceVBO;

	OpaquePass* opaquePass;
	TransparentPass* transparentPass;

	std::vector<RenderCommand> opaqueQueue;

	std::vector<RenderCommand> transparentQueue;

	std::vector<glm::mat4> matrices;

	RenderStats stats;

	size_t currentBufferCapacity = 1024;

	struct ShaderBlock
	{
		glm::mat4 projection;
		glm::mat4 view;
		float time;
		float deltaTime;
		int lightCount;
		float padding_final_field;
		LightCommand lights[16];
	};

	ShaderBlock block;
public:
	Renderer()
	{

		instance = this;

		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, currentBufferCapacity * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		block = ShaderBlock();
		//==================
		currentCamera = Camera::main;

		glGenBuffers(1, &uboCamera);
		glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderBlock), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboCamera);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		opaquePass = new OpaquePass();
		transparentPass = new TransparentPass();
	}

	const RenderStats& GetStats() const { return stats; }

	void UpdateInstanceBuffer(size_t totalObjects)
	{
		if (totalObjects > currentBufferCapacity)
		{
			currentBufferCapacity = totalObjects * 2;

			glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
			glBufferData(GL_ARRAY_BUFFER, currentBufferCapacity * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		}
	}

	~Renderer()
	{
		if (instance == this) instance = nullptr;
		delete opaquePass;
		delete transparentPass;
	}

	static Renderer* Get() { return instance; }

	void Submit(Mesh* mesh, Material* mat, const glm::mat4& model)
	{
		switch (mat->queueType)
		{
		case RenderQueue::Opaque:
			opaqueQueue.push_back({ mesh, mat, model });
			break;
		case RenderQueue::Transparent:
			transparentQueue.push_back({ mesh, mat, model });
		}
	}

	void Clear()
	{
		glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}

	void SubmitLight(const LightCommand& light) {
		if (block.lightCount < 16) {
			block.lights[block.lightCount] = light;
			block.lightCount++;
		}
	}

	void Update()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
		if (currentCamera)
		{
			block.projection = currentCamera->projection;
			block.view = currentCamera->view;
		}
		else
		{
			block.view = glm::mat4(1);
			block.projection = glm::mat4(1);
		}

		block.time = Time::time;
		block.deltaTime = Time::deltaTime;

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShaderBlock), &block);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Flush()
	{
		stats.Reset();

		opaquePass->Execute(this, opaqueQueue);

		transparentPass->Execute(this, transparentQueue);

		opaqueQueue.clear();
		transparentQueue.clear();
		block.lightCount = 0;
	}

	void DrawBatch(const std::vector<RenderCommand>& queue) {

		UpdateInstanceBuffer(queue.size());

		for (size_t i = 0; i < queue.size();)
		{
			size_t start = i;
			while (i < queue.size() && queue[i].material == queue[start].material && queue[i].mesh == queue[start].mesh)
			{
				i++;
				stats.verticesCount += queue[start].mesh->GetVertexCount();
			}

			size_t count = i - start;
			matrices.clear();

			for (size_t j = start; j < i; j++) matrices.push_back(queue[j].modelMatrix);

			glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(glm::mat4), matrices.data());

			queue[start].material->ApplyStatic();
			queue[start].mesh->vao->Bind();

			stats.instanceCount += count;

			for (int k = 0; k < 4; k++)
			{
				glEnableVertexAttribArray(3 + k);
				glVertexAttribPointer(3 + k, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * k));
				glVertexAttribDivisor(3 + k, 1);
			}

			stats.drawCalls++;

			glDrawElementsInstanced(GL_TRIANGLES, queue[start].mesh->GetIndexCount(), GL_UNSIGNED_INT, 0, (GLsizei)count);
		}
	}

};
