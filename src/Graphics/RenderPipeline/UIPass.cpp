// #include "UIPass.h"
// #include "Device/RenderFactory.hpp"

// UIPass::UIPass()
// {
//     uiVAO = VoidCore::RHI::RenderFactory::CreateVAO();
//     uiVBO = VoidCore::RHI::RenderFactory::CreateVBO(nullptr, 1024 * sizeof(float)); // Буфер на 1024 вершины
//     uiVAO->LinkVertexBuffer(uiVBO.get(), 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
//     // ... загрузи сюда свой UI-шейдер ...
// }

// void UIPass::Execute(Renderer *renderer, std::vector<RenderCommand> &uiQueue)
// {
//     if (uiQueue.empty())
//         return;

//     // 1. Отключаем тест глубины
//     renderer->GetContext()->SetDepthTest(false);

//     // 2. Собираем вершины (по 6 вершин на каждый прямоугольник - 2 треугольника)
//     std::vector<float> vertices;
//     for (auto &cmd : uiQueue)
//     {
//         // Тут ты просто переводишь Rect {x, y, w, h} в 6 треугольников
//         // и добавляешь в vertices...
//     }

//     uiVBO->Bind();
//     uiVBO->SetData(vertices.data(), vertices.size() * sizeof(float));

//     uiShader->Load();
//     uiVAO->Bind();
//     glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

//     renderer->GetContext()->SetDepthTest(true);
// }