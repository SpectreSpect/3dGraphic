#pragma once
#include "MyDirectX/PixelShader.h"
#include "Math/Matrix3x3.h"

class TestPS : public PixelShader
{
public:
	struct Input
	{
		float3 pos;
		float4 color;
		float3 normal;
	};
	struct StructuredBuffer
	{
		Matrix3x3 transformMatrix;
	};
	StructuredBuffer* structuredBuffer;
	virtual float4 Execute(void* input) override;
	float4 main(Input input);

	float length(float3 v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	float dot(float3 v1, float3 v2)
	{
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) / length(v1) * length(v2);
	}
	float3 normalize(float3 v)
	{
		return v / length(v);
	}
};

