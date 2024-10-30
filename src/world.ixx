module;

#include <SDL3/SDL.h>
#include <cassert>
#include <vector>
#include "vec2.h"

export module treklair:world;

export inline float gScaleFactor;

export struct Camera
{
	vec2 pos;
	float speed = 100;

	[[nodiscard]] int worldToScreenX(int x) const noexcept
	{
		return x - pos.x;
	}

	[[nodiscard]] int worldToScreenY(int y) const noexcept
	{
		return y - pos.y;
	}

	[[nodiscard]] int screenToWorldX(int x) const noexcept
	{
		return (x / gScaleFactor) + pos.x;
	}

	[[nodiscard]] int screenToWorldY(int y) const noexcept
	{
		return (y / gScaleFactor) + pos.y;
	}
};

export enum class TileType
{
	None,
	DebugWall
};

constexpr int tile_size = 32;

export struct Tile
{
	TileType type = TileType::None;
};

export struct World
{
	World(int size_x_, int size_y_)
	{
		assert(size_x_ > 0 && size_y_ > 0);
		size_x = size_x_;
		size_y = size_y_;
		tiles.resize(size_x * size_y);
	}

	void draw(SDL_Renderer* renderer, Camera const& camera) const
	{
		assert(renderer);
		for (int i = 0; auto const& tile : tiles)
		{
			int pos_x = (i % size_x) * tile_size;
			int pos_y = (i / size_y) * tile_size;
			SDL_FRect rect;
			rect.x = camera.worldToScreenX(pos_x);
			rect.y = camera.worldToScreenY(pos_y);
			rect.w = tile_size;
			rect.h = tile_size;
			SDL_RenderFillRect(renderer, &rect);
			i++;
		}
	}

	int size_x, size_y;
	std::vector<Tile> tiles;
};