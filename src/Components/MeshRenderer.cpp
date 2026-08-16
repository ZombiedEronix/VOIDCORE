#include "MeshRenderer.h"
#include <gtc/matrix_transform.hpp>
#include "Core/GameObject.h"
#include <iostream>

MeshRenderer::MeshRenderer() {}

glm::mat4 MeshRenderer::Matrix() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, gameObject->transform->GetPosition());
	model = glm::rotate(model, glm::radians(gameObject->transform->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, gameObject->transform->GetScale());
	return model;
}

void MeshRenderer::SetMesh(Mesh* meshPtr)
{
	mesh = meshPtr;
}

void MeshRenderer::Render(Renderer& renderer) {
	if (material && mesh) {
		renderer.Submit(mesh, material, this->gameObject->transform->WorldMatrix());
	}
}

