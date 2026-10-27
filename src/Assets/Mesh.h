#pragma once
#include <memory>
#include <vector>
#include "Device/RHI.hpp"
#include "Vertex.h"
#include "Core/Debug.h"

class Mesh
{
private:
	int vertexCount = 0;
	unsigned int indexCount = 0;

public:
	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> vertexBuffer;
	VoidCore::RHI::Ref<VoidCore::RHI::IBuffer> indexBuffer; // uint32

	Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
	{
		using namespace VoidCore::RHI;
		auto &device = GetDevice();

		BufferDesc vb;
		vb.size = vertices.size() * sizeof(Vertex);
		vb.usage = BufferUsage::Vertex;
		vb.memory = MemoryType::Static;
		vb.initialData = vertices.data();
		vb.debugName = "MeshVB";
		vertexBuffer = device.CreateBuffer(vb);

		BufferDesc ib;
		ib.size = indices.size() * sizeof(uint32_t);
		ib.usage = BufferUsage::Index;
		ib.memory = MemoryType::Static;
		ib.initialData = indices.data();
		ib.debugName = "MeshIB";
		indexBuffer = device.CreateBuffer(ib);

		vertexCount = static_cast<int>(vertices.size());
		indexCount = static_cast<unsigned int>(indices.size());
	}

	int GetVertexCount() { return vertexCount; }
	int GetIndexCount() { return indexCount; }
};
