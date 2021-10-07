#pragma once
#include "Texture2D.h"
#include <iostream>
class SwapChain
{
public:
	void CreateBackBuffers(float2 size, DXGI_FORMAT format, int backBuffersCount);
	void Prevent();
	Texture2D* backBuffers;
	int backBuffersCount;
	int frontBufferId;
	int currentBackBufferId;
};

