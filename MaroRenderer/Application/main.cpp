#include "Renderer/Renderer.h"
#include "Application/Window.h"
#include <iostream>
#include "Logging/Logger.h"
#include "Renderer/Shader.h"
#include <vector>
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Renderer/Camera/PerspectiveCamera.h"

const char* vShader = "#version 460 core\n"

						"layout(location = 0) in vec3 aPos;\n"

						"uniform mat4 model;\n"
						"uniform mat4 view;\n"
						"uniform mat4 projection;\n"

						"void main()\n"
						"{\n"
						"gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
						"}\n";

const char* fShader = "#version 460 core\n"

						"out vec3 color;\n"

						"void main()\n"
						"{\n"
						"color = vec3(0.2, 0.2, 0.2);\n"
						"}\n";

void OnEvent(const Event& e);

bool bLeftMousePressed = false;
glm::mat4 model = glm::mat4(1.0f);
float currX = 0.0;
float currY = 0.0;

int main()
{
	Window window;

	window.SetEventCallback(OnEvent);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
	}

	Renderer basicRenderer;

	Shader basicShader(vShader, fShader);
	basicShader.Use();

	PerspectiveCamera camera(glm::vec3(0.0f, -0.3f, 3.0f));

	while (!window.ShouldClose())
	{
		window.Update();

		glm::mat4 view;
		view = glm::lookAt(camera.GetLocation(), camera.GetLocation() + camera.GetFront(), camera.GetUp());

		basicShader.SetMat4("view", view);

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(45.0f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f);

		basicShader.SetMat4("projection", proj);

		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0, 1.0, 0.0));
		basicShader.SetMat4("model", model);

		basicRenderer.Draw();
		
	}
}

void OnEvent(const Event & e)
{
	if (e.GetType() == EventType::KeyPress)
	{
		KeyPressedEvent* keypressed = (KeyPressedEvent*) &e;
		std::cout << keypressed->GetKeyCode() << std::endl;
		LOG_INFO("Key Pressed.");
	}
	else if (e.GetType() == EventType::MouseButtonPress)
	{
		MousePressedEvent* mousepressed = (MousePressedEvent*) &e;
		int button = mousepressed->GetButton();
		if (button == 0)
		{
			bLeftMousePressed = true;
		}
	}
	else if (e.GetType() == EventType::MouseButtonRelease)
	{
		MouseReleasedEvent* mousereleased = (MouseReleasedEvent*)&e;
		int button = mousereleased->GetButton();
		if (button == 0)
		{
			bLeftMousePressed = false;
		}
	}
	else if (e.GetType() == EventType::MouseMove)
	{
		MouseMovedEvent* mousemoved = (MouseMovedEvent*)&e;
		float x = mousemoved->GetX();
		float y = mousemoved->GetY();
		if (bLeftMousePressed)
		{
			model = glm::rotate(model, (x - currX) * glm::radians(0.5f), glm::vec3(0.0, 1.0, 0.0));
			model = glm::rotate(model, (y - currY) * glm::radians(0.5f), glm::vec3(1.0, 0.0, 0.0));
		}
		currX = x;
		currY = y;
	}
	else if (e.GetType() == EventType::WindowResize)
	{
		WindowResizeEvent* windowrezize = (WindowResizeEvent*)&e;
		glViewport(0, 0, windowrezize->GetWidth(), windowrezize->GetHeight());
	}
}

