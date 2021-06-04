#include "bsp_dungeon_gen.h"

BSPDungeonGenerator::BSPDungeonGenerator(Rectangle startArea)
	: startDungeonArea(startArea)
{
}

void BSPDungeonGenerator::GenerateSubparts(unsigned int iterations, float minAmplitude, float maxAmplitude)
{
	subparts.clear();

	for (int i = 0; i < iterations; i++)
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

void BSPDungeonGenerator::DrawSubparts(unsigned int thickness)
{
	for (auto& rec : subparts)
	{
		DrawRectangleLinesEx(rec, thickness, WHITE);
	}
}

int BSPDungeonGenerator::GetSubpartsCount()
{
	return subparts.size();
}
