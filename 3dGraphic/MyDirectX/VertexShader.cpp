#include "VertexShader.h"

void VertexShader::FillStruct(void* data)
{
	void* inputStruct = ((char*)this + sizeof(VertexShader));
	memcpy(inputStruct, data, vertexBuffer->vertexSize);
}
