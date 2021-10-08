#include "Texture2D.h"

void Texture2D::InitTexture2D(int format, float2 size)
{
	this->format = format;
	this->size = size;
	texture2D = new void*[size.x];
	for (int j = 0; j < size.x; j++)
		texture2D[j] = new char[size.y * format];
}
