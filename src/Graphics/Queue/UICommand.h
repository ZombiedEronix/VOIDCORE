#pragma once
#include "UI/Rect.hpp"
#include "glm/glm.hpp"
#include "Assets/Texture.h"

struct UICommand
{
    Rect rect;
    Texture *texture;
    glm::vec4 color;
    int layer;
};