#pragma once
#include "Engine/Objects/Mesh.h"

class Framebuffer
{
public:
	Framebuffer(Framebuffer* inputFramebuffer = nullptr);
	~Framebuffer();

	void Bind();
	void Unbind();

	void AttachTexture(unsigned int textureId, GLenum attachmentType, GLenum textureTarget);
	void AttachRenderbuffer(unsigned int renderbufferId, GLenum attachmentType);
	unsigned int GetId() { return Id; }

private:
	unsigned int Id;

};

