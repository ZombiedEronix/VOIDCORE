#pragma once
#include "Entity/Components/Base/Behaviour.hpp"
#include <memory>

class UIControl;
class UICanvas;

class UIDocument : public Behaviour
{
public:
    std::shared_ptr<UIControl> root;

    void UpdateLayout(float screenW, float screenH);
};
