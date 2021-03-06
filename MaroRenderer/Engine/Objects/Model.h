#pragma once

#include <string>
#include <vector>
#include "Mesh.h"
#include "Engine/Shader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Logging/Logger.h"

class Model
{
public:

	Model(std::string& path) { LoadModel(path); }
	Model(std::string& path, std::string& texturePath, const std::string& normalPath = "") 
	{ 
		m_NormalPath = normalPath;
		m_DiffusePath = texturePath;
		LoadModel(path); 
	}

	void Draw(Shader* shader);

	unsigned int AddTexture(std::string& texturePath, TextureType type);

	static unsigned int TextureFromFile(const char* path);

private:
	void LoadModel(std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Mesh*> m_Meshes;
	std::string m_Directory;

	std::string m_DiffusePath;
	std::string m_NormalPath;

	

};