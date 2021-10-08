#pragma once
#include "DXGI_FORMAT.h"
#include "..//Math/float2.h"
class Texture2D
{
public:
	void InitTexture2D(int format, float2 size);
	void** texture2D;
	int format;
	float2 size;
};

