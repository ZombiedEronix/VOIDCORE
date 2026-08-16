#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Assets/Texture.h"
#include "Assets/Mesh.h"
#include "ModelImporter.h" 

class ResourceManager {
private:
	static std::map<std::string, std::shared_ptr<Texture>> textures;
	static std::map<std::string, std::shared_ptr<Mesh>> meshes;
	static std::map<std::string, std::shared_ptr<Shader>> shaders;

public:
	template<typename T>
	static std::shared_ptr<T> GetResource(const std::string& path) {
		if constexpr (std::is_same_v<T, Texture>) {
			return std::static_pointer_cast<T>(GetTexture(path, GL_RGB));
		}
		if constexpr (std::is_same_v<T, Mesh>) {
			// Передаем путь в метод!
			return std::static_pointer_cast<T>(GetMesh(path));
		}
		return nullptr;
	}

	static std::shared_ptr<Texture> GetTexture(const std::string& path, GLint internalFormat) {
		if (textures.find(path) == textures.end()) {
			textures[path] = std::make_shared<Texture>(path.c_str(), internalFormat);
		}
		return textures[path];
	}

	static Mesh* GetMesh(const std::string& path) {
		if (meshes.find(path) == meshes.end()) {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;

			ModelImporter::Load(path, vertices, indices);

			if (!vertices.empty() && !indices.empty()) {
				meshes[path] = std::make_shared<Mesh>(vertices, indices);
			}
			else {
				return nullptr;
			}
		}
		return meshes[path].get();
	}

	static Shader* GetShader(const std::string& basePath) {
		if (shaders.find(basePath) == shaders.end()) {
			std::string vertPath = basePath + ".vert";
			std::string fragPath = basePath + ".frag";

			shaders[basePath] = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
		}
		return shaders[basePath].get();
	}
};

std::map<std::string, std::shared_ptr<Texture>> ResourceManager::textures;
std::map<std::string, std::shared_ptr<Mesh>> ResourceManager::meshes;
std::map<std::string, std::shared_ptr<Shader>> ResourceManager::shaders;