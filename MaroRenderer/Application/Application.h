#pragma once

#include "Renderer/Renderer.h"
#include "Application/Window.h"
#include "Logging/Logger.h"
#include "Renderer/Shader.h"
#include "Events/Event.h"
#include "Renderer/Camera/PerspectiveCamera.h"

class Application
{
public:
	Application();
	~Application();

	void Run();
	void OnEvent(const Event& e);

	inline Window* GetWindow() { return m_Window; }
	inline Shader* GetShader() { return m_Shader; }

private:

	Window* m_Window;
	Renderer* m_Renderer;
	Shader* m_Shader;
	PerspectiveCamera* m_Camera;

	glm::mat4 m_ModelMat = glm::mat4(1.0f);
	glm::mat4 m_ProjMat = glm::mat4(1.0f);
	glm::mat4 m_ViewMat;

	float m_CurrMouseX = 0.0;
	float m_CurrMouseY = 0.0;

	bool bLeftMousePressed = false;

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

		"out vec4 color;\n"
		"uniform vec4 inColor;\n"

		"void main()\n"
		"{\n"
		"color = inColor;\n"
		"}\n";

};