﻿module;
#include <SDL3/SDL.h>;
#include <cassert>;
#include <numbers>;
#include <list>;
#include <vector>;
#include <print>;
export module treklair:quickRenderer;

import :globals;
import :matrix3;
import :rigidbody;
import :shapes;
import :vec2;

namespace quickdraw {
	struct LineTime
	{
		Vec2 start;
		Vec2 end;
		float time;
		float currentTime = 0;
	};

	static std::vector<LineTime> lineTimes;

	export void updateDebugDraw(float deltaTime)
	{
		for (int i = 0; i < lineTimes.size(); i++)
		{
			lineTimes[i].currentTime += deltaTime;
			SDL_RenderLine(sdl_renderer, lineTimes[i].start.x, lineTimes[i].start.y, lineTimes[i].end.x, lineTimes[i].end.y);

			if (lineTimes[i].currentTime > lineTimes[i].time)
			{
				lineTimes.erase(lineTimes.begin() +  i);
				i--;
			}
		}
	}

	export void drawAABB
	(const AABB& aabb)
	{
		Vec2 size = Size(aabb);
		SDL_FRect rect = SDL_FRect(aabb.min.x, aabb.min.y, size.x, size.y);

		SDL_RenderRect(sdl_renderer, &rect);
	};

	void drawBoxRigidbody(const Rigidbody& rb)
	{
		Vec2 points[5];
		
		points[0] = rb.transform.rotate(rb.box.halfSize); //RU
		points[1] = rb.box.halfSize; //RD
		points[1].y = -rb.box.halfSize.y;
		points[1] = rb.transform.rotate(points[1]);

		points[3] = -points[1] + rb.transform.position; //LU
		points[2] = -points[0] + rb.transform.position; //LD
		points[0] += rb.transform.position;
		points[1] += rb.transform.position;
		points[4] = points[0];

		SDL_RenderLines(sdl_renderer, reinterpret_cast<SDL_FPoint*>(points), 5);
	};

	export void drawCircle(float radius, const Vec2& pos, float rotation, int pointNumber = 16)
	{
		assert(pointNumber < 64);
		SDL_FPoint points[66];

		float theta = rotation;
		float step = std::numbers::pi * 2 / pointNumber;

		for (int i = 0; i < pointNumber + 1; i++)
		{
			points[i].x = pos.x + radius * cos(theta);
			points[i].y = pos.y + radius * sin(theta);
			theta += step;
		};

		points[pointNumber + 1].x = pos.x;
		points[pointNumber + 1].y = pos.y;


		SDL_RenderLines(sdl_renderer, points, pointNumber + 2);
	};

	export void drawRigidbody(const Rigidbody& rb)
	{
		switch (rb.shapeType)
		{
		case CircleShape:
			drawCircle(rb.circle.radius, rb.transform.position, rb.transform.rotation);
			break;
		case BoxShape:
			drawBoxRigidbody(rb);
			break;
		}
	};

	export void drawContact(const Contact& contact)
	{
		SDL_RenderLine(sdl_renderer, contact.point.x, contact.point.y, contact.point.x + (contact.direction.x * contact.depth), contact.point.y + (contact.direction.y * contact.depth));
	}

	export void drawLineTime(const Vec2& start, const Vec2& end, float time)
	{
		lineTimes.push_back(LineTime(start, end, time));
	}

	export void drawLine(const Vec2& start, const Vec2& end)
	{
		SDL_RenderLine(sdl_renderer, start.x,start.y, end.x, end.y);
	}
}