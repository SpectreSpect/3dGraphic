#pragma once
#include "ViewPort.h"
#include "VertexBuffer.h"
#include "../Math/mymath.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "SwapChain.h"
#include "Input_Layout.h"

#define NONE -228
#define BIG_NUM 999999999999
class Device
{
public:
	struct VERTEX
	{
		float3 pos;
		float4 color;
	};
	Device(size_t videoHashMemorySize);
	void SetVertexBuffer(VertexBuffer* vertexBuffer);
	void SetVertexShader(VertexShader* vertexShader);
	void SetPixelShader(PixelShader* pixelShader);
	void SetViewPort(ViewPort viewPort);
	void SetInputLayout(Input_Layout* inputLayout);
	void CreateSwapChain(SwapChain** swapChain, int backBuffersCount, float2 backBuffersSize, DXGI_FORMAT format);
	void SetDepthStencil(Texture2D* depthStencil);
	void Draw(int verticesCount);
	void ClearBuffer(float4 color, Texture2D* texture);
	SwapChain** swapChain;
private:
	VertexShader* vs;
	PixelShader* ps;
	ViewPort viewPort;
	VertexBuffer* vertexBuffer;
	VertexBuffer changedVertexBuffer;
	Input_Layout* inputLayout;
	Texture2D* depthStencil;

	void* backBuffers;
	void* videoHashMemory;
	size_t videoHashMemorySize;
	float3 GetUVZ(float2 pixelPos, int firstVertexID);
	float4 DrawTriangle(float2 pixelPos, int firstVertexID);
};

