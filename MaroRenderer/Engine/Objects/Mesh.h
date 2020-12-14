#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <vector>
#include "Engine/Rendering/Shader.h"
#include "Engine/Rendering/Renderer.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
};

enum class TextureType
{
	DIFFUSE, NORMAL, METALLIC, ROUGHNESS, IRRADIANCE, PREFILTER, BRDF
};

struct Texture
{
	unsigned int Id;
	TextureType Type;
};

class Mesh
{
public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader* shader);
	void DrawColor(int color, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos);

	void AddTexture(unsigned int tetureID, TextureType type);

protected:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;

	Shader* m_Shader;
	unsigned int VAO, VBO, EBO;
	void InitMesh();

};