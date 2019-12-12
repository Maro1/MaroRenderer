#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include "Events/Event.h"

using EventCallbackFn = std::function<void(Event&)>;

class Window
{
public:

	Window();
	Window(unsigned int width, unsigned height, const char* title);

	void Update();
	bool ShouldClose();

private:

	void CreateWindow(unsigned int width, unsigned height, const char* title);
	void SetCallbacks();

	GLFWwindow* mWindow;
};