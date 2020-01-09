#pragma once

#include "Mesh.h"

class CubeMesh : public Mesh
{
public:
	CubeMesh(Shader* shader) : Mesh(CreateVertexStruct(), CubeIndices) {};

private:

	std::vector<Vertex> VertexStruct;

	float CubeVertices[4 * 6 * 3] = {
		-0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f, 

		-0.5f, -0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f,  0.5f,  

		 0.5f,  0.5f,  0.5f,    
		 0.5f,  0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,   
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f, -0.5f,   
		 0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f
	};

	float CubeNormals[4 * 6 * 3] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f,  1.0f,
		0.0f, 0.0f,  1.0f,
		0.0f, 0.0f,  1.0f,
		0.0f, 0.0f,  1.0f,

		-1.0f, 0.0f,  0.0f,
		-1.0f, 0.0f,  0.0f,
		-1.0f, 0.0f,  0.0f,
		-1.0f, 0.0f,  0.0f,

		1.0f, 0.0f,  0.0f,
		1.0f, 0.0f,  0.0f,
		1.0f, 0.0f,  0.0f,
		1.0f, 0.0f,  0.0f,

		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,

		0.0f, 1.0f,  0.0f,
		0.0f, 1.0f,  0.0f,
		0.0f, 1.0f,  0.0f,
		0.0f, 1.0f,  0.0f
	};

	std::vector<unsigned int> CubeIndices = {
		0,  1,  2,  0,  2,  3,  
		4,  5,  6,  4,  6,  7,   
		8,  9,  10, 8,  10, 11,
		12, 13, 14, 12, 14, 15, 
		16, 17, 18, 16, 18, 19,  
		20, 21, 22, 20, 22, 23 };

	std::vector<Vertex> CreateVertexStruct();

	Vertex MakeVertex(glm::vec3 Vertices, glm::vec3 Normals);
};