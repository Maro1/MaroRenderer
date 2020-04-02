#pragma once

#include <vector>
#include "Engine/Objects/SceneNode.h"
#include "Engine/Lighting/Light.h"
#include "Engine/Objects/Actor.h"
#include "Engine/Camera/ArcballCamera.h"
#include "Engine/Lighting/PointLight.h"

class Scene {
public:

	Scene(ArcballCamera* camera);
	void UpdateShaders();
	inline void SetCamera(ArcballCamera* camera) { m_Camera = camera; }
	void RotateLight(float time);
	void AddActor(Actor* node);
	void AddPointLight(PointLight* light);
	void Render();

	inline void AddDirectionalLight() { m_DirectionalLight = true; }
	inline void RemoveDirectionalLight() { m_DirectionalLight = false; }
	inline void ToggleDirectionalLight() { m_DirectionalLight = !m_DirectionalLight; }

	inline SceneNode* GetRoot() { return m_SceneRoot; }

private:

	bool m_DirectionalLight = true;
	std::vector<PointLight*> m_Lights;
	std::vector<Actor*> m_Actors;

	ArcballCamera* m_Camera;
	SceneNode* m_SceneRoot;

	glm::mat4 m_ModelMat = glm::mat4(1.0f);
	glm::mat4 m_ProjMat = glm::mat4(1.0f);
	glm::mat4 m_ViewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0., 0., 0.), glm::vec3(0., 1., 0.));
};