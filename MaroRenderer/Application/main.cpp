#include "Renderer/Renderer.h"
#include "Application/Window.h"
#include <iostream>
#include "Logging/Logger.h"
#include "Renderer/Shader.h"
#include <vector>

const char* vShader = "#version 460 core\n"

						"layout(location = 0) in vec2 aPos;\n"

						"void main()\n"
						"{\n"
						"gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
						"}\n";

const char* fShader = "#version 460 core\n"

						"out vec3 color;\n"

						"void main()\n"
						"{\n"
						"color = vec3(0.2, 0.2, 0.2);\n"
						"}\n";



int main()
{
	Window window;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
	}

	Renderer basicRenderer;

	Shader basicShader(vShader, fShader);
	basicShader.Use();

	while (!window.ShouldClose())
	{
		window.Update();

		basicRenderer.Draw();
		
	}
}

