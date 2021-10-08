#include "SwapChain.h"

void SwapChain::CreateBackBuffers(float2 size, DXGI_FORMAT format, int backBuffersCount)
{
	backBuffers = new Texture2D[backBuffersCount];
	
	for (int i = 0; i < backBuffersCount; i++)
		backBuffers[i].InitTexture2D(format, size);

	frontBufferId = 0;
	currentBackBufferId = 1;
}

void SwapChain::Prevent()
{
	std::swap(frontBufferId, currentBackBufferId);
}
