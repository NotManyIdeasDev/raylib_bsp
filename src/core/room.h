#pragma once
#include <raylib.h>
#include <raymath.h>
#include <iostream>

enum class RoomFlags
{
	NONE = 0 << 0,
	STARTING  = 1 << 0,
	CONNECTED = 1 << 1,
};

inline RoomFlags operator| (RoomFlags a, RoomFlags b)
{
	return static_cast<RoomFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline RoomFlags operator^ (RoomFlags a, RoomFlags b)
{
	return static_cast<RoomFlags>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline RoomFlags operator& (RoomFlags a, RoomFlags b)
{
	return static_cast<RoomFlags>(static_cast<int>(a) & static_cast<int>(b));
}

class Room
{
public:

	Room()
	{
		transform = { 0, 0, 0, 0 };
		flags = RoomFlags::NONE;
	}

	Room(int x, int y, int width, int height)
	{
		transform = { (float)x, (float)y, (float)width, (float)height };
		flags = RoomFlags::NONE;
	}

	Room(float x, float y, float width, float height)
	{
		transform = { x, y, width, height };
		flags = RoomFlags::NONE;
	}

	Rectangle transform;
	RoomFlags flags;

	Vector2 GetCenter()
	{
		return { (float)transform.width / 2 + transform.x, (float)transform.height / 2 + transform.y };
	}


	inline bool operator== (const Room& other)
	{
		return transform == other.transform;
	}

	inline bool operator!= (const Room& other)
	{
		return transform != other.transform;
	}
};