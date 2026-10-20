#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <glm/glm.hpp>

class Input
{
private:
    static inline std::unordered_map<uint8_t, bool> mouseButtons;
    static inline std::unordered_map<uint8_t, bool> mouseButtonsPressedThisFrame;

    static inline std::unordered_map<SDL_Keycode, bool> keys;
    static inline std::unordered_map<SDL_Keycode, bool> keysPressedThisFrame;

    static inline glm::vec2 mousePosition{0.0f, 0.0f};
    static inline glm::vec2 mouseDelta{0.0f, 0.0f};

public:
    static void NewFrame();

    static void ProcessEvent(const SDL_Event &event);

    static bool IsKeyHeld(SDL_Keycode key);
    static bool IsKeyPressed(SDL_Keycode key);

    static glm::vec2 GetMousePosition() { return mousePosition; }
    static glm::vec2 GetMouseDelta() { return mouseDelta; }
    static bool IsMouseButtonHeld(int button) { return mouseButtons[button]; } // 1 - Левая, 3 - Правая (SDL)
};
