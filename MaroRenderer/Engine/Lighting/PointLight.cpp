#include "PointLight.h"

void PointLight::InitLight()
{
	std::string s1 = "pointLights[";
	s1 += currentID + "].position";
	std::string s2 = "pointLights[";
	s2 += currentID + "].ambient";
	std::string s3 = "pointLights[";
	s3 += currentID + "].diffuse";
	std::string s4 = "pointLights[";
	s4 += currentID + "].specular";
	std::string s5 = "pointLights[";
	s5 += currentID + "].constant";
	std::string s6 = "pointLights[";
	s6 += currentID + "].linear";
	std::string s7 = "pointLights[";
	s7 += currentID + "].quadratic";
	m_Shader->Use();
	m_Shader->SetFloat3(s1.c_str(), m_Position);
	m_Shader->SetFloat3(s2.c_str(), glm::vec3(0.05f, 0.05f, 0.05f));
	m_Shader->SetFloat3(s3.c_str(), glm::vec3(0.8f, 0.8f, 0.8f));
	m_Shader->SetFloat3(s4.c_str(), glm::vec3(1.0f, 1.0f, 1.0f));
	m_Shader->SetFloat(s5.c_str(), 1.0f);
	m_Shader->SetFloat(s6.c_str(), 0.09f);
	m_Shader->SetFloat(s7.c_str(), 0.032f);
}
