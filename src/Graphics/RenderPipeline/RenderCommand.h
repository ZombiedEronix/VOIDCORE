#pragma once
#include <glm/glm.hpp>

class Mesh;
class Material;

struct GeometryRenderCommand
{
    Mesh *mesh;
    Material *material;
    glm::mat4 modelMatrix;
};