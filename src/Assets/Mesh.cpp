#include "Mesh.hpp"
#include "Device/RHI.hpp"

using namespace VoidCore::RHI;

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
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

int Mesh::GetVertexCount() const
{
    return vertexCount;
}

int Mesh::GetIndexCount() const
{
    return indexCount;
}