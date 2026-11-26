#pragma once
#include "Graphics/Renderer.hpp"
#include "Entity/Components/System/Transform.hpp"
#include "Entity/Material.hpp"
#include "Assets/Vertex.h"
#include "Assets/Mesh.h"
#include "GarbageCollector/Ref.hpp"

namespace VoidCore::Rendering
{
	class Renderer;
}

namespace VoidCore
{
	class MeshRenderer : public Behaviour
	{
	public:
		Ref<const Mesh> mesh;
		Ref<Material> material;

		MeshRenderer();
		glm::mat4 Matrix() const;
		void SetMesh(Ref<Mesh> meshPtr);
		void Render(Rendering::Renderer &renderer);
	};
}