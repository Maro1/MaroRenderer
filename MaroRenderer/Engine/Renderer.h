#pragma once

#include <glad/glad.h>
#include <vector>
#include "Shader.h"
#include "Application/Window.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"

class Renderer
{
public:

	Renderer(Window* window);

	void Clear();
	void InitFramebuffer();
	inline unsigned int GetFBOTexture() { return m_FBOTexture; }
	void StartRender();
	void StopRender();
	void UpdateFrameBuffer(int x, int y);

private:

	Window* m_Window;
	unsigned int m_FBOTexture;
	unsigned int m_Framebuffer;

};
