#pragma once
#include "MyDirectX/VertexShader.h"




class TestVS : public VertexShader
{
public:
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
	//Input input;
	virtual void* Execute(void* vertexBuffer, int vertexOffeset) override;
	Output* main(Input input);

};

