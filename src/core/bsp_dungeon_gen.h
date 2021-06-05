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
	void DrawSubparts(unsigned int thickness, Color color = GREEN);

	void GenerateRooms(float minSizePercentage, float maxSizePercentage);
	void CutRooms(int maxRoomAmount);
	void DrawRooms(Color color = WHITE);

	int GetSubpartsCount();
	int GetRoomsCount();

private:
	Rectangle startDungeonArea;
	std::vector<Rectangle> subparts;
	std::vector<Rectangle> rooms;
};