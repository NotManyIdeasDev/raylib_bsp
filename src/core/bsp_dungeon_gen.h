#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "random_gen.h"
#include <iostream>
#include <algorithm>

class BSPDungeonGenerator
{
public:

	BSPDungeonGenerator(Rectangle startArea);

	void GenerateSubparts(unsigned int iterations, float minAmplitude, float maxAmplitude);
	void DrawSubparts(unsigned int thickness, Color color = GREEN);

	void GenerateRooms(float minSizePercentage, float maxSizePercentage);
	void CutRooms(int maxRoomAmount, bool preferBiggerRooms, int minRoomX, int minRoomY);
	void DrawRooms(Color color = WHITE);

	inline int GetSubpartsCount() { return subparts.size(); }
	inline int GetRoomsCount() { return rooms.size(); }
	inline int GetDiscardedRoomsCount() { return subparts.size() - rooms.size(); }

	inline std::vector<Rectangle> GetRooms() { return rooms; }


private:
	Rectangle startDungeonArea;
	std::vector<Rectangle> subparts;
	std::vector<Rectangle> rooms;

	inline const std::vector<Rectangle>& GetRoomsReference() { return rooms; }

	int discardedRooms = 0;
};