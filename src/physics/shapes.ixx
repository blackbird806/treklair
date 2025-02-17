module;
#include <SDL3/SDL_render.h>
export module treklair:shapes;
import <cfloat>;
import std;
import :matrix3;
import :matrix2;
import :vec2;
import :globals;

export struct Contact
{
	float depth;
	Vec2 direction;
	Vec2 point;

	static Contact Average(const Contact& a, const Contact& b)
	{
		Contact c;
		c.depth = (a.depth + b.depth) / 2.0f;
		c.direction = (a.direction + b.direction).getNormalized();
		c.point = a.point + ((b.point - a.point) / 2.0f);
		return c;
	}
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

	AABB ToAABB(Vec2 position) const
	{
		return AABB(position - Vec2(radius), position + Vec2(radius));
	}
};

export struct Box
{
	Vec2 halfSize;

	AABB ToAABB(Vec2 position) const
	{
		return AABB(position - halfSize, position + halfSize);
	}

	AABB ToAABBRotated(Vec2 position, float rotation) const 
    {
        if (rotation == 0.0f) {
            return AABB{position - halfSize, position + halfSize};
        }
        
        // Calculate rotated corners
        float c = std::cos(rotation);
        float s = std::sin(rotation);
        Vec2 corners[4] = {
            Vec2(c * halfSize.x - s * halfSize.y,  s * halfSize.x + c * halfSize.y),
            Vec2(-c * halfSize.x - s * halfSize.y, -s * halfSize.x + c * halfSize.y),
            Vec2(c * halfSize.x + s * halfSize.y,  s * halfSize.x - c * halfSize.y),
            Vec2(-c * halfSize.x + s * halfSize.y, -s * halfSize.x - c * halfSize.y)
        };

        // Find min/max bounds
        Vec2 min = position + corners[0];
        Vec2 max = min;
        for(int i = 1; i < 4; i++) {
            Vec2 p = position + corners[i];
            min = Vec2(std::min(min.x, p.x), std::min(min.y, p.y));
            max = Vec2(std::max(max.x, p.x), std::max(max.y, p.y));
        }
        return AABB{min, max};
    }
};

#pragma region Overlap
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

bool projectCornerOnAxis(Vec2 axis, Vec2 corners[4], float halfSize)
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
		if (!projectCornerOnAxis(axes[i], bCorners, a.halfSize[i]))
		{
			return false;
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

export float AABBRayOverlap(const AABB& a, Vec2 begin, Vec2 end)
{
	return false;
};

export bool AABBBoxOverlap(const AABB& a, const Box& b, const Transform& aT, const Transform& bT)
{
	return false;
};

#pragma endregion

#pragma region Contacts
//Contacts computing

//Box box contacts

int projectCornerOnAxis(Vec2 axis, Vec2* corners, float halfSize, float& depth, int* cornerContactIndex, int* cornerInsideIndex, int& cornerInsideCount ,float epsilon)
{
	float bProjs[4];
	float bMinProj = FLT_MAX;
	float bMaxProj = -FLT_MAX;
	int minProjIndex[2];
	int maxProjIndex[2];
	int minContactCount = 1;
	int maxContactCount = 1;
	float unsignedDepths[4];
	cornerInsideCount = 0;

	for (int i = 0; i < 4; i++)
	{
		bool exAequo = false;
		bProjs[i] = axis.dot(corners[i]);
		unsignedDepths[i] = std::abs(bProjs[i]) - halfSize;
		if (unsignedDepths[i] <= FLT_EPSILON) // negative means inside
		{
			cornerInsideIndex[cornerInsideCount] = i;
			cornerInsideCount++;
		}

		//Check ex-aequos
		if (std::abs(bProjs[i] - bMinProj) <= epsilon)
		{
			minProjIndex[1] = i;
			minContactCount = 2;
			exAequo = true;
		}
		if (std::abs(bProjs[i] - bMaxProj) <= epsilon)
		{
			maxProjIndex[1] = i;
			maxContactCount = 2;
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

	bMinProj -= FLT_EPSILON;
	bMaxProj += FLT_EPSILON;
	if (bMinProj > -halfSize && bMinProj < halfSize)
	{
		std::memcpy(cornerContactIndex, minProjIndex, sizeof(minProjIndex));
		depth = halfSize - bMinProj;

		return minContactCount;
	}
	if (bMaxProj > -halfSize && bMaxProj < halfSize)
	{
		std::memcpy(cornerContactIndex, maxProjIndex, sizeof(maxProjIndex));
		depth = -halfSize - bMaxProj;
		return maxContactCount;
	}
	//case of completely inside
	if (bMinProj < -halfSize && bMaxProj > halfSize)
	{
		float minProjDif = -bMinProj - halfSize;
		float maxProjDif = bMaxProj + halfSize; 
		if (-minProjDif > maxProjDif)
		{
			//min proj shortest
			std::memcpy(cornerContactIndex, minProjIndex, sizeof(minProjIndex));
			depth = minProjDif;

			return minContactCount;
		}
		else
		{
			std::memcpy(cornerContactIndex, maxProjIndex, sizeof(maxProjIndex));
			depth = -maxProjDif;
			return maxContactCount;
		}
	}
	return 0;
};

bool boxSAT(const Box& a, const Box& b, const Transform& aT, const Transform& bT, std::vector<Contact>& contacts, float contactDirScale = 1.0f)
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

	bCorners[3] = -bCorners[1] + bRa; //LU
	bCorners[2] = -bCorners[0] + bRa; //LD
	bCorners[0] += bRa;
	bCorners[1] += bRa;

	//Project corners on axes
	float depthX;
	float depthY;
	std::vector<int> collidingCornerIndices;
	int cornerContactIndexX[2];
	int cornerContactIndexY[2];
	int cornerInsideIndexX[4];
	int cornerInsideIndexY[4];
	int cornerInsideCountX;
	int cornerInsideCountY;
	
	int contactCountX = projectCornerOnAxis(axes[0], bCorners, a.halfSize.x, depthX, cornerContactIndexX, cornerInsideIndexX, cornerInsideCountX, FLT_SMALL);
	if (contactCountX == 0)
		return false;

	int contactCountY = projectCornerOnAxis(axes[1], bCorners, a.halfSize.y, depthY, cornerContactIndexY, cornerInsideIndexY, cornerInsideCountY, FLT_SMALL);
	if (contactCountY == 0)
		return false;

	//search if X shortest corner are inside Y axis and vice versa to securise that colliding corners are inside the box
	for (int j = 0; j < contactCountX; j++)
		for (int k = 0; k < cornerInsideCountY; k++)
		{
			if (cornerContactIndexX[j] == cornerInsideIndexY[k])
			{
				Contact contact = Contact();
				contact.point = translate * bCorners[cornerContactIndexX[j]];
				contact.depth = depthX * contactDirScale;
				contact.direction = axes[0];
				contacts.push_back(contact);
			}
		}
	
	for (int j = 0; j < contactCountY; j++)
		for (int k = 0; k < cornerInsideCountX; k++)
		{
			if (cornerContactIndexY[j] == cornerInsideIndexX[k])
			{
				Contact contact = Contact();
				contact.point = translate * bCorners[cornerContactIndexY[j]];
				contact.depth = depthY * contactDirScale;
				contact.direction = axes[1];
				contacts.push_back(contact);
			}
		}

	return true;
};

export bool computeBoxContacts(const Box& a, const Box& b, const Transform& aT, const Transform& bT, std::vector<Contact>& contacts)
{
	//Bounding sphere distance check for quick check opti
	//if ((a.halfSize.sqrLength() + b.halfSize.sqrLength()) < (bT.position - aT.position).sqrLength())
		//return false;
	return  boxSAT(a, b, aT, bT, contacts) && boxSAT(b, a, bT, aT, contacts, -1.0f);
};

//Circle contacts
export bool computeCircleContacts(const Circle& a, const Circle& b, const Transform& aT, const Transform& bT, std::vector<Contact>& contacts)
{
	Vec2 diff = (bT.position - aT.position);
	float dist = diff.length();
	float radiusSum = a.radius + b.radius;
	Contact contact;
	contact.direction = diff / dist;
	contact.point = aT.position + contact.direction * a.radius;
	contact.depth = radiusSum - dist;
	contacts.push_back(contact);
	return (dist <= radiusSum);
}

//Box Circle contacts
export bool computeAABBCircleContacts(const AABB& a, const Circle& b, const Vec2& pT, std::vector<Contact>& contacts)
{
	Vec2 centerAABB = Center(a);
	Vec2 distance = pT - centerAABB;
	Vec2 boundsAABB = a.max - centerAABB;
	Vec2 clampDist = Vec2::clamp(distance, -boundsAABB, boundsAABB);
	Vec2 closestPoint = centerAABB + clampDist;
	Vec2 diff = (closestPoint - pT);
	float closestDistSqr = diff.sqrLength();
	if (closestDistSqr > b.radius * b.radius)
		return false;

	Contact contact;

	//test if circle inside box
	if (closestDistSqr <= FLT_EPSILON)
	{
		clampDist = Vec2::closestAxis(distance, -boundsAABB, boundsAABB);
		closestPoint = centerAABB + clampDist;
		diff = ( pT - closestPoint);
		float closestDist = diff.length();
		contact.depth = -closestDist - b.radius;
		contact.direction = diff / closestDist;
	}
	else
	{
		float closestDist = sqrt(closestDistSqr);
		contact.point = closestPoint;
		contact.depth = closestDist - b.radius;
		contact.direction = diff / closestDist;
	}
	contact.point = closestPoint;

	contacts.push_back(contact);

	return true;
};

export bool computeBoxCircleContacts(const Box& a, const Circle& b, const Transform& aT, const Transform& bT, std::vector<Contact>& contacts)
{
	//Bounding sphere distance check for quick check opti
	//if ((a.halfSize.sqrLength() + b.radius * b.radius) < (bT.position - aT.position).sqrLength())
	//	return false;

	Matrix3 aMatTrans = Matrix3::translation(aT.position);
	Matrix3 aMatRota = Matrix3::rotation(aT.rotation);
	Matrix3 aMat = aMatTrans * aMatRota;
	Matrix3 aMatInverse = Matrix3::inverse(aMat);
	Vec2 bPosInv = aMatInverse * (bT.position);
	bool collide = computeAABBCircleContacts(a.ToAABB(Vec2()), b, bPosInv, contacts);
	if (contacts.empty())
		return collide;
		
	Contact& c = contacts.back();
	c.point = aMat * c.point;
	c.direction = aMatRota * c.direction;

	return collide;
};

#pragma endregion
