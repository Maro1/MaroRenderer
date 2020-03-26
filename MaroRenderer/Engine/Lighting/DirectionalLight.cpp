#include "DirectionalLight.h"

void DirectionalLight::InitLight()
{
	
	m_Shader->Use();

	m_Shader->SetFloat3("direction", glm::vec3(1.0f, 1.0f, 0.3f));

	m_Shader->SetFloat3("directionLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	m_Shader->SetFloat3("directionLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	m_Shader->SetFloat3("directionLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	m_Shader->SetFloat3("directionLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
}
