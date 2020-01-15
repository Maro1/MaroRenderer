#pragma once

#include <glad/glad.h>
#include <vector>
#include "Objects/Light.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"

class Renderer
{
public:

	Renderer();

	void Draw();
	void DrawLight();

	Shader* lightShader;

private:

	unsigned int VBO, VAO;

	unsigned int lightVAO;

};
