#pragma once
#include "glad/glad.h"
#include "Device/IVAO.hpp"
#include "Device/IVBO.hpp"
#include "Device/IEBO.hpp"
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