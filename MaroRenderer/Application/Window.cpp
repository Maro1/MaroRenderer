#include "Window.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window()
{
	glfwInit();
	CreateWindow(1080, 720, "Test");
}

Window::Window(unsigned int width, unsigned height, const char* title)
{
	CreateWindow(width, height, title);
}

void Window::Update()
{
	glfwSwapBuffers(mWindow);
	glfwWaitEvents();
}

void Window::CreateWindow(unsigned int width, unsigned height, const char* title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(width, height, title, NULL, NULL);

	glfwMakeContextCurrent(mWindow);
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(mWindow);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height);
}

