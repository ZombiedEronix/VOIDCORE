#pragma once

#include <glm/glm.hpp>

#include "Components/Component.h"
#include "Core/GameObject.h"

enum LightType : uint32_t
{
    POINT,
    DIRECTIONAL,
    SPOT,
};

class Light : public Component
{
public:
    LightType type = LightType::POINT;
    float intensive = 1;
    glm::vec3 color = glm::vec3(1);
    float radius = 10;

};