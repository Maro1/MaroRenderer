#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:

	Window();
	Window(unsigned int width, unsigned height, const char* title);

	void Update();
	bool ShouldClose();

private:
	void CreateWindow(unsigned int width, unsigned height, const char* title);

	GLFWwindow* mWindow;
};