#pragma once

#include <vector>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


#include "Entity/Components/Base/Component.hpp"

class Transform : public Component
{
protected:
    Transform* parent = nullptr;
    std::vector<Transform*> children;
    mutable glm::mat4 cachedWorldMatrix{ 1.0f };
    mutable bool isDirty = true;

    glm::vec3 Position{ 0.0f };
    glm::vec3 Rotation{ 0.0f };
    glm::vec3 Scale{ 1.0f };

public:
    Transform();

    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
    
    void SetPosition(const glm::vec3& pos);
    void SetRotation(const glm::vec3& rot);
    void SetScale(const glm::vec3& scl);
    void SetParent(Transform* prnt);
    void SetDirty();

    const Transform* GetParent();
    const std::vector<Transform*>& GetChildren();
    const glm::vec3& GetPosition();
    const glm::vec3& GetRotation();
    const glm::vec3& GetScale();

    glm::mat4 WorldMatrix() const;
    glm::mat4 LocalMatrix() const;
};