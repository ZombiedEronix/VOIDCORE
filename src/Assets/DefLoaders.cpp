#include "Assets/AssetRegistry.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Mesh/OBJ/OBJImporter.hpp"

REGISTER_ASSET_LOADER(Shader, ".vcsp", [](const VoidCore::Assets::File &file) -> Ref<Shader>
                      { return new Shader(file.GetData(), file.GetSize(), file.GetPath().c_str()); });

REGISTER_ASSET_LOADER(Mesh, ".obj", [](const VoidCore::Assets::File &file) -> Ref<Mesh>
                      {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::istringstream stream(file.AsString(), std::ios::binary);
    if (OBJImporter::LoadOBJStream(stream, vertices, indices))
        return new Mesh(vertices, indices);

    return nullptr; });