#include "SwapChain.h"

void SwapChain::CreateBackBuffers(float2 size, DXGI_FORMAT format, int backBuffersCount)
{
	backBuffers = new Texture2D[backBuffersCount];
	
	for (int i = 0; i < backBuffersCount; i++)
	{
		backBuffers[i].format = format;
		backBuffers[i].size = size;
		
		backBuffers[i].texture2D = new void*[size.x];
		for (int j = 0; j < size.x; j++)
			backBuffers[i].texture2D[j] = new char[size.y * format];
	}
	frontBufferId = 0;
	currentBackBufferId = 1;
}

void SwapChain::Prevent()
{
	std::swap(frontBufferId, currentBackBufferId);
}
