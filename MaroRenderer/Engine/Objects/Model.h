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
	Model(std::string& path, std::string& texturePath, std::string& textureFilename) 
	{ 
		TextureFromFile(textureFilename.c_str(), texturePath);
		LoadModel(path); 
	}

	void Draw(Shader* shader);

private:
	void LoadModel(std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	std::vector<Mesh> m_Meshes;
	std::string m_Directory;

	unsigned int TextureFromFile(const char* path, const std::string& directory);
	

};