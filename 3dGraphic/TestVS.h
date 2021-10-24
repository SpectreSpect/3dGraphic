#pragma once
#include "MyDirectX/VertexShader.h"
#include "Math/Matrix3x3.h"



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
	struct StructuredBuffer
	{
		Matrix3x3 rotationMatrix;
		float3 objPos;
	};
	StructuredBuffer* structuredBuffer;
	//Input input;
	virtual void* Execute(void* vertexBuffer, int vertexOffeset) override;
	Output* main(Input input);

private:
	void rotOx(Matrix3x3* matrix, float alpha);
	void rotOy(Matrix3x3* matrix, float alpha);
	void rotOz(Matrix3x3* matrix, float alpha);
	//void rot(float3* vec, float alpha);
};

