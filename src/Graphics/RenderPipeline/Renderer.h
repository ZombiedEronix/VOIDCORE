#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "Graphics/RenderPipeline/GeometryRenderQueue.hpp"

#include "Device/RHI.hpp"
#include "Components/Camera.h"
#include <Tracy/tracy/Tracy.hpp>

#include "UI/UIRenderer.hpp"
#include "Graphics/RenderPipeline/IRenderPipeline.hpp"
#include "Graphics/RenderPipeline/LightCommand.hpp"

class Camera;
class GeometryRenderQueue;
class Mesh;
class Material;
class UIRenderer;

struct LightsBlock
{
	LightCommand lights[64];
	int currentLights = 0;
	float padding[3];

	void AddLight(const LightCommand &info)
	{
		if (currentLights < 64)
		{
			lights[currentLights] = info;
			currentLights++;
		}
	}
	void Clear() { currentLights = 0; }
};

class Renderer
{
private:
	VoidCore::RHI::ICommandList *cmd = nullptr;

	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> cameraBuffer;
	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> instanceBuffer;

	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> lightBuffer;

	size_t currentBufferCapacity = 1024;
	LightsBlock *lightBlock;
	Camera *currentCamera = nullptr;

	IRenderPipeline *renderPipeline;
	GeometryRenderQueue *opaqueQueue;

	std::vector<glm::mat4> matrices;

	struct ShaderBlock
	{
		glm::mat4 Projection;
		glm::mat4 View;
		glm::mat4 InvProj;
		glm::mat4 InvView;
		float Time;
		float DeltaTime;
		float padding0;
		float padding1;
	};

	ShaderBlock block;

	void CreateInstanceBuffer(size_t capacity);
	void UploadFrameData();
	void UploadInstances(const GeometryRenderQueue &queue);

public:
	GeometryRenderQueue *GetOpaqueQueue();
	VoidCore::RHI::ICommandList *GetCommandList() const { return cmd; }
	VoidCore::RHI::IBuffer *GetCameraBuffer() const { return cameraBuffer.get(); }
	VoidCore::RHI::IBuffer *GetInstanceBuffer() const { return instanceBuffer.get(); }
	VoidCore::RHI::IBuffer *GetLightBuffer() const { return lightBuffer.get(); }
	void UploadLightData();
	Renderer();
	~Renderer();

	void SetCamera(Camera *camera);
	void BeginFrame(VoidCore::RHI::ICommandList *commandList);

	void Submit(Mesh *mesh, Material *mat, const glm::mat4 &model);

	void SubmitLight(LightCommand &info);

	void Flush();

	void DrawBatch(const std::vector<GeometryRenderCommand> &queue);
};
