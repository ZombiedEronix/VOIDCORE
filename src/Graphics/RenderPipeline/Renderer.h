#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "Graphics/RenderPipeline/GeometryRenderQueue.hpp"

#include "Device/RHI.hpp"
#include <Tracy/tracy/Tracy.hpp>

#include "Entity/Components/System/Camera.hpp"

#include "Graphics/RenderPipeline/IRenderPipeline.hpp"
#include "Graphics/RenderPipeline/LightCommand.hpp"

class Camera;
class GeometryRenderQueue;
class Mesh;
class Material;
class UIRenderer;
class Window;

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

// Переместить в другой файл...
struct ShaderBlock
{
	glm::mat4 U_Projection;
	glm::mat4 U_View;
	glm::mat4 U_InvProj;
	glm::mat4 U_InvView;
	glm::mat4 U_OrthoProjection;
	float U_Time;
	float U_DeltaTime;
	float U_ScreenWidth;
	float U_ScreenHeight;
};
// Переместить в другой файл...

class Renderer
{
private:
	VoidCore::RHI::ICommandList *cmd = nullptr;
	Window *windowPtr;
	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> cameraBuffer;
	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> instanceBuffer;
	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> lightBuffer;
	size_t currentBufferCapacity = 1024;

	Camera *currentCamera = nullptr;

	IRenderPipeline *renderPipeline;
	GeometryRenderQueue *opaqueQueue;
	std::vector<glm::mat4> instanceMatrices;

	ShaderBlock block;
	LightsBlock *lightBlock;

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
	Renderer(Window *window);
	~Renderer();

	void SetCamera(Camera *camera);
	void BeginFrame(VoidCore::RHI::ICommandList *commandList);
	Window *GetWindow() { return windowPtr; }
	void Submit(Mesh *mesh, Material *mat, const glm::mat4 &model);

	void SubmitLight(LightCommand &info);

	void Flush();

	void DrawBatch(const std::vector<GeometryRenderCommand> &queue);
};
