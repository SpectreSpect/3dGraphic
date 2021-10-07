#pragma once
#include "DXGI_FORMAT.h"
#include "..//Math/float2.h"
class Texture2D
{
public:
	void** texture2D;
	int format;
	float2 size;
};

