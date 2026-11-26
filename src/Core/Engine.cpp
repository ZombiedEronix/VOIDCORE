#include "Engine.h"
#include "Device/RHI.hpp"
#include "Assets/ResourceManager.h"
#include <Tracy/tracy/Tracy.hpp>
#include "GarbageCollector/GC.hpp"

namespace VoidCore::Core
{
    /// @brief This method initializes the engine, and starts the game cycle.
    void Engine::Run()
    {
        PrintLogo();
        Debug::Log("Initializing VoidCore (C++20)");
        isRunning = true;
        while (isRunning)
        {
            Update();
        }
    }

    /// @brief This method is a game loop
    void Engine::Update()
    {
        ZoneScopedN("Full Frame");
        // Resetting all input values
        Input::NewFrame();
        Time::Update();

        SceneManager::Update();
        // If the window is shown and created, then update the window and render.
        if (Window::IsShowed() == true && Window::IsCreated())
        {
            // Updating and receiving input
            Window::Update();

            // If an attempt is made to close the window, then shut down.
            if (Window::ShouldClose())
            {
                Window::Close();
                ShutDown();
                return;
            }

            // Begin Frame
            auto *device = Window::GetRenderDevice();
            Renderer *renderer = Window::GetRenderer();

            renderer->BeginFrame(device->BeginFrame());

            // Collecting data for rendering
            SceneManager::Render(*renderer);
            renderer->Flush();

            //End
            device->EndFrame();
        }

        // Garbage cleaning
        GC::Collect();
        FrameMark;
    }

    /// @brief This method depicts the logo of the game engine in the log outputs.
    void Engine::PrintLogo()
    {
        std::cout << R"(
+=============================================================+
|                                                             |
|                                                             |
|      _    ______  ________     __________  ____  ______     |
|     | |  / / __ \/  _/ __ \   / ____/ __ \/ __ \/ ____/     |
|     | | / / / / // // / / /  / /   / / / / /_/ / __/        |
|     | |/ / /_/ // // /_/ /  / /___/ /_/ / _, _/ /___        |
|     |___/\____/___/_____/   \____/\____/_/ |_/_____/        |
|                                                             |
|                                                             |
+=============================================================+)"
                  << std::endl;
    }

    /// @brief This method shuts down the engine and stops the cycle by unloading the scenes and clearing all resources.
    void Engine::ShutDown()
    {
        Debug::Log("Engine is shutting down.");
        isRunning = false;
        SceneManager::UnloadScene();
        ResourceManager::Clear();
        Debug::Log("GoodBye!");
    }
}
