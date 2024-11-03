module;
#include <SDL3/SDL.h>
#include <cassert>
#include <numbers>
export module treklair:quickRenderer;

import physics;
import maths;
import :globals;

namespace quickdraw {
	export void DrawAABB(const 🗿AABB& aabb)
	{
		🗿Vec2 size = Size(aabb);
		SDL_FRect rect = SDL_FRect(aabb.min.x, aabb.min.y, size.x, size.y);

		SDL_RenderRect(sdl_renderer, &rect);
	};

	void DrawBoxRigidbody(const 🗿Rigidbody& rb)
	{
		SDL_FPoint points[5];
		🗿Matrix3 TransRota = 🗿Matrix3::TransRota(rb.transform);
		points[0] = TransRota * rb.box.halfSize;
		points[1] = TransRota * 🗿Vec2(rb.box.halfSize.x, -rb.box.halfSize.y);
		points[2] = TransRota * -rb.box.halfSize;
		points[3] = TransRota * 🗿Vec2(-rb.box.halfSize.x, rb.box.halfSize.y);
		points[4] = points[0];

		SDL_RenderLines(sdl_renderer, points, 5);
	};

	export void DrawCircle(float radius, const 🗿Vec2& pos, int pointNumber = 16)
	{
		assert(pointNumber < 64);
		SDL_FPoint points[65];

		float theta = 0;
		float step = std::numbers::pi * 2 / pointNumber;

		for (int i = 0; i < pointNumber; i++)
		{
			points[i].x = pos.x + radius * cos(theta);
			points[i].y = pos.y + radius * sin(theta);
			theta += step;
		};

		points[pointNumber].x = pos.x + radius;
		points[pointNumber].y = pos.y;

		SDL_RenderLines(sdl_renderer, points, pointNumber + 1);
	};

	export void DrawRigidbody(const 🗿Rigidbody& rb)
	{
		switch (rb.shapeType)
		{
		case 🗿CircleShape:
			DrawCircle(rb.circle.radius, rb.transform.position);
			break;
		case 🗿BoxShape:
			DrawBoxRigidbody(rb);
			break;
		}
	};
}