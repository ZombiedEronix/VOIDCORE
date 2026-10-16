#include <algorithm>
#include "Graphics/RenderPipeline/OpaquePass.h"

#include "Graphics/RenderPipeline/Renderer.h"
#include "Graphics/RenderPipeline/RenderCommand.h"

#include "Graphics/Material.h"
#include "Assets/Mesh.h"

void OpaquePass::Execute(Renderer* renderer, std::vector<RenderCommand>& renderQueue)
{
    std::sort(renderQueue.begin(), renderQueue.end(), [](const RenderCommand& a, const RenderCommand& b)
        {
            if (a.material->shader != b.material->shader)
                return a.material->shader->GetID() < b.material->shader->GetID();
            return a.mesh < b.mesh;
        });

    renderer->DrawBatch(renderQueue);
}