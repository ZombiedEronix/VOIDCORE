#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "Graphics/RenderPipeline/RenderCommand.h"
#include "Device/RHI.hpp"
#include "Components/Camera.h"
#include <Tracy/tracy/Tracy.hpp>

#include "UI/UIRenderer.hpp"
#include "Graphics/RenderPipeline/IRenderPipeline.hpp"

class Camera;
class OpaquePass;
class Mesh;
class Material;
class UIRenderer;

/**
 * @brief Структура характеристика рендера текущего кадра.
 */

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
	VoidCore::RHI::ICommandList *cmd = nullptr;

	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> cameraBuffer;
	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> instanceBuffer;

	size_t currentBufferCapacity = 1024;

	Camera *currentCamera = nullptr;


	IRenderPipeline* renderPipeline;
	OpaquePass *opaquePass;
	std::vector<GeometryRenderCommand> opaqueQueue;
	std::vector<glm::mat4> matrices;
	RenderStats stats;

	struct ShaderBlock
	{
		glm::mat4 projection;
		glm::mat4 view;
		float time;
		float deltaTime;
	};

	ShaderBlock block;

	void CreateInstanceBuffer(size_t capacity);
	void UploadFrameData();
	void UploadInstances(const std::vector<GeometryRenderCommand> &queue);

	
	public:
	std::vector<GeometryRenderCommand>& GetOpaqueQueue();
	VoidCore::RHI::ICommandList *GetCommandList() const { return cmd; }
	VoidCore::RHI::IBuffer *GetCameraBuffer() const { return cameraBuffer.get(); }
	VoidCore::RHI::IBuffer *GetInstanceBuffer() const { return instanceBuffer.get(); }

	Renderer();
	~Renderer();

	void SubmitTestRect(float x, float y, float w, float h, glm::vec4 color);
	void SetCamera(Camera *camera);
	const RenderStats &GetStats() const;
	void BeginFrame(VoidCore::RHI::ICommandList *commandList);

	void Submit(Mesh *mesh, Material *mat, const glm::mat4 &model);

	void Flush();

	void DrawBatch(const std::vector<GeometryRenderCommand> &queue);
};
