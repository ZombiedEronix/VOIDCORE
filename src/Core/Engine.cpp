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
        Time::Update();
        SceneManager::Update();

        if (Window::IsShowed() == true && Window::IsCreated())
        {
            Window::Update();

            if (Window::ShouldClose())
            {
                Window::Close();
                ShutDown();
                return;
            }

            auto *device = Window::GetRenderDevice();
            Renderer *renderer = Window::GetRenderer();

            renderer->BeginFrame(device->BeginFrame());
            SceneManager::Render(*renderer);

            renderer->Flush();
            GC::Collect();
            device->EndFrame();
        }
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
