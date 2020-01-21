#pragma once

#include <vector>
#include "Engine/Objects/SceneNode.h"
#include "Engine/Objects/Light.h"
#include "Engine/Objects/Actor.h"
#include "Engine/Camera/ArcballCamera.h"

class Scene {
public:

	Scene();
	void UpdateShaders();
	inline void SetCamera(ArcballCamera* camera) { m_Camera = camera; }
	void RotateLight(float time);
	void AddActor(Actor* node);
	void AddLight(Light* light);
	void Render();

private:

	// Only one light currently, add more later
	Light* m_Light;
	std::vector<Actor*> m_Actors;

	ArcballCamera* m_Camera;
	SceneNode* m_SceneRoot;

	glm::mat4 m_ModelMat = glm::mat4(1.0f);
	glm::mat4 m_ProjMat = glm::mat4(1.0f);
	glm::mat4 m_ViewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0., 0., 0.), glm::vec3(0., 1., 0.));
};