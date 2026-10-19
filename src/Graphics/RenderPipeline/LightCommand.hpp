#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/vec4.hpp>

struct LightInfo
{
    glm::vec4 Position = glm::vec4(0,0,0,1);
    glm::vec4 Color = glm::vec4(1);
    glm::vec4 Rotation;
};