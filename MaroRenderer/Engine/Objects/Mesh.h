#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <vector>
#include "Engine/Shader.h"
#include "Engine/Renderer.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
};

// TODO: Make texture struct when neccessary

class Mesh
{
public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> m_Indices);
	void Draw(Shader* shader);

	// TODO: Implement using some library
	static Mesh MeshFromFile(const char* path);

protected:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	// TODO: Texture when neccessary

	Shader* m_Shader;
	unsigned int VAO, VBO, EBO;
	void InitMesh();

};