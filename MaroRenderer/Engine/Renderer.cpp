#include "Renderer.h"
#include "Logging/Logger.h"
#include "GLFW/glfw3.h"

Renderer::Renderer()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
	}
	glEnable(GL_DEPTH_TEST);

}

void Renderer::Clear()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
