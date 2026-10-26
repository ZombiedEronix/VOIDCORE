#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Graphics/Resources/Shader.hpp"
#include "Entity/GameObject.hpp"
#include "Entity/Components/Base/Component.hpp"
#include "Entity/Components/System/Transform.hpp"
#include "Core/Window.h"

#include "Entity/Components/Base/Behaviour.hpp"

class Window;


class Camera : public Behaviour
{
private:
	glm::vec4 planes[6];
public:
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 orthoProjection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	unsigned int cameraUBO;

	float fov = 75;
	float neardist = 0.01f;
	float fardist = 1000;

	void Update() override;

	glm::mat4 GetViewMatrix() const;

	bool IsPointInFrustum(const glm::vec3& point);
};
