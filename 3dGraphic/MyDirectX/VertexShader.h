#pragma once
#include "VertexBuffer.h"
#include "..//Math/mymath.h"
#include <iostream>
class VertexShader
{
public:
	void FillStruct(void* data);
	virtual void* Execute(void* vertexBuffer, int vertexOffeset) = 0;
	VertexBuffer* vertexBuffer;
};

