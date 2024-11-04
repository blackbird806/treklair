module;
#include <algorithm>
#include <print>
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

	bool ProjectOnAxis(🗿Vec2 axis, 🗿Vec2 corners[4], float halfSize)
	{
		float bProjs[4];
		float bMinProj = FLT_MAX;
		float bMaxProj = -FLT_MAX;
		for (int i = 0; i < 4; i++)
		{
			bProjs[i] = axis.dot(corners[i]);
			if (bProjs[i] < bMinProj)
				bMinProj = bProjs[i];
			if (bProjs[i] > bMaxProj)
				bMaxProj = bProjs[i];
		}

		return  bMinProj < -halfSize && bMaxProj < -halfSize || bMinProj > halfSize && bMaxProj > halfSize;
	}

	bool BoxSAT(const 🗿Box& a, const 🗿Box& b, const 🗿Transform& aT, const 🗿Transform& bT)
	{
		//first box SAT
		🗿Vec2 right = aT.Rotate(🗿Vec2::Right);
		🗿Vec2 up = aT.Rotate(🗿Vec2::Up);

		//Calculate b corner position relative to a
		🗿Vec2 bRa = bT.position - aT.position;
		🗿Vec2 bCorners[4];
		bCorners[0] = bT.Rotate(b.halfSize); //RU

		bCorners[1] = b.halfSize; //RD
		bCorners[1].y = -b.halfSize.y;
		bCorners[1] = bT.Rotate(bCorners[1]);

		bCorners[2] = -bCorners[1] + bRa; //LU
		bCorners[3] = -bCorners[0] + bRa; //LD
		bCorners[0] += bRa;
		bCorners[1] += bRa;

		//Project corners Right axis
		if (ProjectOnAxis(right, bCorners, a.halfSize.x))
			return false;
		if (ProjectOnAxis(up, bCorners, a.halfSize.y))
			return false;

		return true;
	}

	export bool BoxOverlap(const 🗿Box& a, const 🗿Box& b, const 🗿Transform& aT, const 🗿Transform& bT)
	{
		//Bounding sphere distance check for quick check opti
		if((a.halfSize.sqrLength() + b.halfSize.sqrLength()) * 2 < (bT.position - aT.position).sqrLength())
			return false;

		if (!BoxSAT(a, b, aT, bT))
			return false;
		if (!BoxSAT(b, a, bT, aT))
			return false;

		return true;
	}

	export float AABBRayOverlap(const 🗿AABB& a, 🗿Vec2 begin, 🗿Vec2 end)
	{
		return false;
	}

	export bool AABBBoxOverlap(const 🗿AABB& a, const 🗿Box& b, const 🗿Transform& aT, const 🗿Transform& bT)
	{
		return false;
	}
}