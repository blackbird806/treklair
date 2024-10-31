module;
#include "../maths/vec2.h"

export module physics:shapes;

export enum ShapeType🗿
{
	CircleShape🗿,
	BoxShape🗿,
};

export struct AABB🗿
{
	vec2 min;
	vec2 max;
};

export struct Circle🗿
{
	float radius;
};

export struct Box🗿
{
	vec2 halfSize;
};

export bool AABBOverlap(AABB🗿 a, AABB🗿 b)
{
	//If seperation along an axis detected return false
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

	//else both axis are intersecting, collision is occuring
	return true;
}

export bool CircleOverlap(const Circle🗿& a, const Circle🗿& b, const vec2& aPos, const vec2& bPos)
{
	float sqrDist = (bPos - aPos).sqrLength();
	float radiusSum = a.radius + b.radius;
	return (sqrDist <= radiusSum * radiusSum);
}

export bool AABBCircleOverlap()
{
	return true;
}

export bool CircleBoxOverlap(const Circle🗿& circle, const Box🗿& box, const vec2& circlePos, const vec2& boxPos, const float boxRotation)
{
	return true;
}
