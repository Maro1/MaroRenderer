#include "Scene.h"

Scene::Scene()
{
	m_Light = new Light(glm::vec3(0.0f, 1.0f, 2.0f));
	m_SceneRoot = new SceneNode();
}

void Scene::UpdateShaders()
{
	m_ViewMat = m_Camera->GetView();
	m_ProjMat = m_Camera->GetProjection();
	for (Actor* actor : m_Actors)
	{
		actor->GetShader()->Use();
		actor->GetShader()->SetMat4("model", actor->GetModelMatrix());
		actor->GetShader()->SetMat4("view", m_ViewMat);
		actor->GetShader()->SetMat4("projection", m_ProjMat);
		actor->GetShader()->SetFloat3("lightColor", glm::vec3(1.0f));
		actor->GetShader()->SetFloat3("lightPos", m_Light->GetLocation());
		actor->GetShader()->SetFloat3("viewPos", m_Camera->GetPosition());
		actor->GetShader()->SetFloat3("inColor", glm::vec3(0.2f, 0.2f, 0.2f));
	}
	m_Light->GetShader()->Use();
	m_Light->GetShader()->SetMat4("projection", m_ProjMat);
	m_Light->GetShader()->SetMat4("view", m_ViewMat);
	m_Light->GetShader()->SetMat4("model", m_Light->GetModelMatrix());

}

void Scene::RotateLight(float time)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, (float)log(time) * 0.005f, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));

	m_Light->SetLocation(transform * glm::vec4(m_Light->GetLocation(), 1.0f));
}

void Scene::AddActor(Actor* actor)
{
	m_SceneRoot->AddChild(actor);
	m_Actors.push_back(actor);
}

// Implement further later when using more than 1 light
void Scene::AddLight(Light* light)
{
	m_SceneRoot->AddChild(light);
}

void Scene::Render()
{
	m_SceneRoot->Update();
}

