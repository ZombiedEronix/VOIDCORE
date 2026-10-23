#pragma once
#include "Graphics/RenderPipeline/Renderer.h"
#include "Assets/Texture.h"
#include "Entity/Components/System/Transform.hpp"
#include "Graphics/Material.h"
#include "Assets/Vertex.h"
#include "Assets/Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

class MeshRenderer : public Behaviour
{
public:
	Mesh* mesh;
	Material* material;
	
	MeshRenderer();
	glm::mat4 Matrix() const;
	void SetMesh(Mesh* meshPtr);
	void Render(Renderer& renderer);
};