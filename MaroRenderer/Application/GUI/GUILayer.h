#pragma once

#include "Application/Application.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

class Application;
class SceneNode;
enum class TextureType;

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


private:
	Window* m_Window;
	Application* m_App;
	SceneNode* m_ActiveNode = nullptr;

	unsigned int m_CheckerboardID;

	int m_WindowHeight; 
	int m_WindowWidth;

	ImVec2 m_ViewPortRegionMin;
	ImVec2 m_ViewPortRegionMax;

	void DarculaStyle();
	void PhotoshopStyle();

	void DisplayHierarchy(std::vector<SceneNode*>* objects);
	void DisplayMainMenu();
	void DisplayTabs();
	void DisplayViewport();
	void DisplayConsole();

	void CreateDockspace();
	
	void ImportModel();
	unsigned int ImportTexture(TextureType type);
};