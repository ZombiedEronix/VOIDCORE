#pragma once
#include "glm.hpp"

struct LightCommand
{
    glm::vec3 position;
    float radius;

    glm::vec3 color;
    float intensity;
};