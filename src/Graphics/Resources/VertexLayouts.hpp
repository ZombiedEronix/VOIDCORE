#pragma once
#include <cstddef>
#include <glm/glm.hpp>

#include "Device/RHI.hpp"
#include "Assets/Vertex.h"

// The only place where the format of mesh vertices and instances is described.
// Must match layout(location = N) in shaders:
// 0 pos | 1 uv | 2 normal — slot 0 (to the top)
// 3..6 mat4 model — slot 1 (per instance)

inline void SetupMeshInputLayout(VoidCore::RHI::GraphicsPipelineDesc &d)
{
	using namespace VoidCore::RHI;
	d.vertexBuffers = {
		{sizeof(Vertex), VertexInputRate::PerVertex},
		{sizeof(glm::mat4), VertexInputRate::PerInstance}};

	d.vertexAttributes = {
		{0, Format::RGB32_FLOAT, (uint32_t)offsetof(Vertex, position), 0},
		{1, Format::RG32_FLOAT, (uint32_t)offsetof(Vertex, texCoords), 0},
		{2, Format::RGB32_FLOAT, (uint32_t)offsetof(Vertex, normal), 0},
		{3, Format::RGBA32_FLOAT, 0, 1},
		{4, Format::RGBA32_FLOAT, 16, 1},
		{5, Format::RGBA32_FLOAT, 32, 1},
		{6, Format::RGBA32_FLOAT, 48, 1}};
}
