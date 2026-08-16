 #pragma once
#include "Vendor/imgui/imgui.h"
#include <Vendor/imgui/imgui_impl_opengl3.h>
#include <Vendor/imgui/imgui_impl_glfw.h>
#include <Vendor/imgui/imgui_internal.h>
#include <Core/Screen.h>
#include <Core/SceneManager.h>
#include <Core/Engine.h>
class DebugUI
{
private:
	inline static GameObject* selectedObj = nullptr;
	static bool initialized;
	static bool enabled;
public:
	static void Initialize();
	static void Update();
};