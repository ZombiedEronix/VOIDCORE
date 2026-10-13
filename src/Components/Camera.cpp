#include "Camera.h"

Window* window = nullptr;

Camera::Camera(Window* wnd)
{
    window = wnd;
    glGenBuffers(1, &cameraUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraUBO);
}

glm::mat4 Camera::GetViewMatrix() const {
    return view;
}

void Camera::Update()
{
    if (!gameObject || !gameObject->transform) return;

    glm::vec3 position = gameObject->transform->GetPosition();
    glm::quat rot = glm::radians(gameObject->transform->GetRotation());

    glm::mat4 rotationMat = glm::mat4_cast(rot);
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), -position);

    view = rotationMat * translationMat;

    float aspect = window->GetAspectRatio();
    projection = glm::perspective(glm::radians(fov), aspect, neardist, fardist);
    cameraMatrix = projection * view;


    glm::mat4 m = glm::transpose(cameraMatrix);

    planes[0] = m[3] - m[0]; // Left
    planes[1] = m[3] + m[0]; // Right
    planes[2] = m[3] - m[1]; // Bottom
    planes[3] = m[3] + m[1]; // Top
    planes[4] = m[3] - m[2]; // Near
    planes[5] = m[3] + m[2]; // Far

    for (int i = 0; i < 6; ++i) {
        float length = glm::length(glm::vec3(planes[i]));
        planes[i] /= length;
    }
}

bool Camera::IsPointInFrustum(const glm::vec3& point) {
    for (int i = 0; i < 6; ++i) {
        if (planes[i].x * point.x + planes[i].y * point.y + planes[i].z * point.z + planes[i].w < -17) {
            return false;
        }
    }
    return true;
}