#pragma once
namespace VoidCore::SceneManagement
{
    /// @brief Specifies the mode used when loading a scene.
    enum class LoadSceneMode
    {
        Single,  ///< Unloads all currently loaded scenes before loading the new one.
        Additive ///< Loads the new scene alongside the currently active scenes.
    };
}