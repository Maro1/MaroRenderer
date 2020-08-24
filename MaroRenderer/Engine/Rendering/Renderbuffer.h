#pragma once
#include "Engine/Objects/Mesh.h"

class Renderbuffer
{
public:
	Renderbuffer();
	~Renderbuffer();

	void Bind();
	void Unbind();
	void SetDimensions(unsigned int width, unsigned int height);
	
	unsigned int GetId() { return Id; }

private:
	unsigned int Id;

};