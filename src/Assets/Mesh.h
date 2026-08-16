#pragma once
#include <memory>
#include <vector>
#include "Graphics/VAO.h"
#include "Vertex.h"
#include <Graphics/EBO.h>
#include <Assets/Asset.h>
#include "Core/Debug.h"

class Mesh : public Asset {
private:
	int vertexCount;
	unsigned int indexCount;
public:
	std::shared_ptr<VAO> vao;
	std::shared_ptr<VBO> vbo;
	std::shared_ptr<EBO> ebo;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
		vao = std::make_shared<VAO>();
		vao->Bind();
		indexCount = (unsigned int)indices.size();

		vbo = std::make_shared<VBO>((GLfloat*)vertices.data(), vertices.size() * sizeof(Vertex));
		ebo = std::make_shared<EBO>(indices.data(), indices.size() * sizeof(GLuint));

		ebo->Bind();

		vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
		vao->LinkAttrib(*vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		vao->Unbind();
		ebo->Unbind();
		vbo->Unbind();

		vertexCount = vertices.size();

	}

	int GetVertexCount()
	{
		return vertexCount;
	}

	int GetIndexCount()
	{
		return indexCount;
	}
};