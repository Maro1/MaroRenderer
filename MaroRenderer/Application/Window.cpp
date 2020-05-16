#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Logging/Logger.h"

Window::Window()
{
	glfwInit();
	CreateWindow(2560, 1440, "Test");
}

Window::Window(unsigned int width, unsigned height, const char* title)
{
	CreateWindow(width, height, title);
}

void Window::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void Window::CreateWindow(unsigned int width, unsigned height, const char* title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, 1);

	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

	glfwGetWindowSize(m_Window, (int*)&m_Data.Width, (int*)&m_Data.Height);
	m_Data.Title = title;

	glfwMakeContextCurrent(m_Window);
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetWindowUserPointer(m_Window, &m_Data);

	SetCallbacks();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Window::SetEventCallback(const EventCallbackFn & Callback)
{
	m_Data.EventCallback = Callback;
}

void Window::ToggleFullscreen()
{
	if (!m_Fullscreen)
	{
		// For windowed fullscreen
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		// Backup windowed position/size
		glfwGetWindowPos(m_Window, &m_WindowPos[0], &m_WindowPos[1]);
		glfwGetWindowSize(m_Window, &m_WindowSize[0], &m_WindowSize[1]);

		glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		glfwSetWindowMonitor(m_Window, nullptr, m_WindowPos[0], m_WindowPos[1], m_WindowSize[0], m_WindowSize[1], 0);
	}
	m_Fullscreen = !m_Fullscreen;
}

void Window::SetCallbacks()
{
	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(character);
			data.EventCallback(event);
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event(xpos, ypos);

			data.EventCallback(event);
		});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			double xPos, yPos;
			glfwGetCursorPos(window, &xPos, &yPos);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MousePressedEvent event(button, (int) xPos, (int) yPos, mods);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseReleasedEvent event(button, (int) xPos, (int) yPos);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event(xOffset, yOffset);
			data.EventCallback(event);
		});
}



