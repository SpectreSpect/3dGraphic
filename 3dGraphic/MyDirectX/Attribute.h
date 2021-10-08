#pragma once
#include "DXGI_FORMAT.h"
struct Attribute
{
	char* semantic;
	DXGI_FORMAT format;
	int offsetInVertex;
};