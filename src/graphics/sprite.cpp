#include "sprite.h"

std::string Sprite::baseFilePath = "";
std::map<std::string, std::unique_ptr<Texture>> Sprite::loadedTextures;

void Sprite::SetBaseFilePath(const std::string& base)
{
	baseFilePath = base;
}

void Sprite::UnloadTexture(const std::string& filename)
{
	if (loadedTextures.find(filename) == loadedTextures.end())
		std::cout << "[ERROR] Failed to unload texture: \"" << filename << "\". " << "Texture not found." << std::endl;
	else
	{
		loadedTextures.erase(loadedTextures.find(filename));
	}
}

void Sprite::UnloadTextures()
{
	loadedTextures.clear();
}

Sprite::Sprite(const std::string& filename)
{
	if (loadedTextures.find(filename) != loadedTextures.end())
		texture = *loadedTextures[filename];
	else
	{
		texture = LoadTexture((baseFilePath + filename).c_str());
		loadedTextures[filename] = std::make_unique<Texture>(texture);
	}

	transform.position = { 0, 0 };
	transform.rotation = 0.0f;
	transform.scale = { 1.0f, 1.0f };

	width = texture.width;
	height = texture.height;

	origin = { (int)width / 2, (int)height / 2 };
	color = WHITE;
}

void Sprite::Draw()
{
	Rectangle dest = { transform.position.x, transform.position.y,(float)width * transform.scale.x, (float)height * transform.scale.y };
	DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, dest, origin, transform.rotation, color);
}
