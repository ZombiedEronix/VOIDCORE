#include "Input.hpp"

void Input::NewFrame()
{
    keysPressedThisFrame.clear();
    mouseButtonsPressedThisFrame.clear();
    mouseDelta = glm::vec2(0.0f, 0.0f);
}

void Input::ProcessEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (!event.key.repeat)
        {
            keys[event.key.key] = true;
            keysPressedThisFrame[event.key.key] = true;
        }
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        keys[event.key.key] = false;
    }
    else if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        mousePosition = glm::vec2(event.motion.x, event.motion.y);
        mouseDelta = glm::vec2(event.motion.xrel, event.motion.yrel);
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        mouseButtons[event.button.button] = true;
        mouseButtonsPressedThisFrame[event.button.button] = true;
    }
    else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        mouseButtons[event.button.button] = false;
    }
}

bool Input::IsKeyHeld(SDL_Keycode key)
{
    auto it = keys.find(key);
    return it != keys.end() ? it->second : false;
}

bool Input::IsKeyPressed(SDL_Keycode key)
{
    auto it = keysPressedThisFrame.find(key);
    return it != keysPressedThisFrame.end() ? it->second : false;
}
