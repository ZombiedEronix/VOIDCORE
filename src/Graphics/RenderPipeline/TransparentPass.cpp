#include "TransparentPass.h"

#include "Graphics/RenderPipeline/Renderer.h"
#include "Graphics/RenderPipeline/RenderCommand.h"

#include "Graphics/Material.h"

#include <algorithm>
void TransparentPass::Execute(Renderer* renderer, std::vector<RenderCommand>& renderQueue)
{
    glm::vec3 camPos = Camera::main->gameObject->transform->GetPosition();

    std::sort(renderQueue.begin(), renderQueue.end(), [camPos](const RenderCommand& a, const RenderCommand& b) {
        float distA = glm::length2(camPos - glm::vec3(a.modelMatrix[3]));
        float distB = glm::length2(camPos - glm::vec3(b.modelMatrix[3]));
        return distA > distB;
        });

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_FALSE);

    renderer->DrawBatch(renderQueue);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}