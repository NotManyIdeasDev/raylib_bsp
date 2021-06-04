#include "window.h"

Window::Window(unsigned int width, unsigned int height, const std::string& title, bool vsync, unsigned int targetFPS)
	: width(width), height(height), title(title), isVsynced(vsync), targetFPS(targetFPS)
{
	if (vsync)
		SetConfigFlags(FLAG_VSYNC_HINT);
	else
		SetTargetFPS(targetFPS);

	InitWindow(width, height, title.c_str());

	if (width == 0 && height == 0)
	{
		this->width = GetMonitorWidth(0);
		this->height = GetMonitorHeight(0);
		ToggleFullscreen();
	}

	SetupRLImGui(true);
}

void Window::SetWindowFPS(unsigned int fps)
{
	SetTargetFPS(fps);
}

void Window::SetFlag(ConfigFlags flag)
{
	SetConfigFlags(flag);
}

void Window::Close()
{
	CloseWindow();
}
