#pragma once

#include "Engine/Objects/SceneNode.h"
#include "Engine/Rendering/Shader.h"

class Actor : public SceneNode {

public:

	Actor(Shader* shader = DefaultActorShader(), 
		glm::vec3 location = glm::vec3(0.0f), 
		glm::vec3 scale = glm::vec3(1.0f), Model* model = NULL) 
		: SceneNode(shader, location, scale, model) { 
		m_Shader = shader;
		m_Color = glm::vec3(0.2f, 0.2f, 0.2f);

		std::string label = "Actor " + std::to_string(GetUUID());
		SetLabel(label);
	}

	Actor(Model* model, Shader* shader = DefaultActorShader(),
		glm::vec3 location = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f))
		: SceneNode(shader, location, scale, model) {
		m_Shader = shader;
		m_Color = glm::vec3(0.2f, 0.2f, 0.2f);

		std::string label = "Actor " + std::to_string(GetUUID());
		SetLabel(label);
	}

	~Actor(){}

	inline glm::vec3 GetColor() { return m_Color; }
	inline void SetColor(glm::vec3 color) { m_Color = color; }

	virtual bool HasMaterial() override { return true; }

private:

	Shader* m_Shader;
	glm::vec3 m_Color;

	inline static Shader* DefaultActorShader() { 
		return Shader::CreateShaderFromPath("Engine/Shaders/pbr.vs", "Engine/Shaders/pbr.fs"); 
	}
};