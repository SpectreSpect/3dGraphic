#include "Mesh.h"

Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
    setupMesh();
}

void Mesh::Draw(Device* device, VertexShader* vertexShader, PixelShader* pixelShader)
{
    structuredBuffer.objPos = position;
    structuredBuffer.rotationMatrix = rotationMatrix;
    device->SetStructuredBuffer(&structuredBuffer);
    device->SetVertexBuffer(&vertexBuffer);
    device->SetIndexBuffer(&indexBuffer);
    device->SetVertexShader(vertexShader);
    device->SetPixelShader(pixelShader);
    device->Draw(vertexBuffer.verticesCount);
}

void Mesh::setupMesh()
{
    vertexBuffer.buffer = &vertices[0];
    vertexBuffer.vertexSize = sizeof(MeshVertex);
    vertexBuffer.verticesCount = vertices.size();

    indexBuffer.Init(&indices[0], indices.size());
}
