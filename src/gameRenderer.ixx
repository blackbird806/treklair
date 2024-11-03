module;
#include <SDL3/SDL.h>
#include "vec2.h"

export module treklair:gameRenderer;

import :world;
import :globals;

export inline float gScaleFactor;

export struct Camera
{
	vec2 pos;
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

export struct GameRenderer
{
	void drawWorld(World const& world) const
	{
		for (int i = 0; auto const& tile : world.tiles)
		{
			int pos_x = (i % world.size_x) * tile_size;
			int pos_y = (i / world.size_x) * tile_size;
			SDL_FRect rect;
			rect.x = camera.worldToScreenX(pos_x);
			rect.y = camera.worldToScreenY(pos_y);
			rect.w = tile_size;
			rect.h = tile_size;
			SDL_RenderFillRect(sdl_renderer, &rect);
			i++;
		}
	}
	Camera camera;
};

