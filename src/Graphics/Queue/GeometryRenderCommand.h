#pragma once
#include <glm/glm.hpp>

class Mesh;
class Material;

enum RenderMode
{
    Opaque,
    Transparent,
};

struct GeometryRenderCommand
{
    RenderMode mode = RenderMode::Opaque;
    const Mesh *mesh;
    const Material *material;
    glm::mat4 modelMatrix;
};