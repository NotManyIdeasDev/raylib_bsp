#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "random_gen.h"
#include <iostream>

class BSPDungeonGenerator
{
public:

	BSPDungeonGenerator(Rectangle startArea);

	void GenerateSubparts(unsigned int iterations, float minAmplitude, float maxAmplitude);
	void DrawSubparts(unsigned int thickness);

	int GetSubpartsCount();

private:
	Rectangle startDungeonArea;
	std::vector<Rectangle> subparts;
};