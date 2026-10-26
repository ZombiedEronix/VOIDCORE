#include "GeometryRenderQueue.hpp"
#include <algorithm>
#include <tuple>

#include "Graphics/Renderer.hpp"
#include "Graphics/Resources/Material.h"
#include "Assets/Mesh.h"

void GeometryRenderQueue::AddCommand(const GeometryRenderCommand &command)
{
    m_Queue.push_back(command);
}

const std::vector<GeometryRenderCommand>& GeometryRenderQueue::GetData() const
{
    return m_Queue;
}

void GeometryRenderQueue::Sort()
{
    if (m_Queue.empty())
        return;

    std::sort(m_Queue.begin(), m_Queue.end(), [](const GeometryRenderCommand &a, const GeometryRenderCommand &b)
              { return std::tie(a.material->shader, a.material, a.mesh) <
                       std::tie(b.material->shader, b.material, b.mesh); });
}

void GeometryRenderQueue::Execute(Renderer *renderer)
{
    if (m_Queue.empty())
        return;
        renderer->DrawBatch(m_Queue);
}

void GeometryRenderQueue::Clear()
{
    m_Queue.clear();
}
