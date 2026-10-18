#include <algorithm>
#include <tuple>
#include "Graphics/RenderPipeline/OpaquePass.h"

#include "Graphics/RenderPipeline/Renderer.h"
#include "Graphics/RenderPipeline/RenderCommand.h"

#include "Graphics/Material.h"
#include "Assets/Mesh.h"

void OpaquePass::Sort(std::vector<GeometryRenderCommand>& renderQueue)
{
    std::sort(renderQueue.begin(), renderQueue.end(), [](const GeometryRenderCommand& a, const GeometryRenderCommand& b)
        {
            return std::tie(a.material->shader, a.material, a.mesh) <
                   std::tie(b.material->shader, b.material, b.mesh);
        });
}

void OpaquePass::Execute(Renderer* renderer, std::vector<GeometryRenderCommand>& renderQueue)
{
    renderer->DrawBatch(renderQueue);
}
