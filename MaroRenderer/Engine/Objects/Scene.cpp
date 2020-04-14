#include "Scene.h"

Scene::Scene(ArcballCamera* camera)
{
	m_SceneRoot = new SceneNode();
	m_Camera = camera;
}

void Scene::UpdateShaders()
{
	m_ViewMat = m_Camera->GetView();
	m_ProjMat = m_Camera->GetProjection();
	for (SceneNode* actor : m_Actors)
	{
		// Have to update all the children as well, maybe better to add all children to actors list
		for (auto child : actor->GetChildren()) 
		{
			if (!std::count(m_Actors.begin(), m_Actors.end(), child)) 
			{
				m_Actors.push_back(child);
			}
		}
		actor->GetShader()->Use();
		actor->GetShader()->SetMat4("model", actor->GetModelMatrix());
		actor->GetShader()->SetMat4("view", m_ViewMat);
		actor->GetShader()->SetMat4("projection", m_ProjMat);
		actor->GetShader()->SetFloat3("viewPos", m_Camera->GetPosition());

		if (m_DirectionalLight)
		{
			actor->GetShader()->SetFloat3("directionLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
			actor->GetShader()->SetFloat3("directionLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			actor->GetShader()->SetFloat3("directionLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
			actor->GetShader()->SetFloat3("directionLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		}
		else
		{
			actor->GetShader()->SetFloat3("directionLight.direction", glm::vec3(0.0f));
			actor->GetShader()->SetFloat3("directionLight.ambient", glm::vec3(0.0f));
			actor->GetShader()->SetFloat3("directionLight.diffuse", glm::vec3(0.0f));
			actor->GetShader()->SetFloat3("directionLight.specular", glm::vec3(0.0f));
		}

		for (PointLight* light : m_Lights)
		{
			// Find better way to create string
			actor->GetShader()->SetFloat3(std::string("pointLights[").append(std::to_string(light->GetID())).append("].position").c_str(), light->GetLocation());
			actor->GetShader()->SetFloat(std::string("pointLights[").append(std::to_string(light->GetID())).append("].constant").c_str(), light->GetConstant());
			actor->GetShader()->SetFloat(std::string("pointLights[").append(std::to_string(light->GetID())).append("].linear").c_str(), light->GetLinear());
			actor->GetShader()->SetFloat(std::string("pointLights[").append(std::to_string(light->GetID())).append("].quadratic").c_str(), light->GetQuadradic());
			actor->GetShader()->SetFloat3(std::string("pointLights[").append(std::to_string(light->GetID())).append("].ambient").c_str(), glm::vec3(0.05f, 0.05f, 0.05f));
			actor->GetShader()->SetFloat3(std::string("pointLights[").append(std::to_string(light->GetID())).append("].diffuse").c_str(), glm::vec3(0.4f, 0.4f, 0.4f));
			actor->GetShader()->SetFloat3(std::string("pointLights[").append(std::to_string(light->GetID())).append("].specular").c_str(), glm::vec3(0.5f, 0.5f, 0.5f));
		}
	}
	for (PointLight* light : m_Lights)
	{
		light->GetShader()->Use();
		light->GetShader()->SetMat4("projection", m_ProjMat);
		light->GetShader()->SetMat4("view", m_ViewMat);
		light->GetShader()->SetMat4("model", light->GetModelMatrix());
	}

}

void Scene::RotateLight(float time)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, (float) (1 - exp(-time)) * 0.005f, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));

	for (PointLight* light : m_Lights) 
	{
		light->SetLocation(transform * glm::vec4(light->GetLocation(), 1.0f));
	}
}

void Scene::AddActor(Actor* actor)
{
	m_SceneRoot->AddChild(actor);
	m_Actors.push_back(actor);
}

void Scene::AddPointLight(PointLight* light)
{
	m_SceneRoot->AddChild(light);
	m_Lights.push_back(light);
}

void Scene::Render()
{
	m_SceneRoot->Update();
	for (PointLight* light : m_Lights)
	{
		light->GetModel()->Draw(light->GetShader());
	}
}

