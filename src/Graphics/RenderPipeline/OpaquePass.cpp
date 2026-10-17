#include <algorithm>
#include <tuple>
#include "Graphics/RenderPipeline/OpaquePass.h"

#include "Graphics/RenderPipeline/Renderer.h"
#include "Graphics/RenderPipeline/RenderCommand.h"

#include "Graphics/Material.h"
#include "Assets/Mesh.h"

void OpaquePass::Sort(std::vector<RenderCommand>& renderQueue)
{
    // shader → material → mesh: одинаковые (material, mesh) идут подряд и
    // склеиваются в один instanced draw. Порядок по указателям детерминирован
    // внутри кадра, этого достаточно для группировки.
    std::sort(renderQueue.begin(), renderQueue.end(), [](const RenderCommand& a, const RenderCommand& b)
        {
            return std::tie(a.material->shader, a.material, a.mesh) <
                   std::tie(b.material->shader, b.material, b.mesh);
        });
}

void OpaquePass::Execute(Renderer* renderer, std::vector<RenderCommand>& renderQueue)
{
    renderer->DrawBatch(renderQueue);
}
