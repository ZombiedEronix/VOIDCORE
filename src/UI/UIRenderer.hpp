#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Device/RHI.hpp"
#include "UI/UICommand.hpp"

class UIRenderer
{
private:
    std::vector<UICommand> uiQueue;
    VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> uiVertexBuffer;
    size_t currentUiCapacity = 2048;

    struct UIVertex
    {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec4 color;
    };

public:
    void Submit(const UICommand &cmd)
    {
        uiQueue.push_back(cmd);
    }

    void Render(VoidCore::RHI::ICommandList *cmdList);
};
