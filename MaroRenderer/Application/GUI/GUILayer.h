#pragma once

#include "Application/Window.h"

enum class GUIStyle
{
	Darcula, Photoshop
};

class GUILayer
{
public:
	GUILayer(Window* window);

	void Attach();
	void Detach();
	void Begin();
	void End();

	void SetStyle(GUIStyle style);

	inline float* GetColor() { return color; }

private:
	Window* m_Window;

	void DarculaStyle();
	void PhotoshopStyle();

	float color[4] = { 1.0f,1.0f,1.0f,1.0f };
};