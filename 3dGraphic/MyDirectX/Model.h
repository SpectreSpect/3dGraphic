#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

class Model
{
public:
	
	Model(const char* path);
	void Draw(Device* device, VertexShader* vertexShader, PixelShader* pixelShader);
	Matrix3x3 rotationMatrix;
	float3 position;
private:

	std::vector<Mesh> meshes;

	void LoadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

