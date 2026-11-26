#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include "Assets/Vertex.h"
#include "Core/Debug.h"

#include "OBJ/OBJImporter.hpp"

class ModelImporter
{
public:
	static bool Load(const std::string &path, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices)
	{
		Debug::Log("Loading Model : ", path);

		std::ifstream file(path, std::ios::binary);
		if (!file.is_open())
		{
			Debug::LogError("Model is not imported.\nReason: File is not exist!");
			return false;
		}

		std::string ext = path.substr(path.find_last_of("."));
		if (ext == ".obj") return OBJImporter::LoadOBJStream(file, path, outVertices, outIndices);

		Debug::LogError("Model is not imported.\nReason: Format is not supported!");
		return false;
	}

	static bool LoadFromMemory(const std::string &path, const std::vector<uint8_t> &buffer, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices)
	{
		Debug::Log("Loading Model from VPAK :", path);

		std::string rawData(reinterpret_cast<const char *>(buffer.data()), buffer.size());
		std::istringstream stream(rawData, std::ios::binary);

		std::string ext = path.substr(path.find_last_of("."));
		
		if (ext == ".obj") return OBJImporter::LoadOBJStream(stream, path, outVertices, outIndices);

		Debug::LogError("Model is not imported from VPAK.\nReason: Format is not supported!");
		return false;
	}

private:

	
};
