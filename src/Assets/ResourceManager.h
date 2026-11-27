#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Assets/Texture.hpp"
#include "Assets/Mesh.hpp"
#include "Assets/Shader.hpp"
#include "File.hpp"
#include "Assets/VPAK/VPAKReading.hpp"
#include "MaterialLoader.hpp"
#include "Core/GarbageCollector/Ref.hpp"
#include "Assets/AssetRegistry.hpp"

class ResourceManager
{
public:
	static bool MountPak(const std::string &pakPath)
	{
		return pakReader.Mount(pakPath);
	}

	/// @brief Get's material from resources
	/// @param basePath path to material
	/// @return loaded material
	static const Ref<Material> GetMaterial(const std::string &path) { return Load<Material>(path); }

	/// @brief Get's shader from resources
	/// @param basePath path to shader
	/// @return loaded shader
	static const Ref<Shader> GetShader(const std::string &basePath) { return Load<Shader>(basePath + ".vcsp"); }

	/// @brief Get's mesh from resources
	/// @param basePath path to mesh
	/// @return loaded mesh
	static const Ref<Mesh> GetMesh(const std::string &path) { return Load<Mesh>(path); }

	/// @brief Get's file from resources
	/// @param basePath path to file
	/// @return loaded file
	static const Ref<VoidCore::Assets::File> GetFile(const std::string &path, bool useCache = true)
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
				return nullptr;

			std::streamsize size = diskFile.tellg();
			if (size > 0)
			{
				diskFile.seekg(0, std::ios::beg);
				buffer.resize(static_cast<size_t>(size));
				diskFile.read(reinterpret_cast<char *>(buffer.data()), size);
			}
		}

		Ref<VoidCore::Assets::File> asset = new VoidCore::Assets::File(path, std::move(buffer));
		if (useCache)
			files[path] = asset;

		return asset;
	}

	/// @brief Universal method for loading resources
	/// @param basePath path to asset
	/// @return loaded asset
	template <typename T>
	static Ref<T> Load(const std::string &path)
	{
		static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");

		auto it = cache.find(path);
		if (it != cache.end())
		{
			return static_cast<T *>(it->second.Get());
		}

		auto fileAsset = GetFile(path, false);
		if (!fileAsset || fileAsset->IsEmpty())
		{
			Debug::LogError("ResourceManager: File not found -> " + path);
			return nullptr;
		}

		Ref<T> asset = AssetRegistry::Load<T>(*fileAsset);
		if (asset)
		{
			cache[path] = asset.Get();
		}
		else
		{
			Debug::LogError("ResourceManager: No loader registered for -> " + path);
		}

		return asset;
	}

	/// @brief Get's font from resources (support .ttf only)
	/// @param basePath path to font
	/// @return loaded font
	static const Ref<Font> GetFont(const std::string &basePath, float px = 20)
	{
		std::string key = basePath + "_" + std::to_string(static_cast<int>(px));
		if (fonts.find(key) == fonts.end())
		{
			auto file = GetFile(basePath, false);
			if (file && !file->IsEmpty())
				fonts[key] = new Font(file->GetBytes(), px);
			else
				Debug::LogWarning("Font : ", basePath.c_str(), " is not found");
		}
		return fonts[key];
	}

	/// @brief Get's texture from resources
	/// @param basePath path to texture
	/// @return loaded texture
	static const Ref<Texture> GetTexture(const std::string &path, bool srgb = false)
	{
		std::string key = path + (srgb ? "_srgb" : "_lin");
		if (textures.find(key) == textures.end())
		{
			auto file = GetFile(path, false);
			if (file && !file->IsEmpty())
				textures[key] = new Texture(file->GetData(), file->GetSize(), path.c_str(), srgb);
			else
				textures[key] = new Texture(path.c_str(), srgb);
		}
		return textures[key];
	}

	/// @brief Clean resources
	static void Clear()
	{
		cache.clear();
		textures.clear();
		fonts.clear();
		files.clear();
		Texture::ReleaseSharedResources();
	}

private:
	inline static std::map<std::string, Ref<Object>> cache;

	inline static std::map<std::string, Ref<Texture>> textures;
	inline static std::map<std::string, Ref<Font>> fonts;
	inline static std::map<std::string, Ref<VoidCore::Assets::File>> files;
	inline static VoidCore::VPAK::AssetPakReader pakReader;
};
