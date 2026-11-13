#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Assets/Texture.h"
#include "Assets/Mesh.h"
#include "Graphics/Resources/Shader.hpp"
#include "ModelImporter.h"
#include "File.hpp"
#include "Assets/VPAK/VPAKReading.hpp"
#include "VMAT/MaterialLoader.hpp"

class ResourceManager
{
private:
	inline static std::map<std::string, std::shared_ptr<Texture>> textures;
	inline static std::map<std::string, std::shared_ptr<Mesh>> meshes;
	inline static std::map<std::string, std::shared_ptr<Shader>> shaders;
	inline static std::map<std::string, std::shared_ptr<Material>> materials;
	inline static std::map<std::string, std::shared_ptr<VoidCore::Assets::File>> files;
	inline static VoidCore::VPAK::AssetPakReader pakReader;

public:
	static bool MountPak(const std::string &pakPath)
	{
		return pakReader.Mount(pakPath);
	}

	template <typename T>
	static std::shared_ptr<T> GetResource(const std::string &path)
	{
		if constexpr (std::is_same_v<T, Texture>)
		{
			return std::static_pointer_cast<T>(GetTexture(path, false));
		}
		if constexpr (std::is_same_v<T, Mesh>)
		{
			return std::static_pointer_cast<T>(GetMesh(path));
		}
		if constexpr (std::is_same_v<T, VoidCore::Assets::File>)
		{
			return std::static_pointer_cast<T>(GetFile(path));
		}
		return nullptr;
	}

	static Material *GetMaterial(const std::string &path)
	{
		if (materials.find(path) == materials.end())
		{
			auto fileAsset = GetFile(path, false);
			if (!fileAsset || fileAsset->IsEmpty())
			{
				Debug::LogError("ResourceManager: Material file not found -> " + path);
				return nullptr;
			}

			auto mat = MaterialLoader::LoadYAML(
				*fileAsset,
				[](const std::string &shaderPath)
				{ return GetShader(shaderPath); },
				[](const std::string &texPath, bool srgb)
				{ return GetTexture(texPath, srgb); });

			if (!mat)
			{
				return nullptr;
			}

			materials[path] = mat;
		}
		return materials[path].get();
	}

	static std::shared_ptr<Texture> GetTexture(const std::string &path, bool srgb = false)
	{
		if (textures.find(path) == textures.end())
		{
			std::vector<uint8_t> buffer;
			if (pakReader.ReadFile(path, buffer))
			{
				textures[path] = std::make_shared<Texture>(buffer.data(), buffer.size(), path.c_str(), srgb);
			}
			else
			{
				textures[path] = std::make_shared<Texture>(path.c_str(), srgb);
			}
		}
		return textures[path];
	}

	static std::shared_ptr<VoidCore::Assets::File> GetFile(const std::string &path, bool useCache = true)
	{
		if (useCache && files.find(path) != files.end())
		{
			return files[path];
		}

		std::vector<uint8_t> buffer;

		if (!pakReader.ReadFile(path, buffer))
		{
			std::ifstream diskFile(path, std::ios::binary | std::ios::ate);
			if (!diskFile.is_open())
			{
				return nullptr;
			}

			std::streamsize size = diskFile.tellg();
			if (size > 0)
			{
				diskFile.seekg(0, std::ios::beg);
				buffer.resize(static_cast<size_t>(size));
				diskFile.read(reinterpret_cast<char *>(buffer.data()), size);
			}
		}

		auto asset = std::make_shared<VoidCore::Assets::File>(path, std::move(buffer));
		if (useCache)
		{
			files[path] = asset;
		}
		return asset;
	}

	static Mesh *GetMesh(const std::string &path)
	{
		if (meshes.find(path) == meshes.end())
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<uint8_t> buffer;

			if (pakReader.ReadFile(path, buffer))
			{
				ModelImporter::LoadFromMemory(path, buffer, vertices, indices);
			}
			else
			{
				//ADD LOGGING
				ModelImporter::Load(path, vertices, indices);
			}

			if (!vertices.empty() && !indices.empty())
			{
				meshes[path] = std::make_shared<Mesh>(vertices, indices);
			}
			else
			{
				return nullptr;
			}
		}
		return meshes[path].get();
	}

	static Shader *GetShader(const std::string &basePath)
	{
		if (shaders.find(basePath) == shaders.end())
		{
			std::string path = basePath + ".glsl";
			std::string shaderSource;

			if (pakReader.ReadTextFile(path, shaderSource))
			{
				shaders[basePath] = Shader::CreateFromSource(shaderSource, path.c_str());
			}
			else
			{
				Debug::LogWarning("Shader : ", basePath.c_str(), "is not found");
			}
		}
		return shaders[basePath].get();
	}

	static void Clear()
	{
		textures.clear();
		meshes.clear();
		shaders.clear();
		Texture::ReleaseSharedResources();
	}
};
