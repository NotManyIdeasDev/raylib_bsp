#pragma once
#include <raylib.h>
#include <string>
#include "rlImGui.h"

class Window
{
public:

	Window(unsigned int width, unsigned int height, const std::string& title, bool vsync, unsigned int targetFPS = 60);
	void SetWindowFPS(unsigned int fps);
	void SetFlag(ConfigFlags flag);
	void Close();

public:
	unsigned int width;
	unsigned int height;
	std::string title;
	unsigned int targetFPS;
	bool isVsynced;
};