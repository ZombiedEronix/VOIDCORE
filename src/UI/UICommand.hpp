#pragma once
#include <glm/glm.hpp>

#include "Rect.hpp"

class Material;

class Texture;

struct UICommand
{
    Rect rect;
    Material* mat;
    glm::vec4 color;
    int layer;
};
