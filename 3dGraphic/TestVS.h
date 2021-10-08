#pragma once
#include "MyDirectX/VertexShader.h"




class TestVS : public VertexShader
{
public:
	struct Input
	{
		float3 pos;
		float4 color;
		float3 normal;
	};
	struct Output
	{
		float3 pos;
		float4 color;
		float3 normal;
	};
	//Input input;
	virtual void* Execute(void* vertexBuffer, int vertexOffeset) override;
	Output* main(Input input);

};

