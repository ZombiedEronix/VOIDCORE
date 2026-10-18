#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Assets/Mesh.h"
#include "Graphics/Material.h"
#include "Components/Camera.h"
#include "Core/EngineTime.h"
#include "Graphics/RenderPipeline/OpaquePass.h"
#include "Graphics/RenderPipeline/Deffered/DefferedRenderPipeline.hpp"

using namespace VoidCore::RHI;

Renderer::Renderer()
{
    block = ShaderBlock();

    BufferDesc cb;
    cb.size = sizeof(ShaderBlock);
    cb.usage = BufferUsage::Uniform;
    cb.memory = MemoryType::Dynamic;
    cb.debugName = "FrameUBO";
    cameraBuffer = GetDevice().CreateBuffer(cb);

    CreateInstanceBuffer(currentBufferCapacity);
    opaquePass = new OpaquePass();
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
    delete opaquePass;
}


void Renderer::SetCamera(Camera *camera)
{
    currentCamera = camera;
}

const RenderStats &Renderer::GetStats() const
{
    return stats;
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
        opaqueQueue.push_back({mesh, mat, model});
        break;
    default:
        break;
    }
}

void Renderer::UploadFrameData()
{
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

    cmd->UpdateBuffer(cameraBuffer.get(), &block, sizeof(ShaderBlock));
}

void Renderer::UploadInstances(const std::vector<GeometryRenderCommand> &queue)
{
    matrices.clear();
    matrices.reserve(queue.size());
    for (const GeometryRenderCommand &c : queue)
        matrices.push_back(c.modelMatrix);

    if (matrices.size() > currentBufferCapacity)
        CreateInstanceBuffer(matrices.size() * 2);

    if (!matrices.empty())
        cmd->UpdateBuffer(instanceBuffer.get(), matrices.data(), matrices.size() * sizeof(glm::mat4));
}

std::vector<GeometryRenderCommand>& Renderer::GetOpaqueQueue()
{
    return opaqueQueue;
}

void Renderer::Flush()
{
    ZoneScoped;
    if (!cmd)
        return;
    stats.Reset();

    UploadFrameData();
    UploadInstances(opaqueQueue);

    if (renderPipeline)
    {
        renderPipeline->Execute(this);
    }

    opaqueQueue.clear();
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

        stats.drawCalls++;
        stats.instanceCount += count;
        stats.verticesCount += static_cast<uint32_t>(mesh->GetVertexCount()) * count;

        cmd->DrawIndexed(static_cast<uint32_t>(mesh->GetIndexCount()), count, 0, 0, static_cast<uint32_t>(start));
    }
}
