#pragma once
#include "MyDirectX/PixelShader.h"

class TestPS : public PixelShader
{
public:
	struct Input
	{
		float3 pos;
		float4 color;
	};
	virtual float4 Execute(void* input) override;
	float4 main(Input input);
};

