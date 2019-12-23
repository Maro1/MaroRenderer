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

	lightShader = new Shader(lightVShader, lightFShader);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	// Light things, only here temporarely

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void Renderer::Draw()
{
	glBindVertexArray(VAO);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::DrawLight()
{

	lightShader->SetMat4("model", light.GetModel());

	glBindVertexArray(lightVAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}
