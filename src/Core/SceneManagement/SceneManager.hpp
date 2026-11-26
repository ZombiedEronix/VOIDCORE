#pragma once

#include "Scene.hpp"

#include "LoadSceneMode.hpp"

namespace VoidCore::Rendering
{
	class Renderer;
}

namespace VoidCore::SceneManagement
{
	class SceneManager
	{
	public:
		inline static Scene *currentScene = nullptr; ///< Currently active scene

		/// @brief Loads a new scene using the specified load mode.
		/// @param obj Pointer to the scene to load.
		/// @param loadMode Mode determining how the scene is loaded.
		static void LoadScene(Scene *obj, LoadSceneMode loadMode);

		/// @brief Updates the active scene.
		static void Update();

		/// @brief Renders the active scene.
		/// @param renderer Reference to the active renderer.
		static void Render(Rendering::Renderer &renderer);

		/// @brief Unloads the currently active scene and frees its resources.
		static void UnloadScene();
	};
}