#pragma once

#include "Engine/Rendering/Renderer.h"
#include "Application/Window.h"
#include "Application/ObjectPicker.h"
#include "Logging/Logger.h"
#include "Engine/Rendering/Shader.h"
#include "Events/Event.h"
#include "Engine/Camera/ArcballCamera.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "GUI/GUILayer.h"
#include "Engine/Lighting/Light.h"
#include "Engine/Lighting/PointLight.h"
#include "Engine/Objects/Actor.h"
#include "Engine/Objects/Scene.h"
#include "Engine/Objects/Skybox.h"

class GUILayer;

class Application
{
public:
	Application();
	~Application();

	void Run();
	void OnEvent(const Event& e);
	void PollEvents();

	inline Window* GetWindow() { return m_Window; }
	inline Shader* GetShader() { return m_Shader; }
	inline Scene* GetScene() { return m_Scene; }
	inline Renderer* GetRenderer() { return m_Renderer; }
	inline ArcballCamera* GetCamera() { return m_Camera; }

	inline void SetViewPortFocused(bool focused) { m_ViewPortFocused = focused; }

private:

	Window* m_Window;
	Renderer* m_Renderer ;
	Shader* m_Shader;
	ArcballCamera* m_Camera;
	Scene* m_Scene;
	GUILayer* m_GuiLayer;
	ObjectPicker* m_ObjectPicker;

	glm::mat4 m_ModelMat = glm::mat4(1.0f);
	glm::mat4 m_ProjMat = glm::mat4(1.0f);
	glm::mat4 m_ViewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0., 0., 0.), glm::vec3(0., 1., 0.));

	float m_PrevTime = 0;
	float m_DeltaTime = 0;

	bool m_ViewPortFocused = false;
};