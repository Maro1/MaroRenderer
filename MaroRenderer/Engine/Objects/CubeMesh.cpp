#include "CubeMesh.h"
#include <iostream>

std::vector<Vertex> CubeMesh::CreateVertexStruct()
{
	/*for (int i = 0; i < 69; i++)
	{
		if (i % 3 == 0)
		{
			Vertex vertex = MakeVertex(glm::vec3(CubeVertices[i], CubeVertices[i + 1], CubeVertices[i + 2]), 
				glm::vec3(CubeNormals[i], CubeNormals[i + 1], CubeNormals[i + 2]));
			VertexStruct.push_back(vertex);
		}
	}*/
	return VertexStruct;
}

Vertex CubeMesh::MakeVertex(glm::vec3 Vertices, glm::vec3 Normals)
{
	Vertex v = { Vertices, Normals };
	return v;
}
