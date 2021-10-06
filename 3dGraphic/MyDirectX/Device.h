#pragma once
#include "ViewPort.h"
#include "VertexBuffer.h"
#include "../Math/mymath.h"
#include "VertexShader.h"
class Device
{
public:
	Device(size_t videoHashMemorySize);
	void SetVertexBuffer(VertexBuffer* vertexBuffer);
	void SetVertexShader(VertexShader* vertexShader);
	void SetPixelShader();
	void SetViewPort(ViewPort viewPort);
	void Draw(int verticesCount);
private:
	//PixelShader ps;
	VertexShader* vs;
	ViewPort viewPort;
	VertexBuffer* vertexBuffer;
	VertexBuffer changedVertexBuffer;
	void* videoHashMemory;
	size_t videoHashMemorySize;


	float4 DrawTriangle(float2 pixelPos, float3 A, float3 B, float3 C);
	void ExecuteVertexShader();
	//float4 DrawTriangle(float2 pixelPos);
};

