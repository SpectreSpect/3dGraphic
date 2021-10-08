#pragma once
#include "ViewPort.h"
#include "VertexBuffer.h"
#include "../Math/mymath.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "SwapChain.h"
#include "Input_Layout.h"

#define NONE -23564343
class Device
{
public:
	Device(size_t videoHashMemorySize);
	void SetVertexBuffer(VertexBuffer* vertexBuffer);
	void SetVertexShader(VertexShader* vertexShader);
	void SetPixelShader(PixelShader* pixelShader);
	void SetViewPort(ViewPort viewPort);
	void SetInputLayout(Input_Layout* inputLayout);
	void CreateSwapChain(SwapChain** swapChain, int backBuffersCount, float2 backBuffersSize, DXGI_FORMAT format);
	void Draw(int verticesCount);
	SwapChain** swapChain;
private:
	VertexShader* vs;
	PixelShader* ps;
	ViewPort viewPort;
	VertexBuffer* vertexBuffer;
	VertexBuffer changedVertexBuffer;
	Input_Layout* inputLayout;

	void* backBuffers;
	void* videoHashMemory;
	size_t videoHashMemorySize;


	//float4 DrawTriangle(float2 pixelPos, float3 A, float3 B, float3 C);
	float4 DrawTriangle(float2 pixelPos, int firstVertexID);
	void ExecuteVertexShader();
	//float4 DrawTriangle(float2 pixelPos);
};

