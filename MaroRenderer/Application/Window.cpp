#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

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
	glfwPollEvents();
}

void Window::CreateWindow(unsigned int width, unsigned height, const char* title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(width, height, title, NULL, NULL);

	glfwMakeContextCurrent(mWindow);
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	SetCallbacks();

}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(mWindow);
}

void Window::SetCallbacks()
{
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height);
}

