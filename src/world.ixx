module;

#include <cassert>

export module treklair:world;
import std;

export enum class TileType
{
	None,
	DebugWall
};

export constexpr int tile_size = 32;

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

	int size_x, size_y;
	std::vector<Tile> tiles;
};