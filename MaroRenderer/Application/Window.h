#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <array>
#include "Events/Event.h"

class Window
{
public:

	Window();
	Window(unsigned int width, unsigned height, const char* title);

	void Update();
	bool ShouldClose();

	using EventCallbackFn = std::function<void(Event&)>;

	void SetEventCallback(const EventCallbackFn& Callback);

	void ToggleFullscreen();

	inline int GetWidth() { return m_Data.Width; }
	inline int GetHeight() { return m_Data.Height; }

	inline GLFWwindow* GetGLFWwindow() { return m_Window; }

private:

	void CreateWindow(unsigned int width, unsigned height, const char* title);
	void SetCallbacks();

	GLFWwindow* m_Window;

	struct WindowData
	{
		const char* Title;
		unsigned int Width, Height;
		EventCallbackFn EventCallback;
	};

	WindowData m_Data;

	std::array< int, 2 > m_WindowPos{ 0, 0 };
	std::array< int, 2 > m_WindowSize{ 0, 0 };

	bool m_Fullscreen = false;
};