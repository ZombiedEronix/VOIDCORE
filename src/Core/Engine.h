#pragma once
#include <memory>
#include <iostream>
#include "Core/Window.h"
#include "Core/SceneManagement/SceneManager.hpp"
#include "Core/EngineTime.h"
#include "Core/Debug.h"
#include "Graphics/Renderer.hpp"
#include "Device/RHI.hpp"

namespace VoidCore::Core
{
	class Engine
	{
	public:
		/// @brief Initializes the engine and starts the game cycle.
		static void Run();
		/// @brief Shuts down the engine and stops the cycle by unloading the scenes and clearing all resources.
		static void ShutDown();

		private:
		static inline bool isRunning = false;
		/// @brief Game Loop
		static void Update();

		/// @brief Outputs the logo of the game engine in the log outputs.
		static void PrintLogo();
	};
}