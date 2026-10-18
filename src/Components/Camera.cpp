#include "Camera.h"

Window* window = nullptr;

Camera::Camera(Window* wnd)
{
    window = wnd;
    // UBO камеры принадлежит Renderer (binding 0), здесь ничего не делаем.
}

glm::mat4 Camera::GetViewMatrix() const {
    return view;
}

void Camera::Update()
{
    if (!gameObject || !gameObject->transform) return;

    glm::vec3 pos = gameObject->transform->GetPosition();
    glm::vec3 rot = gameObject->transform->GetRotation(); // (Pitch, Yaw, Roll)

    // Вычисляем направление взгляда (Forward) на основе углов
    float pitch = glm::radians(rot.x);
    float yaw = glm::radians(rot.y);

    glm::vec3 forward;
    forward.x = cos(yaw) * cos(pitch);
    forward.y = sin(pitch);
    forward.z = sin(yaw) * cos(pitch);
    forward = glm::normalize(forward);

    // Строим View матрицу
    view = glm::lookAt(pos, pos + forward, glm::vec3(0, 1, 0));

    float aspect = window->GetAspectRatio();
    projection = glm::perspective(glm::radians(fov), aspect, neardist, fardist);
    cameraMatrix = projection * view;


    glm::mat4 m = glm::transpose(cameraMatrix);

    planes[0] = m[3] - m[0]; // Left
    planes[1] = m[3] + m[0]; // Right
    planes[2] = m[3] - m[1]; // Bottom
    planes[3] = m[3] + m[1]; // Top
    // Глубина NDC в [0,1] (GLM_FORCE_DEPTH_ZERO_TO_ONE): 0 <= z <= w.
    planes[4] = m[3] - m[2]; // Far  (z <= w)
    planes[5] = m[2];        // Near (z >= 0)  — в GL-конвенции [-1,1] было бы m[3] + m[2]

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