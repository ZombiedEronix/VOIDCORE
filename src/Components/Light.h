#pragma once

#include <glm/glm.hpp>

#include "Components/Component.h"
#include "Core/GameObject.h"

class Light : public Component
{
public:
    glm::vec3 color = glm::vec3(1,1,1);
    float radius = 16;
    float intensity = 1;

    void Update() override;
};