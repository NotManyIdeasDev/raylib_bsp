#pragma once
#include <string>
#include <raylib.h>
#include <raymath.h>
#include <imgui.h>
#include <sstream>
#include <iostream>

class CoreUtils
{
public:

	static ImVec4 HexToRGB(const std::string& hexString)
	{
		std::string copy = "";

		if (hexString[0] == '#')
			copy = hexString.substr(1, hexString.size() - 1);

		std::string red = copy.substr(0, 2);
		std::string green = copy.substr(2, 2);
		std::string blue = copy.substr(4, 2);

		return ImVec4(std::stoi(red, 0, 16)/255.0f, std::stoi(green, 0, 16)/255.0f, std::stoi(blue, 0, 16)/255.0f, 1.0f);
	}

	static ImVec4 GetRainbowColor()
	{
		float frequency = 3.0f;
		float red   = sin(frequency * GetTime() + 0) * 127 + 128;
		float green = sin(frequency * GetTime() + 2) * 127 + 128;
		float blue  = sin(frequency * GetTime() + 4) * 127 + 128;

		return ImVec4(red/255.0f, green/255.0f, blue/255.0f, 1.0f);
	}
};