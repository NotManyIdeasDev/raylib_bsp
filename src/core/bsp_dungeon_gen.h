#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <math.h>
#include "random_gen.h"
#include "room.h"
#include "math_utils.h"
#include <iostream>
#include <algorithm>

class BSPDungeonGenerator
{
public:

	BSPDungeonGenerator(Rectangle startArea);

	void GenerateSubparts(unsigned int iterations, float minAmplitude, float maxAmplitude);
	void DrawSubparts(unsigned int thickness, Color color = GREEN);

	void GenerateRooms(float minSizePercentage, float maxSizePercentage, int multiple = 0);
	void CutRooms(int maxRoomAmount, bool preferBiggerRooms, int minRoomX, int minRoomY);

	void DrawRooms(Color color = WHITE);

	inline void SetStartArea(Rectangle rec) { startDungeonArea = rec; }

	inline int GetSubpartsCount() { return subparts.size(); }
	inline int GetRoomsCount() { return rooms.size(); }
	inline int GetDiscardedRoomsCount() { return subparts.size() - rooms.size(); }

	inline std::vector<Room> GetRooms() { return rooms; }


private:
	Rectangle startDungeonArea;
	std::vector<Rectangle> subparts;
	std::vector<Room> rooms;

	int startingIndex;

	inline const std::vector<Room>& GetRoomsReference() { return rooms; }

	int discardedRooms = 0;
};