#pragma once
#include <vector>

#include "Entity/GameObject.hpp"
#include "Entity/Components/System/Camera.hpp"
#include "Entity/Components/System/MeshRenderer.hpp"

#include "Entity/Components/System/UI/UICanvas.hpp"

namespace VoidCore::Rendering
{
	class Renderer;
}

namespace VoidCore::SceneManagement
{
	class Scene
	{
	public:
		std::string name = "Scene"; ///< Scene display name
		std::vector<Ref<GameObject>> gameObjects;
		std::vector<UICanvas *> canvases;

		/// @brief Adds a game object to the scene.
		/// @param gameObject Pointer to the game object to add.
		void AddGameObject(GameObject *gameObject);

		/// @brief Updates all active game objects (runs Update, LateUpdate, and cleans up destroyed objects).
		void Update();

		/// @brief Collects render data from all visible objects and UI canvases.
		/// @param renderer Reference to the active renderer.
		void Render(Rendering::Renderer &renderer);
	};
}