#pragma once
#include <vector>
#include <memory>
#include <glm.hpp>

#include "Graphics/RenderPipeline/RenderCommand.h"
#include "Graphics/RenderPipeline/LightCommand.h"
#include "Device/RHI.hpp"
#include "Components/Camera.h"
#include "tracy/Tracy.hpp"

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
	VoidCore::RHI::ICommandList *cmd = nullptr; // действителен между BeginFrame и Flush

	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> cameraBuffer;   // UBO, binding 0
	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> instanceBuffer; // mat4 на инстанс, vertex slot 1
	size_t currentBufferCapacity = 1024;                       // в матрицах

	Camera *currentCamera = nullptr;
	OpaquePass *opaquePass;

	std::vector<RenderCommand> opaqueQueue;
	std::vector<glm::mat4> matrices;
	RenderStats stats;

	float clearColor[4] = {0.35f, 0.35f, 0.35f, 1.0f};

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

	void CreateInstanceBuffer(size_t capacity);
	void UploadFrameData();
	void UploadInstances(const std::vector<RenderCommand> &queue);

public:
	Renderer();
	~Renderer();

	void SetCamera(Camera *camera);
	const RenderStats &GetStats() const;

	// Начало кадра: сюда приходит command list от device->BeginFrame().
	void BeginFrame(VoidCore::RHI::ICommandList *commandList);

	void Submit(Mesh *mesh, Material *mat, const glm::mat4 &model);
	void SubmitLight(const LightCommand &light);

	// Залить данные в буферы → render pass → draw'ы. После него можно device->EndFrame().
	void Flush();

	// Только draw'ы; вызывается из пассов внутри render pass.
	void DrawBatch(const std::vector<RenderCommand> &queue);
};
