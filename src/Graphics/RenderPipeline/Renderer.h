#pragma once
#include <vector>
#include <memory>
#include <glm.hpp>

#include "Graphics/RenderPipeline/RenderCommand.h"
#include "Graphics/RenderPipeline/LightCommand.h"
#include "Device/IVBO.hpp"
#include "Components/Camera.h"

class Camera;
class OpaquePass;
class Mesh;
class Material;

struct RenderStats
{
	uint32_t drawCalls = 0;
	uint32_t instanceCount = 0;
	uint32_t verticesCount = 0;
	float frameTimeMs = 0.0f;

	void Reset()
	{
		drawCalls = 0;
		instanceCount = 0;
		verticesCount = 0;
	}
};

class Renderer
{
private:
	unsigned int uboCamera;
	Camera *currentCamera = nullptr;

	std::unique_ptr<VoidCore::RHI::IVBO> instanceVBO;
	std::unique_ptr<VoidCOre::RHI::IUBO> uboCamera;
	OpaquePass *opaquePass;

	std::vector<RenderCommand> opaqueQueue;
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
	Renderer();
	~Renderer();

	void SetCamera(Camera *camera);
	const RenderStats &GetStats() const;
	void UpdateInstanceBuffer(size_t totalObjects);

	void Submit(Mesh *mesh, Material *mat, const glm::mat4 &model);
	void Clear();
	void SubmitLight(const LightCommand &light);

	void Update();
	void Flush();
	void DrawBatch(const std::vector<RenderCommand> &queue);
};