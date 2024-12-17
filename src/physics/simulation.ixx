module;
#include <SDL3/SDL_render.h>
export module treklair:simulation;

import <vector>;
import <algorithm>;
import <print>;
import <string>;
import <cstdint>;
import <cmath>;
import <unordered_map>;

import :shapes;
import :constraints;
import :rigidbody;
import :quickRenderer;
import :globals;

export struct CollisionPair
{
	Rigidbody* first;
	Rigidbody* second;

	std::vector<Contact> contacts;
};

export class Simulation
{
private:
	float fixedDeltaTime = 1.0 / 200.0; // tic
	float timeSinceLastUpdate = 0;
	unsigned int idCount = 0;

	//Bodies
	std::unordered_map<int, Rigidbody> bodies; //bodies stored in this
	std::vector<Rigidbody*> sorted_bodies; //used for sweep and prune sorting

	//Pairs
	std::vector<CollisionPair> circleCirclePairs;
	std::vector<CollisionPair> boxCirclePairs;
	std::vector<CollisionPair> boxBoxPairs;
	std::vector<CollisionPair> overlapingPairs;
	std::unordered_map<Rigidbody*, std::vector<Rigidbody*>> overlapingBodiesMap;
	std::unordered_map<Rigidbody*, std::vector<Rigidbody*>> overlapingBodiesMapCache;

	//Constraints
	std::vector<DistanceContraint> distanceConstraints;


	void addPair(Rigidbody* body1, Rigidbody* body2)
	{
		if (body1->shapeType == CircleShape)
		{
			if (body2->shapeType == CircleShape)
				circleCirclePairs.push_back(CollisionPair(body1, body2));
			else
				boxCirclePairs.push_back(CollisionPair(body2, body1));
		}
		else
		{
			if (body2->shapeType == CircleShape)
				boxCirclePairs.push_back(CollisionPair(body1, body2));
			else
				boxBoxPairs.push_back(CollisionPair(body1, body2));
		}
	};

	/// <summary>
	/// Sort rigidbodies for fast aabb checks and pair by shape type bodies
	/// </summary>
	void sweepAndPrune()
	{
		sorted_bodies.clear();
		for (auto& body : bodies)
			sorted_bodies.push_back(&body.second);

		for (int i = 0; i < sorted_bodies.size(); i++)
		{
			for (int j = i + 1; j < sorted_bodies.size(); j++)
			{
				addPair(sorted_bodies[i], sorted_bodies[j]);
			}
		}
	};

	#pragma region Collision Phase
	void circleCircleCollisions()
	{
		for (CollisionPair pair : circleCirclePairs)
		{
			if (computeCircleContacts(pair.first->circle, pair.second->circle, pair.first->transform, pair.second->transform, pair.contacts))
				overlapingPairs.push_back(pair);
		}
	};

	void circleBoxCollisions()
	{
		for (CollisionPair pair : boxCirclePairs)
		{
			if (computeBoxCircleContacts(pair.first->box, pair.second->circle, pair.first->transform, pair.second->transform, pair.contacts))
				overlapingPairs.push_back(pair);
		}
	};

	void boxBoxCollisions()
	{
		for (CollisionPair pair : boxBoxPairs)
		{
			if (computeBoxContacts(pair.first->box, pair.second->box, pair.first->transform, pair.second->transform, pair.contacts))
				overlapingPairs.push_back(pair);
		}
	};

	void computeCollisions()
	{
		circleCircleCollisions();
		circleBoxCollisions();
		boxBoxCollisions();
	};

	void computeContactResponse(const Contact& shortest, const CollisionPair& pair, float deltaTime)
	{
		//Collision velocity
			// 
			//Compute linear velocity of both bodies at position

		//check if overlapped last frame, only iterate on first needed.
		bool overlapedLastFrame = false;
		std::vector<Rigidbody*> overlapingLastFrame = overlapingBodiesMapCache[pair.first];
		for (int i = 0; i < overlapingLastFrame.size(); i++)
		{
			if (overlapingLastFrame[i] == pair.second)
			{
				//overlapedLastFrame = true;
			}
		}

		float elasticity = pair.first->material.elasticity * pair.second->material.elasticity;
		float friction = pair.first->material.friction * pair.second->material.friction;
		float pairInverseMass = (pair.first->inverseMass + pair.second->inverseMass);

		Vec2 normal = shortest.direction;
		Vec2 diff1 = shortest.point - pair.first->transform.position;
		Vec2 diff2 = shortest.point - pair.second->transform.position;

		Vec2 ang1 = pair.first->angularVelocityVector(diff1);
		Vec2 ang2 = pair.second->angularVelocityVector(diff2);
		Vec2 vel1 = pair.first->linearVelocity + ang1;
		Vec2 vel2 = pair.second->linearVelocity + ang2;
		Vec2 pairVelocity = vel1 - vel2; \
		float dotPairVelocity = pairVelocity.dot(normal);
		float impulse = -(1 + elasticity) * std::max(0.0f, dotPairVelocity) / pairInverseMass;
		Vec2 vectorImpulse = normal * impulse;

		if (simulate)
		{
			//Depenetration
			float inverseMassRatio1 = pair.first->inverseMass / pairInverseMass;
			float inverseMassRatio2 = pair.second->inverseMass / pairInverseMass;

			float depth = shortest.depth;
			
			//translate solution
			//pair.first->transform.position -= shortest.direction * depth * inverseMassRatio1;
			//pair.second->transform.position += shortest.direction * depth * inverseMassRatio2;
			
			//velocity solution
			Vec2 depenetrationFirst;
			Vec2 depenetrationSecond;
			if (!pair.first->isKinematic())
			{
				depenetrationFirst = normal * -std::min(depth * inverseMassRatio1 * depenetrationForce * deltaTime / pair.first->inverseMass, depenatrationMaxForce * pair.first->inverseMass);
			}
			if (!pair.second->isKinematic())
			{
				depenetrationSecond = normal * std::min(depth * inverseMassRatio2 * depenetrationForce * deltaTime / pair.second->inverseMass, depenatrationMaxForce * pair.second->inverseMass);
			}

			Vec2 tengent = normal.tengent();
			Vec2 tangentVelo = (tengent * tengent.dot(pairVelocity));
			Vec2 frictionAccel = tangentVelo * std::min(1.0f, friction * deltaTime);

			//quickdraw::drawLineTime(shortest.point, shortest.point + frictionAccel * 10, 1);

			if (!pair.first->isKinematic())
				pair.first->addImpulseAtPosLocal(vectorImpulse + depenetrationFirst - (frictionAccel / pair.first->inverseMass), diff1);
			if (!pair.second->isKinematic())
				pair.second->addImpulseAtPosLocal(-vectorImpulse + depenetrationSecond + (frictionAccel / pair.second->inverseMass), diff2);
		}

		overlapingBodiesMap[pair.first].push_back(pair.second);
		overlapingBodiesMap[pair.second].push_back(pair.first);
	}

	void computeCollisionResponse(float deltaTime)
	{
		for (CollisionPair pair : overlapingPairs)
		{
			// if both kinematic return
			if (pair.second->inverseMass + pair.first->inverseMass <= FLT_EPSILON)
				continue;

			Contact shortest;
			shortest.depth = FLT_MAX;
			int i = 0;
			for (Contact& c : pair.contacts)
			{

				if (c.depth < 0)
				{
					c.depth *= -1.0f;
					c.direction *= -1.0f;
				}

				//equality average both contacts into one
				if (c.depth < shortest.depth + FLT_SMALL && c.depth > shortest.depth - FLT_SMALL)
				{
					shortest = Contact::Average(c, shortest);
					continue;
				}

				//shorter just set it to new value
				if (c.depth < shortest.depth)
				{
					shortest = c;
				}
			}
			
			computeContactResponse(shortest, pair, deltaTime);
		}
	};
#pragma endregion

public:
	bool simulate = true;

	void computeSimulation(float deltaTime)
	{
		//Update bodies
		if (simulate)
		{
			for (auto& body : bodies)
			{
				body.second.update(deltaTime);
			}

			for (DistanceContraint& constraint : distanceConstraints)
			{
				constraint.update(deltaTime);
			}
		}

		circleCirclePairs.clear();
		boxCirclePairs.clear();
		boxBoxPairs.clear();
		overlapingBodiesMapCache = overlapingBodiesMap;
		overlapingBodiesMap.clear();
		overlapingPairs.clear();

		//Sweep and prune
		sweepAndPrune();

		//Collision check
		computeCollisions();

		//Collision response
		computeCollisionResponse(deltaTime);
	}

	void update(float deltaTime)
	{
		if (fixedDeltaTime < 0)
		{
			computeSimulation(deltaTime);
		}
		else
		{
			timeSinceLastUpdate += deltaTime;
			if (timeSinceLastUpdate > fixedDeltaTime)
			{
				timeSinceLastUpdate = fmodf(timeSinceLastUpdate, fixedDeltaTime);
				computeSimulation(fixedDeltaTime);
			}
		}

	};

	/// <summary>
	/// Returns a pointer of copy of the given rigidbody, instancied in this simulation
	/// </summary>
	/// <param name="rigidbody">body to copy</param>
	/// <returns>instance of the body in the simulation</returns>
	Rigidbody* createRigidbody(const Rigidbody& rigidbody)
	{
		Rigidbody& body = bodies[idCount] = rigidbody;
		body.ID = idCount;
		body.computeInertiaTensor();

		idCount++;
		return &body;
	};

	DistanceContraint* createDistanceConstraint(const DistanceContraint& constraint)
	{
		distanceConstraints.push_back(constraint);
		DistanceContraint& inst = distanceConstraints.back();

		return &inst;
	};

	bool removeRigidbody(Rigidbody* body)
	{
		return bodies.erase(body->ID);
	};

	void removeRigidbodies(std::vector<Rigidbody*>& input)
	{
		for (Rigidbody* body : input)
		{
			removeRigidbody(body);
		}
		input.clear();
	};

	void clearConstraints()
	{
		distanceConstraints.clear();
	};

	void debugDrawRigidbodies()
	{
		for (Rigidbody* body : sorted_bodies)
			quickdraw::drawRigidbody(*body);
	};

	void debugDrawConstraints()
	{
		for (DistanceContraint& constraint : distanceConstraints)
			quickdraw::drawLine(constraint.firstBody->transform.position, constraint.secondBody->transform.position);
	};
};