#pragma once
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <iostream>

#include "../core/transform.h"

class Sprite
{
public:
	static void SetBaseFilePath(const std::string& base);
	static void UnloadTexture(const std::string& filename);
	static void UnloadTextures();

	Sprite(const std::string& filename);
	void Draw();

public:
	Transform2D transform;
	unsigned int width;
	unsigned int height;
	Vector2Int origin;

	Color color;

private:
	Texture texture;

	static std::string baseFilePath;
	static std::map<std::string, std::unique_ptr<Texture>> loadedTextures;
};