#include "TestVS.h"

void* TestVS::Execute(void* vertexBuffer, int vertexOffeset)
{
	structuredBuffer = reinterpret_cast<StructuredBuffer*>(pVoidStructuredBuffer);
	return main(*(Input*)((char*)vertexBuffer + vertexOffeset));
}

TestVS::Output* TestVS::main(Input input)
{
	Output output;

	output.pos = (structuredBuffer->rotationMatrix * float3{ input.pos.x, input.pos.y, input.pos.z }) + structuredBuffer->objPos;
	output.color = input.color;
	output.normal = structuredBuffer->rotationMatrix * input.normal;

	return &output;
}






