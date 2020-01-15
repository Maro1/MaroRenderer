#include "Renderer.h"
#include "Logging/Logger.h"
#include "GLFW/glfw3.h"

Renderer::Renderer()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
	}
}

void Renderer::Draw()
{
	glBindVertexArray(VAO);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawLight()
{

	lightShader->SetMat4("model", light.GetModel());

	glBindVertexArray(lightVAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}
