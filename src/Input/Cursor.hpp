#pragma once
#include <SDL3/SDL.h>

enum class CursorLockMode
{
    None = 0,
    Locked = 1,
    Confined = 2
};

class Cursor
{
private:
    static inline CursorLockMode lockState = CursorLockMode::None;
    static inline bool isVisible = true;

public:
    static void SetLockState(CursorLockMode mode)
    {
        lockState = mode;
        SDL_Window *activeWindow = SDL_GetKeyboardFocus();
        if (!activeWindow)
            return;

        if (mode == CursorLockMode::Locked)
        {
            SDL_SetWindowRelativeMouseMode(activeWindow, true);
        }
        else if (mode == CursorLockMode::Confined)
        {
            SDL_SetWindowRelativeMouseMode(activeWindow, false);
            SDL_SetWindowMouseGrab(activeWindow, true);
            SDL_ShowCursor();
        }
        else
        {
            SDL_SetWindowRelativeMouseMode(activeWindow, false);
            SDL_SetWindowMouseGrab(activeWindow, false);
            if (isVisible)
                SDL_ShowCursor();
        }
    }

    static void SetVisible(bool visible)
    {
        isVisible = visible;
        if (lockState != CursorLockMode::Locked)
        {
            if (visible)
                SDL_ShowCursor();
            else
                SDL_HideCursor();
        }
    }

    static void SetCursorTexture(SDL_Cursor *texture)
    {
        if (texture)
            SDL_SetCursor(texture);
    }

    static CursorLockMode GetLockState() { return lockState; }
    static bool IsVisible() { return isVisible; }
};
