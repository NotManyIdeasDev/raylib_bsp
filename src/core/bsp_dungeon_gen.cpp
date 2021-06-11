#include "bsp_dungeon_gen.h"

BSPDungeonGenerator::BSPDungeonGenerator(Rectangle startArea)
	: startDungeonArea(startArea)
{

}

void BSPDungeonGenerator::GenerateSubparts(unsigned int iterations, float minAmplitude, float maxAmplitude)
{
	subparts.clear();

	for (int i = 0; i < iterations + 1; i++)
	{
		if (i == 0)
		{
			subparts.push_back(startDungeonArea);
		}
		else
		{
			int currentSubparts = subparts.size();

			for (int j = 0; j < currentSubparts; j++)
			{
				bool direction = subparts[j].width * RNG::GenerateNumber<float>(0.7f, 1.3f) > subparts[j].height * RNG::GenerateNumber<float>(0.7f, 1.3f) ? 1 : 0; //0 = horizontal, 1 = vertical;

				if (direction == 0)
				{
					unsigned int randomY = RNG::GenerateNumber<int>(subparts[j].height * minAmplitude, subparts[j].height * maxAmplitude);
					Rectangle first = { subparts[j].x, subparts[j].y, subparts[j].width, randomY };
					Rectangle second = { subparts[j].x, (float)subparts[j].y + randomY, subparts[j].width, (float)subparts[j].height -  randomY };

					subparts.erase(subparts.begin() + j);
					subparts.push_back(first);
					subparts.push_back(second);
				}
				else
				{
					unsigned int randomX = RNG::GenerateNumber<int>(subparts[j].width * minAmplitude, subparts[j].width * maxAmplitude);
					Rectangle first = { subparts[j].x, subparts[j].y, randomX, subparts[j].height };
					Rectangle second = { subparts[j].x + randomX, subparts[j].y, (float)subparts[j].width - randomX, subparts[j].height };

					subparts.erase(subparts.begin() + j);
					subparts.push_back(first);
					subparts.push_back(second);
				}
			}
		}
	}
}

void BSPDungeonGenerator::DrawSubparts(unsigned int thickness, Color color)
{
	for (auto& rec : subparts)
	{
		DrawRectangleLinesEx(rec, thickness, color);
	}
}

void BSPDungeonGenerator::GenerateRooms(float minSizePercentage, float maxSizePercentage, int multiple)
{
	rooms.clear();

	for (int i = 0; i < subparts.size(); i++)
	{
		Room room = 
		{ 
			subparts[i].x + RNG::GenerateNumber<int>(0, subparts[i].width * 0.3f),
			subparts[i].y + RNG::GenerateNumber<int>(0, subparts[i].height * 0.3f),
			subparts[i].width * RNG::GenerateNumber<float>(minSizePercentage, maxSizePercentage),
			subparts[i].height * RNG::GenerateNumber<float>(minSizePercentage, maxSizePercentage)
		};

		room.transform.width = MathUtils::Round(room.transform.width, multiple);
		room.transform.height = MathUtils::Round(room.transform.height, multiple);

		rooms.push_back(room);
	}
}

void BSPDungeonGenerator::CutRooms(int maxRoomAmount, bool preferBiggerRooms, int minRoomX, int minRoomY)
{
	int currentRoomAmount = rooms.size();

	std::sort(rooms.begin(), rooms.end(),
		[](const Room& a, const Room& b) -> bool 
	{
		return a.transform.width + a.transform.height > b.transform.width + b.transform.height;
	});

	if (currentRoomAmount > maxRoomAmount)
	{
		for (int i = currentRoomAmount; i > maxRoomAmount; i--)
		{
			if (preferBiggerRooms)
			{
				int half = rooms.size() / 2;

				if (rooms.size() - 1 >= half)
				{
					int random = RNG::GenerateNumber<int>(half, rooms.size() - 1);
					rooms.erase(rooms.begin() + random);
				}
			}
			else
			{
				int half = rooms.size() / 2;
				int random = 0;

				if (half >= 1)
					random = RNG::GenerateNumber<int>(0, half - 1);
				else
					random = RNG::GenerateNumber<int>(0, 1);

				rooms.erase(rooms.begin() + random);
			}
		}
	}

	std::vector<Room> roomCopy = GetRoomsReference();
	std::vector<Room>::const_iterator iter = roomCopy.begin();

	while (iter != roomCopy.end())
	{
		if (iter->transform.width < minRoomX || iter->transform.height < minRoomY)
			iter = roomCopy.erase(iter);
		else
			++iter;
	}

	rooms = roomCopy;

	int finalRoomCount = rooms.size();

	if ((finalRoomCount - 1) > 0)
	{
		int r_index = RNG::GenerateNumber<int>(0, finalRoomCount - 1);
		rooms[r_index].flags = RoomFlags::STARTING;
		startingIndex = r_index;
	}
}

void BSPDungeonGenerator::DrawRooms(Color color)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].flags == RoomFlags::STARTING)
		{
			DrawRectangleRec(rooms[i].transform, RED);
		}
		else
			DrawRectangleRec(rooms[i].transform, color);

		//if (i > 0)
		//{
		//	DrawLineEx(
		//		{ rooms[i].transform.width / 2 + rooms[i].transform.x, rooms[i].transform.height / 2 + rooms[i].transform.y },
		//		{ rooms[i - 1].transform.width / 2 + rooms[i - 1].transform.x, rooms[i - 1].transform.height / 2 + rooms[i - 1].transform.y },
		//		2,
		//		DARKPURPLE);
		//}
		//else
		//{
		//	DrawLineEx(
		//		{ rooms[0].transform.width / 2 + rooms[0].transform.x, rooms[0].transform.height / 2 + rooms[0].transform.y },
		//		{ rooms[1].transform.width / 2 + rooms[1].transform.x, rooms[1].transform.height / 2 + rooms[1].transform.y },
		//		2,
		//		DARKPURPLE);
		//}
	}
}