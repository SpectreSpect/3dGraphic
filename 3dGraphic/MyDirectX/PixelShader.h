#pragma once
#include "..//Math/mymath.h"
#include <iostream>
class PixelShader
{
public:
	virtual float4 Execute(void* input) = 0;
};

