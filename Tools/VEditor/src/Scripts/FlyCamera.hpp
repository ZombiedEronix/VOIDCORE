#pragma once
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include "Entity/Components/Base/Behaviour.hpp"
#include "Entity/GameObject.hpp"
#include "Input/Input.hpp"
#include "Input/Cursor.hpp"
#include "Core/EngineTime.h"

class FlyCamera : public Behaviour
{
public:
    float moveSpeed = 6.0f;
    float minSpeed = 0.5f;
    float maxSpeed = 100.0f;
    float scrollStep = 1.25f;

    float moveDamping = 12.0f;
    float lookDamping = 20.0f;
    float panSpeed = 0.015f;
    float sensitivity = 0.15f;

private:
    glm::vec3 currentVelocity{0.0f};
    glm::vec3 targetRotation{0.0f};
    bool initialized = false;

public:
    void Update() override
    {
        if (!gameObject || !gameObject->transform)
            return;

        Transform *transform = gameObject->transform;
        float dt = Time::GetUnscaledDeltaTime();

        if (!initialized)
        {
            targetRotation = transform->GetRotation();
            initialized = true;
        }

        float scrollY = Input::GetMouseScroll().y;
        if (std::abs(scrollY) > 0.001f)
        {
            if (scrollY > 0.0f)
                moveSpeed *= std::pow(scrollStep, scrollY);
            else
                moveSpeed /= std::pow(scrollStep, -scrollY);

            moveSpeed = std::clamp(moveSpeed, minSpeed, maxSpeed);
        }

        bool rmbHeld = Input::IsMouseButtonHeld(3);
        bool mmbHeld = Input::IsMouseButtonHeld(2);

        if (rmbHeld || mmbHeld)
        {
            if (Cursor::GetLockState() != CursorLockMode::Locked)
            {
                Cursor::SetLockState(CursorLockMode::Locked);
                Cursor::SetVisible(false);
            }
        }
        else if (Cursor::GetLockState() == CursorLockMode::Locked)
        {
            Cursor::SetLockState(CursorLockMode::None);
            Cursor::SetVisible(true);
        }

        glm::vec2 mouseDelta = Input::GetMouseDelta();

        if (mmbHeld && glm::length(mouseDelta) > 0.0001f)
        {
            glm::vec3 right = transform->GetRight();
            glm::vec3 up = transform->GetUp();

            glm::vec3 panOffset = (-right * mouseDelta.x + up * mouseDelta.y) * (panSpeed * moveSpeed * 0.25f);
            transform->SetPosition(transform->GetPosition() + panOffset);
        }

        if (rmbHeld && glm::length(mouseDelta) > 0.0001f)
        {
            targetRotation.y += mouseDelta.x * sensitivity;
            targetRotation.x -= mouseDelta.y * sensitivity;
            targetRotation.x = std::clamp(targetRotation.x, -89.0f, 89.0f);
        }

        glm::vec3 currentRot = transform->GetRotation();
        float lookT = 1.0f - std::exp(-lookDamping * dt);
        transform->SetRotation(glm::mix(currentRot, targetRotation, lookT));

        glm::vec3 wishDir{0.0f};
        if (rmbHeld)
        {
            if (Input::IsKeyHeld(KeyCode::W))
                wishDir += transform->GetForward();
            if (Input::IsKeyHeld(KeyCode::S))
                wishDir -= transform->GetForward();
            if (Input::IsKeyHeld(KeyCode::D))
                wishDir += transform->GetRight();
            if (Input::IsKeyHeld(KeyCode::A))
                wishDir -= transform->GetRight();
            if (Input::IsKeyHeld(KeyCode::E))
                wishDir += glm::vec3(0.0f, 1.0f, 0.0f);
            if (Input::IsKeyHeld(KeyCode::Q))
                wishDir -= glm::vec3(0.0f, 1.0f, 0.0f);
        }

        if (glm::length(wishDir) > 0.001f)
            wishDir = glm::normalize(wishDir);

        float speed = moveSpeed * (Input::IsKeyHeld(KeyCode::LShift) ? 3.0f : 1.0f);
        glm::vec3 targetVelocity = wishDir * speed;

        float moveT = 1.0f - std::exp(-moveDamping * dt);
        currentVelocity = glm::mix(currentVelocity, targetVelocity, moveT);

        if (glm::length(currentVelocity) > 0.0001f)
        {
            transform->SetPosition(transform->GetPosition() + currentVelocity * dt);
        }
    }
};
