module;
#include <SDL3/SDL_render.h>
export module treklair:simulation;

import <vector>;
import <algorithm>;
import <print>;
import <string>;
import <cstdint>;
import <unordered_map>;


import :shapes;
import :rigidbody;
import :quickRenderer;
import :globals;

static const float epsilon = 0.0001;

export struct CollisionPair
{
	Rigidbody* first;
	Rigidbody* second;

	std::vector<Contact> contacts;
};

export class Simulation
{
private:
	float fixedDeltaTime = 1.0 / 120.0; // tic
	float timeSinceLastUpdate = 0;
	unsigned int idCount = 0;
	std::unordered_map<int, Rigidbody> bodies; //bodies stored in this
	std::vector<Rigidbody*> sorted_bodies; //used for sweep and prune sorting

	std::vector<CollisionPair> circleCirclePairs;
	std::vector<CollisionPair> boxCirclePairs;
	std::vector<CollisionPair> boxBoxPairs;

	std::vector<CollisionPair> overlapingPairs;

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

	void computeCollisionResponse()
	{
		for (CollisionPair pair : overlapingPairs)
		{
			// if both kinematic return
			if (pair.second->inverseMass + pair.first->inverseMass <= FLT_EPSILON)
				continue;

			Contact shortest;
			shortest.depth = FLT_MAX;
			for (Contact c : pair.contacts)
			{
				if (c.depth < 0)
				{
					c.depth *= -1.0f;
					c.direction *= -1.0f;
				}
				if (c.depth < shortest.depth)
				{
					shortest = c;
				}
			}
			//Collision velocity
			Vec2 normal = shortest.direction;
			Vec2 pairVelocity = pair.first->linearVelocity - pair.second->linearVelocity;
			float impulse = -2 * pairVelocity.dot(normal) / (pair.first->inverseMass + pair.second->inverseMass);
			Vec2 vectorImpulse = normal * impulse;

			if (simulate)
			{
				pair.first->addImpulse(vectorImpulse);
				pair.second->addImpulse(-vectorImpulse);

				//Depenetration
				float pairInverseMass = (pair.first->inverseMass + pair.second->inverseMass);
				float inverseMassRatio1 = pair.first->inverseMass / pairInverseMass;
				float inverseMassRatio2 = pair.second->inverseMass / pairInverseMass;


				shortest.depth += epsilon;
				pair.first->transform.position -= shortest.direction * shortest.depth * inverseMassRatio1;
				pair.second->transform.position += shortest.direction * shortest.depth * inverseMassRatio2;
			}

			//quickdraw::drawLine(shortest.point, shortest.point + shortest.direction * shortest.depth);
			quickdraw::drawLineTime(shortest.point, shortest.point + shortest.direction * shortest.depth, 1);
			std::print("\nCollision occured : {0}", shortest.depth);
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
		}

		circleCirclePairs.clear();
		boxCirclePairs.clear();
		boxBoxPairs.clear();
		overlapingPairs.clear();

		//Sweep and prune
		sweepAndPrune();

		//Collision check
		computeCollisions();

		//Collision response
		computeCollisionResponse();
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
		idCount++;
		return &body;
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
	}

	void debugDrawRigidbodies()
	{
		for (Rigidbody* body : sorted_bodies)
			quickdraw::drawRigidbody(*body);
	};
};