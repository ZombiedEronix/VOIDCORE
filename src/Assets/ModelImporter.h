#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Assets/Vertex.h"
#include "Core/Debug.h"

struct MeshHeader {
	char magic[4] = { 'E','N','T','M' };
	uint32_t version = 1;
	uint32_t vCount;
	uint32_t iCount;
};


class ModelImporter
{
public:
	static bool Load(const std::string& path, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices)
	{
		Debug::Log("Loading Model :", path);

		std::ifstream file(path);
		if (!file.is_open()) {
			Debug::LogError("Model is not imported.\nReason: File is not exist!");
			return false;
		}
		
		std::string ext = path.substr(path.find_last_of("."));
		if (ext == ".obj") return LoadOBJ(path, outVertices, outIndices);
		if (ext == ".entm") return LoadEntropyModel(path, outVertices, outIndices);

		
		Debug::LogError("Model is not imported.\nReason: Format is not supported!");
		return false;
	}
private:
	static bool LoadEntropyModel(const std::string& path, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices)
	{
		std::ifstream file(path, std::ios::binary);
		if (!file.is_open()) return false;

		MeshHeader header;
		file.read((char*)&header, sizeof(MeshHeader));

		outVertices.resize(header.vCount);
		outIndices.resize(header.iCount);

		file.read((char*)outVertices.data(), sizeof(Vertex) * header.vCount);
		file.read((char*)outIndices.data(), sizeof(unsigned int) * header.iCount);

		Debug::Log("Exporting Entropy Model...");
		Debug::Log("Version :", header.version);
		Debug::Log("Vertices :", header.vCount);
		Debug::Log("Indices:", header.iCount);

		file.close();
		return true;
	}

	static bool LoadOBJ(const std::string& path, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices) {
		std::vector<glm::vec3> temp_positions;
		std::vector<glm::vec2> temp_texCoords;
		std::vector<glm::vec3> temp_normals;


		std::ifstream file(path);
		if (!file.is_open()) {
			Debug::LogError("Failed open file : " + path);
			return false;
		}
		outVertices.clear();
		outIndices.clear();

		unsigned int vertex_count = 0;
		std::string line;

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::string prefix;
			ss >> prefix;

			if (prefix == "v") {
				glm::vec3 pos;
				ss >> pos.x >> pos.y >> pos.z;
				temp_positions.push_back(pos);
			}
			else if (prefix == "vt") {
				glm::vec2 uv;
				ss >> uv.x >> uv.y;
				uv.y = 1.0f - uv.y;
				temp_texCoords.push_back(uv);
			}
			else if (prefix == "vn")
			{
				glm::vec3 nm;
				ss >> nm.x >> nm.y >> nm.z;
				temp_normals.push_back(nm);
			}
			else if (prefix == "f") {
				std::string vertexStr;
				std::vector<std::string> faceVertices;

				while (ss >> vertexStr) {
					faceVertices.push_back(vertexStr);
				}

				auto parseVertex = [&](const std::string& vStr) {
					std::stringstream vss(vStr);
					std::string s_pos, s_uv, s_norm;

					std::getline(vss, s_pos, '/');
					std::getline(vss, s_uv, '/');
					std::getline(vss, s_norm, '/');

					Vertex vertex;
					vertex.position = glm::vec3(0.0f);
					vertex.texCoords = glm::vec2(0.0f);

					if (!s_pos.empty()) {
						int pos_idx = std::stoi(s_pos) - 1;
						if (pos_idx >= 0 && pos_idx < temp_positions.size()) {
							vertex.position = temp_positions[pos_idx];
						}
					}

					if (!s_uv.empty()) {
						int uv_idx = std::stoi(s_uv) - 1;
						if (uv_idx >= 0 && uv_idx < temp_texCoords.size()) {
							vertex.texCoords = temp_texCoords[uv_idx];
						}
					}

					if (!s_norm.empty()) {
						int norm_idx = std::stoi(s_norm) - 1;
						if (norm_idx >= 0 && norm_idx < temp_normals.size())
							vertex.normal = temp_normals[norm_idx];
					}

					outVertices.push_back(vertex);
					};

				for (size_t i = 1; i < faceVertices.size() - 1; ++i) {
					parseVertex(faceVertices[0]);
					unsigned int idx0 = outVertices.size() - 1;

					parseVertex(faceVertices[i]);
					unsigned int idx1 = outVertices.size() - 1;

					parseVertex(faceVertices[i + 1]);
					unsigned int idx2 = outVertices.size() - 1;

					outIndices.push_back(idx0);
					outIndices.push_back(idx1);
					outIndices.push_back(idx2);
				}
			}
		}

		Debug::Log("Loaded model : " + path + " with " + std::to_string(outVertices.size()) + " vertices and " + std::to_string(outIndices.size()) + " indices.");
		Debug::Log("Model Info:");
		Debug::Log("//-------------------------------------------------");
		Debug::Log("Positions: " + std::to_string(temp_positions.size()));
		Debug::Log("UVs: " + std::to_string(temp_texCoords.size()));
		Debug::Log("Normals: " + std::to_string(temp_normals.size()));
		Debug::Log("Format : Object File (.obj)");
		Debug::Log("//-------------------------------------------------");


		return true;
	}
};
