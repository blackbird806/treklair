module;
#include <SDL3/SDL.h>
#include <cassert>
#include <numbers>
export module renderer:drawdebug;

import physics;
import maths;

export SDL_Renderer* debugRenderer;

export void DrawAABB(const 🗿AABB& aabb)
{
	🗿Vec2 size = Size(aabb);
	SDL_FRect rect = SDL_FRect(aabb.min.x, aabb.max.y, size.x, size.y);
	
	SDL_RenderRect(debugRenderer, &rect);
};

export void DrawCircle(float radius, const 🗿Vec2& pos,int pointNumber = 16)
{
	assert(pointNumber < 64);
	SDL_FPoint points[65];

	float theta = 0;
	float step = std::numbers::pi * 2 / pointNumber;

	for (int i = 0; i < pointNumber ; i++)
	{
		points[i].x = pos.x + radius * cos(theta);
		points[i].y = pos.y + radius * sin(theta);
		theta += step;
	};

	points[pointNumber].x = pos.x + radius;
	points[pointNumber].y = pos.y;

	SDL_RenderLines(debugRenderer, points, pointNumber + 1);
}

export void DrawRigidbody(const 🗿Rigidbody& rb)
{
	switch (rb.shapeType)
	{
	case 🗿CircleShape:
		DrawCircle(rb.circle.radius, rb.transform.position);
		break;
	}
};