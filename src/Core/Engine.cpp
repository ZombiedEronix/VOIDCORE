#include "Engine.h"
#include "Device/RHI.hpp"
#include "Assets/ResourceManager.h"
#include <Tracy/tracy/Tracy.hpp>
#include "GarbageCollector/GC.hpp"

namespace VoidCore::Core
{
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

    

    void Engine::Update()
    {
        ZoneScopedN("Full Frame");
        // Resetting all input values
        Input::NewFrame();
        Time::Update();

        
        if (Window::IsCreated())
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
        }

        SceneManager::Update();

        if (Window::IsShowed() && Window::IsCreated())
        {
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

    void Engine::ShutDown()
    {
        Debug::Log("Engine is shutting down.");
        isRunning = false;
        SceneManager::UnloadScene();
        ResourceManager::Clear();
        Debug::Log("GoodBye!");
    }
}
