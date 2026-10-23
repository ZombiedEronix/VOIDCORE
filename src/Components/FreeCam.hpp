#pragma once

#include "Components/Component.h"
#include "Core/GameObject.h"
#include "Components/Transform.h"
#include "Core/EngineTime.h"
#include "Input/Input.hpp"

class FreeCam : public Component
{
public:
    float moveSpeed = 5.0f;
    float lookSensitivity = .7f;

    void Update() override
    {
        Transform *transform = gameObject->transform;

        float currentSpeed = moveSpeed;
        if (Input::IsKeyHeld(SDLK_LSHIFT))
        {
            currentSpeed *= 3.0f;
        }

        glm::vec3 moveDir{0.0f};

        if (Input::IsKeyHeld(SDLK_W))
            moveDir += transform->GetForward();
        if (Input::IsKeyHeld(SDLK_S))
            moveDir -= transform->GetForward();
        if (Input::IsKeyHeld(SDLK_D))
            moveDir += transform->GetRight();
        if (Input::IsKeyHeld(SDLK_A))
            moveDir -= transform->GetRight();

        if (Input::IsKeyHeld(SDLK_E))
            moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
        if (Input::IsKeyHeld(SDLK_Q))
            moveDir -= glm::vec3(0.0f, 1.0f, 0.0f);

        if (glm::length(moveDir) > 0.0f)
        {
            glm::vec3 newPos = transform->GetPosition() + glm::normalize(moveDir) * currentSpeed * Time::deltaTime;
            transform->SetPosition(newPos);
        }

        if (Input::IsMouseButtonHeld(3))
        {
            glm::vec2 mouseDelta = Input::GetMouseDelta();

            if (glm::length(mouseDelta) > 0.0f)
            {
                glm::vec3 currentRot = transform->GetRotation();

                currentRot.y += mouseDelta.x * lookSensitivity;

                currentRot.x -= mouseDelta.y * lookSensitivity;

                currentRot.x = glm::clamp(currentRot.x, -89.0f, 89.0f);

                transform->SetRotation(currentRot);
            }
        }
    }
};
