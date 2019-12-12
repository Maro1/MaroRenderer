#pragma once

#include <GLFW/glfw3.h>
#include <functional>
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

	inline int GetWidth() { return m_Data.Width; }
	inline int GetHeight() { return m_Data.Height; }

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
};