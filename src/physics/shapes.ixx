module;
#include <algorithm>
#include <print>
#include <SDL3/SDL_render.h>
export module treklair:shapes;

import :matrix3;
import :vec2;
import :globals;

export struct Contact
{
	Vec2 point;
	Vec2 direction;
	float depth;
};

export enum ShapeType
{
	CircleShape,
	BoxShape,
};

export struct AABB
{
	Vec2 min;
	Vec2 max;
};

export inline Vec2 Center(const AABB& aabb)
{
	return (aabb.min + aabb.max) * 0.5f;
};

export inline Vec2 Bounds(const AABB& aabb)
{
	return (aabb.max - aabb.min) * 0.5f;
};

export inline Vec2 Size(const AABB& aabb)
{
	return (aabb.max - aabb.min);
};

export struct Circle
{
	float radius;
};

export struct Box
{
	Vec2 halfSize;

	AABB ToAABB(Vec2 position) const
	{
		return AABB(position - halfSize, position + halfSize);
	}
};

namespace physics {
	export bool AABBOverlap(const AABB& a, const AABB& b)
	{
		//If seperation along an axis detected return false
		if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
		if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

		//else both axis are intersecting, collision is occuring
		return true;
	};

	export bool CircleOverlap(const Circle& a, const Circle& b, const Transform& aT, const Transform& bT)
	{
		float sqrDist = (bT.position - aT.position).sqrLength();
		float radiusSum = a.radius + b.radius;
		return (sqrDist <= radiusSum * radiusSum);
	};

	export bool AABBCircleOverlap(const AABB& a, const Circle& b, const Vec2& pT)
	{
		Vec2 centerAABB = Center(a);
		Vec2 distance = pT - centerAABB;
		Vec2 boundsAABB = a.max - centerAABB;
		Vec2 clampDist = Vec2::clamp(distance, -boundsAABB, boundsAABB);
		Vec2 closestPoint = centerAABB + clampDist;
		return (closestPoint - pT).sqrLength() <= b.radius * b.radius;
	};

	export bool BoxCircleOverlap(const Box& a, const Circle& b, const Transform& aT, const Transform& bT)
	{
		Matrix3 aMat = Matrix3::transRota(aT);
		Matrix3 aMatInverse = Matrix3::inverse(aMat);
		Vec2 bPosInv = aMatInverse * (bT.position);

		return AABBCircleOverlap(a.ToAABB(Vec2()), b, bPosInv);
	};

	bool ProjectOnAxis(Vec2 axis, Vec2 corners[4], float halfSize, float& depth, int& cornerContactIndex)
	{
		float bProjs[4];
		float bMinProj = FLT_MAX;
		float bMaxProj = -FLT_MAX;
		int minProjIndex;
		int maxProjIndex;
		for (int i = 0; i < 4; i++)
		{
			bProjs[i] = axis.dot(corners[i]);
			if (bProjs[i] < bMinProj)
			{
				bMinProj = bProjs[i];
				minProjIndex = i;
			}
			if (bProjs[i] > bMaxProj)
			{
				bMaxProj = bProjs[i];
				maxProjIndex = i;
			}
		}

		//SDL_RenderLine(sdl_renderer, 500 + axis.x * bMinProj, 500 + axis.y * bMinProj, 500 + axis.x * bMaxProj, 500 + axis.y * bMaxProj);
		//SDL_RenderLine(sdl_renderer, 500 + axis.x * -halfSize, 500 + axis.y * -halfSize, 500 + axis.x * halfSize, 500 + axis.y * halfSize);

		if (bMinProj > -halfSize && bMinProj < halfSize)
		{
			cornerContactIndex = minProjIndex;
			depth = std::abs(halfSize - bMinProj);
			return true;
		}
		if (bMaxProj > -halfSize && bMaxProj < halfSize)
		{
			cornerContactIndex = maxProjIndex;
			depth = std::abs(bMaxProj + halfSize); //- - halfsize
			return true;
		}
		if (bMinProj < -halfSize && bMaxProj > halfSize)
		{
			float minProjDif = -bMinProj - halfSize;
			float maxProjDif = bMaxProj - halfSize;
			if (minProjDif < maxProjDif)
			{
				//min proj shortest
				cornerContactIndex = minProjIndex;
				depth = minProjDif;
				return true;
			}
			else
			{
				cornerContactIndex = maxProjIndex;
				depth = maxProjDif;
				return true;
			}
		}
		return false;
	}

	bool BoxSAT(const Box& a, const Box& b, const Transform& aT, const Transform& bT, Contact& contact)
	{
		//first box SAT
		Vec2 axes[2];
		axes[0] = aT.rotate(Vec2::Right);
		axes[1] = aT.rotate(Vec2::Up);
		Matrix3 translate = Matrix3::translation(aT.position);

		//Calculate b corners position relative to a
		Vec2 bRa = bT.position - aT.position;
		Vec2 bCorners[4];
		bCorners[0] = bT.rotate(b.halfSize); //RU

		bCorners[1] = b.halfSize; //RD
		bCorners[1].y = -b.halfSize.y;
		bCorners[1] = bT.rotate(bCorners[1]);

		bCorners[2] = -bCorners[1] + bRa; //LU
		bCorners[3] = -bCorners[0] + bRa; //LD
		bCorners[0] += bRa;
		bCorners[1] += bRa;

		//Project corners on axes
		float depth[2];
		int cornerContactIndex[2];
		contact.depth = FLT_MAX;
		for (int i = 0; i < 2; i++)
		{
			if (!ProjectOnAxis(axes[i], bCorners, a.halfSize[i], depth[i], cornerContactIndex[i]))
			{
				contact = Contact();
				return false;
			}

			if (depth[i] < contact.depth)
			{
				contact.point = translate * (bCorners[cornerContactIndex[i]]);
				contact.depth = depth[i];
				contact.direction = axes[i];
			}
		}

		return true;
	}

	export int BoxOverlap(const Box& a, const Box& b, const Transform& aT, const Transform& bT, Contact* contacts)
	{
		//Bounding sphere distance check for quick check opti
		if ((a.halfSize.sqrLength() + b.halfSize.sqrLength()) * 2 < (bT.position - aT.position).sqrLength())
			return false;

		int contactCount = 0;
		if (BoxSAT(a, b, aT, bT, contacts[contactCount]))
			contactCount++;
		if (BoxSAT(b, a, bT, aT, contacts[contactCount]))
			contactCount++;

		return contactCount;
	}

	export float AABBRayOverlap(const AABB& a, Vec2 begin, Vec2 end)
	{
		return false;
	}

	export bool AABBBoxOverlap(const AABB& a, const Box& b, const Transform& aT, const Transform& bT)
	{
		return false;
	}
}