#pragma once

#include <glad/glad.h>
#include <vector>

class Renderer
{
public:

	Renderer();

	void Draw();

private:

	unsigned int VBO, VAO;

	float Vertices[6] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f, 0.5f
	};


};
