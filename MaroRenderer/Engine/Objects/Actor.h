#pragma once

#include "Engine/Objects/SceneNode.h"
#include "Engine/Shader.h"

class Actor : public SceneNode {

public:

	Actor(Shader* shader = DefaultActorShader(), 
		glm::vec3 location = glm::vec3(0.0f), 
		glm::vec3 scale = glm::vec3(1.0f), Model* model = NULL) 
		: SceneNode(shader, location, scale, model) { m_Shader = shader;
	}

	Actor(Model* model, Shader* shader = DefaultActorShader(),
		glm::vec3 location = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f))
		: SceneNode(shader, location, scale, model) {
		m_Shader = shader;
	}

private:

	Shader* m_Shader;

	inline static Shader* DefaultActorShader() { 
		return Shader::CreateShaderFromPath("Engine/Shaders/basic2d.vs", "Engine/Shaders/basic2d.fs"); 
	}
};