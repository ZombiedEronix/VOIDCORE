#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "Graphics/Queue/GeometryRenderQueue.hpp"

#include "Device/RHI.hpp"
#include <Tracy/tracy/Tracy.hpp>

#include "Entity/Components/System/Camera.hpp"

#include "Graphics/Pipeline/IRenderPipeline.hpp"
#include "Graphics/Queue/LightCommand.hpp"

#include "Graphics/Uniforms.hpp"

#include "UI/UIContext.hpp"

class Camera;
class GeometryRenderQueue;
class Mesh;
class Material;
namespace VoidCore::Rendering
{
	class UIPass;
}
class Window;

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
	VoidCore::Rendering::UIPass *uiPass = nullptr;
	UIContext uiContext;
	GeometryRenderQueue *opaqueQueue;
	std::vector<glm::mat4> instanceMatrices;

	ShaderBlock block;
	LightsBlock *lightBlock;

	void CreateInstanceBuffer(size_t capacity);
	void UploadFrameData();
	void UploadInstances(const GeometryRenderQueue &queue);

public:
	GeometryRenderQueue *GetOpaqueQueue();
	UIContext &GetUIContext() { return uiContext; }
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
