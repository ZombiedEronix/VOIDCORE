#pragma once
#include "glad.h"
#include "Graphics/VAO.h"
#include "Graphics/VBO.h"
#include "Graphics/EBO.h"
#include <Graphics/RenderPipeline/Renderer.h>
#include "Assets/Texture.h"
#include "Components/Transform.h"
#include "Graphics/Material.h"
#include "Assets/Vertex.h"
#include "Assets/Mesh.h"

class MeshRenderer : public Component
{
public:
	Mesh* mesh;
	Material* material;
	
	MeshRenderer();
	glm::mat4 Matrix() const;
	void SetMesh(Mesh* meshPtr);
	void Render(Renderer& renderer);
};