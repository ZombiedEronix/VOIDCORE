#pragma once
#include <glm/glm.hpp>

#include "UI/Rect.hpp"

class Texture;

struct UICommand
{
    Rect rect;
    Texture *texture;
    glm::vec4 color;
    int layer;
};
