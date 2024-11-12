module;
#include <cassert>
#include <string>
#include <stdexcept>
#include <cmath>
export module treklair:transform;
import :vec2;

export struct 🗿Transform
{
	🗿Vec2 position;
	float rotation;
	🗿Vec2 scale;

	🗿Vec2 Rotate(const 🗿Vec2& vec) const
	{
		🗿Vec2 rotated = 🗿Vec2::rotate(vec, rotation);
		return rotated;
	};
};
