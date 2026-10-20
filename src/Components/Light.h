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
    LightCommand info;
    LightType type = LightType::DIRECTIONAL;
    float intensive = 1.0f;
    glm::vec3 color = glm::vec3(1, 1, 1);
    float radius = 50.0f;

    void Update() override
    {
        info.Position = glm::vec4(gameObject->transform->GetPosition(), radius);
        info.Color = glm::vec4(color, intensive);
        info.Rotation = glm::vec4(gameObject->transform->GetRotation(), static_cast<float>(type));
    }
};
