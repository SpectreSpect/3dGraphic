#include "TestVS.h"

void* TestVS::Execute(void* vertexBuffer, int vertexOffeset)
{
	return main(*(Input*)((char*)vertexBuffer + vertexOffeset));
}


Output* TestVS::main(Input input)
{
	Output output;
	output.pos = input.pos * 2;
	output.color = input.color;

	return &output;
}




