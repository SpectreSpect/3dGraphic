#include "TestVS.h"

void* TestVS::Execute(void* vertexBuffer, int vertexOffeset)
{
	return main(*(Input*)((char*)vertexBuffer + vertexOffeset));
}


TestVS::Output* TestVS::main(Input input)
{
	Output output;
	output.pos = float3{ input.pos.x, input.pos.y, input.pos.z };
	output.color = input.color;

	return &output;
}




