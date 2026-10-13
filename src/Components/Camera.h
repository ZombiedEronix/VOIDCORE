#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"



#include "Graphics/Shader.h"
#include "Core/GameObject.h"
#include "Components/Component.h"
#include "Components/Transform.h"
#include "Core/Window.h"

class Window;


class Camera : public Component
{
private:
	glm::vec4 planes[6];
public:
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	unsigned int cameraUBO;

	float fov = 75;
	float neardist = 0.01f;
	float fardist = 1000;

	Camera(Window* window);

	void Update() override;

	glm::mat4 GetViewMatrix() const;

	bool IsPointInFrustum(const glm::vec3& point);
};
