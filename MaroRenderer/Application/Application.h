#pragma once

#include "Engine/Renderer.h"
#include "Application/Window.h"
#include "Logging/Logger.h"
#include "Engine/Shader.h"
#include "Events/Event.h"
#include "Engine/Camera/ArcballCamera.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "GUI/GUILayer.h"
#include "Engine/Lighting/Light.h"
#include "Engine/Objects/Actor.h"
#include "Engine/Objects/Scene.h"

class Application
{
public:
	Application();
	~Application();

	void Run();
	void OnEvent(const Event& e);

	inline Window* GetWindow() { return m_Window; }
	inline Shader* GetShader() { return m_Shader; }

private:

	Window* m_Window;
	Renderer* m_Renderer;
	Shader* m_Shader;
	ArcballCamera* m_Camera;

	glm::mat4 m_ModelMat = glm::mat4(1.0f);
	glm::mat4 m_ProjMat = glm::mat4(1.0f);
	glm::mat4 m_ViewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0., 0., 0.), glm::vec3(0., 1., 0.));
};