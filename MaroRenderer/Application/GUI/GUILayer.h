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

	void SetActiveNode(SceneNode* node) { m_ActiveNode = node; }

	void SetStyle(GUIStyle style);

	inline float* GetColor() { return color; }

private:
	Window* m_Window;
	Application* m_App;
	SceneNode* m_ActiveNode;

	int m_WindowHeight; 
	int m_WindowWidth;

	void DarculaStyle();
	void PhotoshopStyle();

	void DisplayHierarchy(std::vector<SceneNode*>* objects);
	void DisplayMainMenu();
	void DisplayTabs();
	void DisplayViewport();
	
	void ImportModel();
	void ImportTexture();

	float color[4] = { 0.2f,0.2f,0.2f,0.2f };
};