#pragma once

#include "Application/Application.h"

class Application;
class SceneNode;

enum class GUIStyle
{
	Darcula, Photoshop
};

class GUILayer
{
public:
	GUILayer(Window* window, Application* app);

	void Attach();
	void Detach();
	void Begin();
	void End();

	void SetStyle(GUIStyle style);

	inline float* GetColor() { return color; }

private:
	Window* m_Window;
	Application* m_App;

	void DarculaStyle();
	void PhotoshopStyle();

	void DisplayHierarchy(std::vector<SceneNode*>* objects);

	float color[4] = { 0.2f,0.2f,0.2f,0.2f };
};