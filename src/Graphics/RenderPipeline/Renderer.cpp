#include "Renderer.h"
#include "glad/glad.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Assets/Mesh.h"
#include "Graphics/Material.h"
#include "Components/Camera.h"
#include "Core/EngineTime.h"
#include "Graphics/RenderPipeline/OpaquePass.h"
#include "Device/RenderFactory.hpp"

Renderer::Renderer()
{
    instanceVBO = VoidCore::RHI::RenderFactory::CreateVBO(NULL, currentBufferCapacity * sizeof(glm::mat4));
    block = ShaderBlock();

    glGenBuffers(1, &uboCamera);
    glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderBlock), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboCamera);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    opaquePass = new OpaquePass();
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

void Renderer::UpdateInstanceBuffer(size_t totalObjects)
{
    if (totalObjects > currentBufferCapacity)
    {
        currentBufferCapacity = totalObjects * 2;
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO->GetID());
        glBufferData(GL_ARRAY_BUFFER, currentBufferCapacity * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Renderer::Submit(Mesh *mesh, Material *mat, const glm::mat4 &model)
{
    switch (mat->queueType)
    {
    case RenderQueue::Opaque:
        opaqueQueue.push_back({mesh, mat, model});
        break;
    }
}

void Renderer::Clear()
{
    glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Renderer::SubmitLight(const LightCommand &light)
{
    if (block.lightCount < 16)
    {
        block.lights[block.lightCount] = light;
        block.lightCount++;
    }
}

void Renderer::Update()
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

void Renderer::Flush()
{
    stats.Reset();
    opaquePass->Execute(this, opaqueQueue);
    opaqueQueue.clear();
    block.lightCount = 0;
}

void Renderer::DrawBatch(const std::vector<RenderCommand> &queue)
{
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

        for (size_t j = start; j < i; j++)
            matrices.push_back(queue[j].modelMatrix);

        instanceVBO->Bind();
        instanceVBO->SetData(matrices.data(), count * sizeof(glm::mat4));

        queue[start].material->ApplyStatic();
        queue[start].mesh->vao->Bind();

        stats.instanceCount += count;

        for (int k = 0; k < 4; k++)
        {
            glEnableVertexAttribArray(3 + k);
            glVertexAttribPointer(3 + k, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * k));
            glVertexAttribDivisor(3 + k, 1);
        }

        stats.drawCalls++;

        glDrawElementsInstanced(GL_TRIANGLES, queue[start].mesh->GetIndexCount(), GL_UNSIGNED_INT, 0, (GLsizei)count);
    }
}