#include "IndexBuffer.h"

void IndexBuffer::Init(const unsigned int* buffer, int indexesCount)
{
	this->buffer = buffer;
	this->indexesCount = indexesCount;
}
