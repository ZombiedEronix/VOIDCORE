#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Assets/Mesh.h"
#include "Graphics/Material.h"
#include "Entity/Components/System/Camera.hpp"
#include "Core/EngineTime.h"

#include "Graphics/RenderPipeline/GeometryRenderQueue.hpp"

#include "Graphics/RenderPipeline/Deferred/DefferedRenderPipeline.hpp"

using namespace VoidCore::RHI;
using namespace VoidCore::Rendering::Deferred;

Renderer::Renderer(Window* window)
{
    windowPtr = window;
    block = ShaderBlock();

    BufferDesc cb;
    cb.size = sizeof(ShaderBlock);
    cb.usage = BufferUsage::Uniform;
    cb.memory = MemoryType::Dynamic;
    cb.debugName = "FrameUBO";
    cameraBuffer = GetDevice().CreateBuffer(cb);
    ///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    BufferDesc lightB;
    lightB.size = sizeof(LightsBlock);
    lightB.usage = BufferUsage::Uniform;
    lightB.memory = MemoryType::Dynamic;
    lightB.debugName = "GlobalLightUBO";
    lightBuffer = GetDevice().CreateBuffer(lightB);
    ///=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    CreateInstanceBuffer(currentBufferCapacity);
    opaqueQueue = new GeometryRenderQueue();
    lightBlock = new LightsBlock();
    renderPipeline = new DeferredRenderPipeline();
}

void Renderer::CreateInstanceBuffer(size_t capacity)
{
    BufferDesc ib;
    ib.size = capacity * sizeof(glm::mat4);
    ib.usage = BufferUsage::Vertex;
    ib.memory = MemoryType::Dynamic;
    ib.debugName = "InstanceBuffer";
    instanceBuffer = GetDevice().CreateBuffer(ib);
    currentBufferCapacity = capacity;
}
Renderer::~Renderer()
{
    delete opaqueQueue;
}

void Renderer::SetCamera(Camera *camera)
{
    currentCamera = camera;
}

void Renderer::BeginFrame(ICommandList *commandList)
{
    cmd = commandList;
}

void Renderer::Submit(Mesh *mesh, Material *mat, const glm::mat4 &model)
{
    switch (mat->queueType)
    {
    case RenderQueue::Opaque:
        opaqueQueue->AddCommand({RenderMode::Opaque, mesh, mat, model});
        break;
    default:
        break;
    }
}

void Renderer::UploadFrameData()
{
    if (currentCamera)
    {
        block.U_Projection = currentCamera->projection;
        block.U_View = currentCamera->view;
        block.U_InvProj = glm::inverse(currentCamera->projection);
        block.U_InvView = glm::inverse(currentCamera->view);
    }
    else
    {
        block.U_InvProj = glm::mat4(1);
        block.U_InvView = glm::mat4(1);
        block.U_View = glm::mat4(1);
        block.U_Projection = glm::mat4(1);
    }

    block.U_Time = Time::time;
    block.U_DeltaTime = Time::deltaTime;
    block.U_ScreenWidth = Window::GetWidth();
    block.U_ScreenHeight = Window::GetHeight();
    cmd->UpdateBuffer(cameraBuffer.get(), &block, sizeof(ShaderBlock));
}

void Renderer::UploadInstances(const GeometryRenderQueue &queue)
{
    instanceMatrices.clear();
    instanceMatrices.reserve(queue.GetCount());
    for (const GeometryRenderCommand &c : queue.GetData())
        instanceMatrices.push_back(c.modelMatrix);

    if (instanceMatrices.size() > currentBufferCapacity)
        CreateInstanceBuffer(instanceMatrices.size() * 2);

    if (!instanceMatrices.empty())
        cmd->UpdateBuffer(instanceBuffer.get(), instanceMatrices.data(), instanceMatrices.size() * sizeof(glm::mat4));
}

GeometryRenderQueue *Renderer::GetOpaqueQueue()
{
    return opaqueQueue;
}

void Renderer::UploadLightData()
{
    cmd->UpdateBuffer(lightBuffer.get(), lightBlock, sizeof(LightsBlock));
}

void Renderer::SubmitLight(LightCommand &info)
{
    lightBlock->AddLight(info);
}

void Renderer::Flush()
{
    ZoneScoped;
    if (!cmd)
        return;

    UploadFrameData();
    UploadInstances(*opaqueQueue);
    UploadLightData();
    if (renderPipeline)
    {
        renderPipeline->Execute(this);
    }

    opaqueQueue->Clear();
    lightBlock->Clear();
    cmd = nullptr;
}

void Renderer::DrawBatch(const std::vector<GeometryRenderCommand> &queue)
{
    ZoneScoped;

    for (size_t i = 0; i < queue.size();)
    {
        const size_t start = i;
        while (i < queue.size() && queue[i].material == queue[start].material && queue[i].mesh == queue[start].mesh)
            i++;

        const uint32_t count = static_cast<uint32_t>(i - start);
        Mesh *mesh = queue[start].mesh;

        if (!queue[start].material->Apply(cmd))
            continue;

        cmd->SetVertexBuffer(0, mesh->vertexBuffer.get());
        cmd->SetIndexBuffer(mesh->indexBuffer.get(), IndexType::UInt32);

        cmd->DrawIndexed(static_cast<uint32_t>(mesh->GetIndexCount()), count, 0, 0, static_cast<uint32_t>(start));
    }
}
