#pragma once
#include <iostream>
#include <vector>
#include "../Math/mymath.h"
#include "Device.h"
class Mesh
{
public:
	struct MeshVertex
	{
		float3 position;
		float3 normal;
	};
	struct StructuredBuffer
	{
		Matrix3x3 rotationMatrix;
		float3 objPos;
	};
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;

	Matrix3x3 rotationMatrix;
	float3 position;

	Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices);
	void Draw(Device* device, VertexShader* vertexShader, PixelShader* pixelShader);
private:
	StructuredBuffer structuredBuffer;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	void setupMesh();
};

