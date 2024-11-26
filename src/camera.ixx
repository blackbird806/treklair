module;
#include <SDL3/SDL.h>

export module treklair:camera;

import :vec2;

export inline float gScaleFactor;

export struct Camera
{
	Vec2 pos;
	float speed = 100;

	int worldToScreenX(int x) const noexcept
	{
		return x - pos.x;
	}

	int worldToScreenY(int y) const noexcept
	{
		return y - pos.y;
	}

	int screenToWorldX(int x) const noexcept
	{
		return (x / gScaleFactor) + pos.x;
	}

	int screenToWorldY(int y) const noexcept
	{
		return (y / gScaleFactor) + pos.y;
	}
};