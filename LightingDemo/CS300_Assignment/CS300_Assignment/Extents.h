#pragma once
#include "float.h"

/*Extents of a models x, y, or z values*/
struct Extents
{
	Extents() : min(10000000), max(-100000000) {}
	float min;
	float max;
};