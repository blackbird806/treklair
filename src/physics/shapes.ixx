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
	return aabb.min + aabb.max * 0.5f;
};

export inline 🗿Vec2 Bounds(const 🗿AABB& aabb)
{
	return aabb.max - Center(aabb);
};

export struct 🗿Circle
{
	float radius;
};

export struct 🗿Box
{
	🗿Vec2 halfSize;
};

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

export bool AABBCircleOverlap(const 🗿AABB& a, const 🗿Circle& b, const 🗿Transform& bT)
{
	🗿Vec2 centerAABB = Center(a);
	🗿Vec2 distance = bT.position - centerAABB;
	🗿Vec2 boundsAABB = a.max - centerAABB;
	🗿Vec2 clampDist = 🗿Vec2::clamp(distance, -boundsAABB, boundsAABB);
	🗿Vec2 closestPoint = centerAABB + clampDist;
	return (closestPoint - bT.position).sqrLength() <= b.radius;
};

export bool BoxCircleOverlap(const 🗿Box& a, const 🗿Circle& b, const 🗿Transform& aT, const 🗿Transform& bT)
{
	return true;
};
