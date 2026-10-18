#include <algorithm>

#include "UI/UIRenderer.hpp"

void UIRenderer::Render(VoidCore::RHI::ICommandList *cmdList)
{
    if (uiQueue.empty())
        return;

    // 1. СОРТИРОВКА ПО СЛОЯМ
    std::sort(uiQueue.begin(), uiQueue.end(), [](const UICommand &a, const UICommand &b)
              {
        if (a.layer != b.layer) 
            return a.layer < b.layer;
        return a.texture < b.texture; });

    // 2. СБОРКА ГЕОМЕТРИИ С НУЛЯ (Превращаем Rect в треугольники)
    std::vector<UIVertex> vertices;
    vertices.reserve(uiQueue.size() * 6);

    for (const auto &ui : uiQueue)
    {
        float x1 = ui.rect.x;
        float y1 = ui.rect.y;
        float x2 = ui.rect.x + ui.rect.w;
        float y2 = ui.rect.y + ui.rect.h;

        glm::vec2 uv00(0.0f, 0.0f);
        glm::vec2 uv10(1.0f, 0.0f);
        glm::vec2 uv01(0.0f, 1.0f);
        glm::vec2 uv11(1.0f, 1.0f);

        // Треугольник 1
        vertices.push_back({{x1, y1}, uv00, ui.color});
        vertices.push_back({{x2, y1}, uv10, ui.color});
        vertices.push_back({{x1, y2}, uv01, ui.color});

        // Треугольник 2
        vertices.push_back({{x1, y2}, uv01, ui.color});
        vertices.push_back({{x2, y1}, uv10, ui.color});
        vertices.push_back({{x2, y2}, uv11, ui.color});
    }

    // Создаем буфер через RHI, если его еще нет
    if (!uiVertexBuffer || vertices.size() > currentUiCapacity)
    {
        currentUiCapacity = std::max(vertices.size(), size_t(2048));
        VoidCore::RHI::BufferDesc bd;
        bd.size = currentUiCapacity * sizeof(UIVertex);
        bd.usage = VoidCore::RHI::BufferUsage::Vertex;
        bd.memory = VoidCore::RHI::MemoryType::Dynamic;
        bd.debugName = "UI_VertexBuffer";
        uiVertexBuffer = VoidCore::RHI::GetDevice().CreateBuffer(bd);
    }

    // Заливаем вершины кадра на видеокарту вне render pass
    cmdList->UpdateBuffer(uiVertexBuffer.get(), vertices.data(), vertices.size() * sizeof(UIVertex));

    // 3. ОТРИСОВКА (Запускаем пасс поверх 3D сцены)
    VoidCore::RHI::RenderPassDesc passDesc;
    passDesc.debugName = "UI_Render_Pass";
    passDesc.colorCount = 1;
    passDesc.colors[0].loadOp = VoidCore::RHI::LoadOp::Load; // Не затираем 3D!
    passDesc.hasDepth = false;

    cmdList->BeginRenderPass(passDesc);
    cmdList->SetVertexBuffer(0, uiVertexBuffer.get());

    size_t vertexOffset = 0;
    for (size_t i = 0; i < uiQueue.size();)
    {
        size_t start = i;
        while (i < uiQueue.size() && uiQueue[i].texture == uiQueue[start].texture)
        {
            i++;
        }
    }
    cmdList->EndRenderPass();
}