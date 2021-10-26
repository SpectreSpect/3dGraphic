#pragma once
class IndexBuffer
{
public:
	const unsigned int* buffer;
	int indexesCount;

	//void Init(unsigned int* buffer, size_t indexesCount);
	void Init(const unsigned int* buffer, int indexesCount);

};

