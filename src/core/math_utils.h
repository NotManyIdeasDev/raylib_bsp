#pragma once
#include "raymath.h"

class MathUtils
{
public:

	static int Round(int number, int multiple)
	{
		if (multiple == 0)
			return number;

		int remainder = abs(number) % multiple;
		if (remainder == 0)
			return number;

	    return number - remainder;
	}

};