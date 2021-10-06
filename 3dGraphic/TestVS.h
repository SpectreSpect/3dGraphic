#pragma once
#include "MyDirectX/VertexShader.h"

struct Input
{
	float3 pos;
	float4 color;
};

struct Output
{
	float3 pos;
	float4 color;
};

class TestVS : public VertexShader
{
public:
	Input input;
	Output* main(Input input);

	virtual void* Execute(void* vertexBuffer, int vertexOffeset) override;

};

