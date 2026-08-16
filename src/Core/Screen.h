#pragma once

#include <iostream>
#include <vector>
#include <glad.h>
#include <glfw3.h>
#include <Core/Debug.h>
struct VideoMode
{
	int width;
	int height;
	int refreshRate;
};

class Screen {
public:
	inline static GLFWmonitor* primaryMonitor = nullptr;
	inline static GLFWwindow* windowPtr;

	inline static int width = 1280;
	inline static int height = 720;

	inline static bool isFullScreen = false;

	static float GetAspectRatio() { return (float)width / (float)height; }

	static int Initialize()
	{
#if defined _DEBUG
		Debug::Log("Initializing Screen...");
#endif
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		windowPtr = glfwCreateWindow(width, height, "Entropy Engine", NULL, NULL);
		primaryMonitor = glfwGetPrimaryMonitor();
		glfwMakeContextCurrent(Screen::windowPtr);


		int code = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#if defined _DEBUG
		Debug::Log("Loading OpenGL...");
		if (!code) {
			Debug::LogError("Failed to initialize GLAD");
			Debug::Log("Initialization stopped.");
			return -1;
		}
#endif
		glfwSetFramebufferSizeCallback(windowPtr, OnChangedFrameBufferSize);

		return 0;
	}

	static void SetVerticalSync(bool enabled)
	{
		Debug::Log("set Vertical sync to ", enabled == true ? "Enabled" : "Disabled");
		glfwSwapInterval(enabled ? 1 : 0);
	}

	static void Destroy()
	{
		Debug::Log("Shutting down GLFW...");
		glfwDestroyWindow(windowPtr);
		glfwTerminate();
	}

	static void OnChangedFrameBufferSize(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	static void SetResolution(int w, int h)
	{
		Debug::Log("Setting video resolution to ", w, "x", h);

		width = w;
		height = h;

		if (windowPtr != nullptr && isFullScreen == false)
		{
			glfwSetWindowSize(windowPtr, width, height);
		}
	}

	static void SetResolution(GLFWvidmode mode)
	{
		Debug::Log("Setting video resolution to ", mode.width, "x", mode.height);

		width = mode.width;
		height = mode.height;

		if (windowPtr != nullptr)
		{
			if (isFullScreen)
			{
				glfwSetWindowMonitor(windowPtr, glfwGetPrimaryMonitor(), 0, 0, mode.width, mode.height, GLFW_DONT_CARE);
			}
			else
			{
				SetResolution(mode.width, mode.height);
			}
		}
	}

	static void GetVideoModes(std::vector<VideoMode>& modes)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		int count;
		const GLFWvidmode* videoModes = glfwGetVideoModes(monitor, &count);
		modes.clear();
		for (int i = 0; i < count; i++)
		{
			VideoMode mode;
			mode.width = videoModes[i].width;
			mode.height = videoModes[i].height;
			mode.refreshRate = videoModes[i].refreshRate;
			modes.push_back(mode);
		}
	}

	static void Update()
	{
		glfwSwapBuffers(windowPtr);
		glfwPollEvents();
	}

	static bool isWindowShouldClose() { return glfwWindowShouldClose(windowPtr); }

	static void SetFullScreen(bool fullscreen)
	{
		Debug::Log("Set to", fullscreen == false ? "fullscreen mode" : "windowed");

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		isFullScreen = fullscreen;
		if (fullscreen)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(windowPtr, monitor, 0, 0, width, height, mode->refreshRate);
		}
		else
		{
			glfwSetWindowMonitor(windowPtr, NULL, 100, 100, width, height, GLFW_DONT_CARE);
		}
	}
};