#include "IndexBuffer.h"

void IndexBuffer::Init(const unsigned int* buffer, int indexesCount)
{
	this->buffer = buffer;
	this->indexesCount = indexesCount;
}

//void IndexBuffer::Init(unsigned int* buffer, size_t indexesCount)
//{
//	this->buffer = buffer;
//	this->indexesCount = indexesCount;
//}

