#pragma once
class IndexBuffer
{
public:
	const unsigned int* buffer;
	int indexesCount;

	void Init(const unsigned int* buffer, int indexesCount);

};

