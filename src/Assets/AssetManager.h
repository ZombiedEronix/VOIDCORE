#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <Assets/Mesh.h>
#include <Assets/ModelImporter.h>

class AssetManager {
private:
	std::unordered_map<std::string, std::shared_ptr<Asset>> loadedAssets;
public:
	template<typename T>
	std::shared_ptr<T> Get(const std::string& path)
	{
		if (loadedAssets.find(path) != loadedAssets.end())
		{
			return std::dynamic_pointer_cast<T>(loadedAssets[path]);
		}

		auto asset = std::make_shared<T>();
		asset->load(path);
		loadedAssets[path] = asset;
		return asset;
	}
};