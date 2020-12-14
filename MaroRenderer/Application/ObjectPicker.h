#pragma once

#include "Engine/Objects/SceneNode.h"
#include "Application/Events/MouseEvent.h"
#include "Engine/Objects/Scene.h"
#include "Engine/Rendering/Framebuffer.h"
#include <map>

class ObjectPicker 
{
public:
	ObjectPicker(Scene* scene);
	SceneNode* PickObject(MouseReleasedEvent* event);

	void RenderImage();

private:
	void RenderUniqueColors();

	Framebuffer m_Framebuffer;
	Scene* m_Scene;
	unsigned int m_ColorTexture;

	std::map<int, SceneNode*> m_ColorLookup;

};