module;
#include <algorithm>
#include <vector>
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

export bool AABBOverlap(const AABB& a, const AABB& b)
{
	//If seperation along an axis detected return false
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;

	//else both axis are intersecting, collision is occuring
	return true;
};

export bool circleOverlap(const Circle& a, const Circle& b, const Transform& aT, const Transform& bT)
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

export bool boxCircleOverlap(const Box& a, const Circle& b, const Transform& aT, const Transform& bT)
{
	Matrix3 aMat = Matrix3::transRota(aT);
	Matrix3 aMatInverse = Matrix3::inverse(aMat);
	Vec2 bPosInv = aMatInverse * (bT.position);

	return AABBCircleOverlap(a.ToAABB(Vec2()), b, bPosInv);
};

bool projectOnAxis(Vec2 axis, Vec2 corners[4], float halfSize)
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

	return  !(bMinProj < -halfSize && bMaxProj < -halfSize || bMinProj > halfSize && bMaxProj > halfSize);
};

int projectOnAxis(Vec2 axis, Vec2 corners[4], float halfSize, float& depth, int* cornerContactIndex, float epsilon)
{
	float bProjs[4];
	float bMinProj = FLT_MAX;
	float bMaxProj = -FLT_MAX;
	int minProjIndex[2];
	int maxProjIndex[2];
	int minContactCount = 1;
	int maxContactCount = 1;

	for (int i = 0; i < 4; i++)
	{
		bool exAequo = false;
		bProjs[i] = axis.dot(corners[i]);
		//Check ex-aequos
		if (std::abs(bProjs[i] - bMinProj) <= epsilon)
		{
			minProjIndex[1] = i;
			int minContactCount = 2;
			exAequo = true;
		}
		if (std::abs(bProjs[i] - bMaxProj) <= epsilon)
		{
			maxProjIndex[1] = i;
			int maxContactCount = 2;
			exAequo = true;
		}

		//if not ex aequo check pure superior / inferior
		if (!exAequo)
		{
			if (bProjs[i] < bMinProj)
			{
				bMinProj = bProjs[i];
				minProjIndex[0] = i;
				minContactCount = 1;
			}
			if (bProjs[i] > bMaxProj)
			{
				bMaxProj = bProjs[i];
				maxProjIndex[0] = i;
				maxContactCount = 1;
			}
		}
	}

	//SDL_RenderLine(sdl_renderer, 500 + axis.x * bMinProj, 500 + axis.y * bMinProj, 500 + axis.x * bMaxProj, 500 + axis.y * bMaxProj);
	//SDL_RenderLine(sdl_renderer, 500 + axis.x * -halfSize, 500 + axis.y * -halfSize, 500 + axis.x * halfSize, 500 + axis.y * halfSize);

	if (bMinProj > -halfSize && bMinProj < halfSize)
	{
		std::memcpy(cornerContactIndex, minProjIndex, sizeof(minProjIndex));
		depth = halfSize - bMinProj;

		return minContactCount;
	}
	if (bMaxProj > -halfSize && bMaxProj < halfSize)
	{
		std::memcpy(cornerContactIndex, maxProjIndex, sizeof(minProjIndex));
		depth = -bMaxProj - halfSize; //- - halfsize
		return maxContactCount;
	}
	//case of completely inside
	if (bMinProj < -halfSize && bMaxProj > halfSize)
	{
		float minProjDif = bMinProj - halfSize;
		float maxProjDif = bMaxProj + halfSize; // jsuis un génie ???
		if (minProjDif > maxProjDif)
		{
			//min proj shortest
			std::memcpy(cornerContactIndex, minProjIndex, sizeof(minProjIndex));
			depth = minProjDif;

			return minContactCount;
		}
		else
		{
			std::memcpy(cornerContactIndex, maxProjIndex, sizeof(minProjIndex));
			depth = -maxProjDif;
			return maxContactCount;
		}
	}
	return 0;
};

bool boxSAT(const Box& a, const Box& b, const Transform& aT, const Transform& bT)
{
	//first box SAT
	Vec2 axes[2];
	axes[0] = aT.rotate(Vec2::Right);
	axes[1] = aT.rotate(Vec2::Up);

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
	for (int i = 0; i < 2; i++)
	{
		if (!projectOnAxis(axes[i], bCorners, a.halfSize[i]))
		{
			return false;
		}
	}

	return true;
};


bool boxSAT(const Box& a, const Box& b, const Transform& aT, const Transform& bT, std::vector<Contact>& contacts)
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
	float depth;
	int cornerContactIndex[2];
	for (int i = 0; i < 2; i++)
	{
		int contactCount = projectOnAxis(axes[i], bCorners, a.halfSize[i], depth, cornerContactIndex, 0.01f);
		if (contactCount == 0)
			return false;

		for (int j = 0; j < contactCount; j++)
		{
			Contact contact = Contact();
			contact.point = translate * bCorners[cornerContactIndex[j]];
			contact.depth = depth;
			contact.direction = axes[i];
			contacts.push_back(contact);
		}
	}

	return true;
};

export bool boxOverlap(const Box& a, const Box& b, const Transform& aT, const Transform& bT)
{
	//Bounding sphere distance check for quick check opti
	if ((a.halfSize.sqrLength() + b.halfSize.sqrLength()) * 2 < (bT.position - aT.position).sqrLength())
		return false;

	int contactCount = 0;
	if (boxSAT(a, b, aT, bT))
		contactCount++;
	if (boxSAT(b, a, bT, aT))
		contactCount++;

	return contactCount > 1;
};

export bool computeBoxContacts(const Box& a, const Box& b, const Transform& aT, const Transform& bT, std::vector<Contact>& contacts)
{
	//Bounding sphere distance check for quick check opti
	if ((a.halfSize.sqrLength() + b.halfSize.sqrLength()) * 2 < (bT.position - aT.position).sqrLength())
		return false;

	return boxSAT(a, b, aT, bT, contacts) && boxSAT(b, a, bT, aT, contacts);
};

export float AABBRayOverlap(const AABB& a, Vec2 begin, Vec2 end)
{
	return false;
};

export bool AABBBoxOverlap(const AABB& a, const Box& b, const Transform& aT, const Transform& bT)
{
	return false;
};
