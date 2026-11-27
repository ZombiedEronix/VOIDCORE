#pragma once
#include <vector>
#include "Vertex.h"
#include "Device/RHI.hpp"
#include "Entity/Object.hpp"

class Mesh : public ::Object
{
public:
	VoidCore::RHI::VRef<VoidCore::RHI::IBuffer> vertexBuffer;
	VoidCore::RHI::VRef<VoidCore::RHI::IBuffer> indexBuffer;

	Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

	int GetVertexCount() const;
	int GetIndexCount() const;

private:
	int vertexCount = 0;
	unsigned int indexCount = 0;
};
