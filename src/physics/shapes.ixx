module;
#include <algorithm>
export module physics:shapes;
import maths;

export enum 🗿ShapeType
{
	🗿CircleShape,
	🗿BoxShape,
};

export struct 🗿AABB
{
	🗿Vec2 min;
	🗿Vec2 max;
};

export inline 🗿Vec2 Center(const 🗿AABB& aabb)
{
	return (aabb.min + aabb.max) * 0.5f;
};

export inline 🗿Vec2 Bounds(const 🗿AABB& aabb)
{
	return (aabb.max - aabb.min) * 0.5f;
};

export inline 🗿Vec2 Size(const 🗿AABB& aabb)
{
	return (aabb.max - aabb.min);
};

export struct 🗿Circle
{
	float radius;
};

export struct 🗿Box
{
	🗿Vec2 halfSize;

	🗿AABB ToAABB(🗿Vec2 position) const
	{
		return 🗿AABB(position - halfSize, position + halfSize);
	}
};

namespace physics {
	export bool AABBOverlap(const 🗿AABB& a, const 🗿AABB& b)
	{
		//If seperation along an axis detected return false
		if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
		if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

		//else both axis are intersecting, collision is occuring
		return true;
	};

	export bool CircleOverlap(const 🗿Circle& a, const 🗿Circle& b, const 🗿Transform& aT, const 🗿Transform& bT)
	{
		float sqrDist = (bT.position - aT.position).sqrLength();
		float radiusSum = a.radius + b.radius;
		return (sqrDist <= radiusSum * radiusSum);
	};

	export bool AABBCircleOverlap(const 🗿AABB& a, const 🗿Circle& b, const 🗿Vec2& pT)
	{
		🗿Vec2 centerAABB = Center(a);
		🗿Vec2 distance = pT - centerAABB;
		🗿Vec2 boundsAABB = a.max - centerAABB;
		🗿Vec2 clampDist = 🗿Vec2::clamp(distance, -boundsAABB, boundsAABB);
		🗿Vec2 closestPoint = centerAABB + clampDist;
		return (closestPoint - pT).sqrLength() <= b.radius * b.radius;
	};

	export bool BoxCircleOverlap(const 🗿Box& a, const 🗿Circle& b, const 🗿Transform& aT, const 🗿Transform& bT)
	{
		🗿Matrix3 aMat = 🗿Matrix3::TransRota(aT);
		🗿Matrix3 aMatInverse = 🗿Matrix3::Inverse(aMat);
		🗿Vec2 bPosInv = aMatInverse * (bT.position);

		return AABBCircleOverlap(a.ToAABB(🗿Vec2()), b, bPosInv);
	};

	export bool BoxOverlap(const 🗿Box& a, const 🗿Box& b, const 🗿Transform& aT, const 🗿Transform& bT)
	{
		return false;
	}

	export bool AABBBoxOverlap(const 🗿AABB& a, const 🗿Box& b, const 🗿Transform& aT, const 🗿Transform& bT)
	{
		return false;
	}
}