#pragma once
#include <memory>
#include <vector>
#include "Device/IVAO.hpp"
#include "Device/IVBO.hpp"
#include "Device/IEBO.hpp"
#include "Device/RenderFactory.hpp" // Твой класс фабрики
#include "Vertex.h"
#include "Assets/Asset.h"
#include "Core/Debug.h"

class Mesh : public Asset
{
private:
	int vertexCount;
	unsigned int indexCount;

public:
	std::shared_ptr<VoidCore::RHI::IVAO> vao;
	std::shared_ptr<VoidCore::RHI::IVBO> vbo;
	std::shared_ptr<VoidCore::RHI::IEBO> ebo;

	Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
	{
		// Создаем объекты через фабрику (бэкенд-независимо)
		vao = VoidCore::RHI::RenderFactory::CreateVAO();
		vao->Bind();

		indexCount = static_cast<unsigned int>(indices.size());

		// Передаем сырые данные и размер в байтах (через sizeof)
		vbo = VoidCore::RHI::RenderFactory::CreateVBO(vertices.data(), vertices.size() * sizeof(Vertex));
		ebo = VoidCore::RHI::RenderFactory::CreateEBO(indices.data(), indices.size() * sizeof(GLuint));

		ebo->Bind();

		vao->LinkVertexBuffer(vbo.get(), 0, 3, 0x1406 /* GL_FLOAT */, sizeof(Vertex), (const void *)offsetof(Vertex, position));
		vao->LinkVertexBuffer(vbo.get(), 1, 2, 0x1406 /* GL_FLOAT */, sizeof(Vertex), (const void *)offsetof(Vertex, texCoords));
		vao->LinkVertexBuffer(vbo.get(), 2, 3, 0x1406 /* GL_FLOAT */, sizeof(Vertex), (const void *)offsetof(Vertex, normal));
		vao->Unbind();
		ebo->Unbind();
		vbo->Unbind();

		vertexCount = static_cast<int>(vertices.size());
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