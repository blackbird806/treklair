#include <SDL3/SDL_render.h>
export module treklair:simulation;

import <vector>;
import <algorithm>;
import <print>;
import <string>;


import :shapes;
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
	std::vector<Rigidbody> bodies; //use vector for faster sorting

	std::vector<CollisionPair> circleCirclePairs;
	std::vector<CollisionPair> circleBoxPairs;
	std::vector<CollisionPair> boxBoxPairs;

	std::vector<CollisionPair> overlapingPairs;

	void addPair(Rigidbody& body1, Rigidbody& body2)
	{
		if (body1.shapeType == CircleShape)
		{
			if (body2.shapeType == CircleShape)
				circleCirclePairs.push_back(CollisionPair(&body1, &body2));
			else
				circleBoxPairs.push_back(CollisionPair(&body1, &body2));
		}
		else
		{
			if (body2.shapeType == CircleShape)
				circleBoxPairs.push_back(CollisionPair(&body2, &body1));
			else
				boxBoxPairs.push_back(CollisionPair(&body1, &body2));
		}
	};

	/// <summary>
	/// Sort rigidbodies for fast aabb checks and pair by shape type bodies
	/// </summary>
	void sweepAndPrune()
	{
		for (int i = 0; i < bodies.size(); i++)
		{
			for (int j = i + 1; j < bodies.size(); j++)
			{
				addPair(bodies[i], bodies[j]);
			}
		}
	}

	void circleCircleCollisions()
	{

	}

	void circleBoxCollisions()
	{

	}

	void boxBoxCollisions()
	{
		for (CollisionPair pair : boxBoxPairs)
		{
			if (computeBoxContacts(pair.first->box, pair.second->box, pair.first->transform, pair.second->transform, pair.contacts))
				overlapingPairs.push_back(pair);
		}
	}

	void computeCollisions()
	{
		circleCircleCollisions();
		circleBoxCollisions();
		boxBoxCollisions();
	}

	void computeCollisionResponse()
	{
		for (CollisionPair pair : overlapingPairs)
		{
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
			float impulse = - 0.5 * pairVelocity.dot(normal) * (pair.first->mass + pair.second->mass);
			Vec2 vectorImpulse = normal * impulse;

			pair.first->addImpulse(vectorImpulse);
			pair.second->addImpulse(-vectorImpulse);

			//Depenetration
			if (pair.second->kinematic && pair.first->kinematic)
				return;

			float massRatio1 = 0;
			float massRatio2 = 0;

			if (pair.first->kinematic)
			{
				massRatio1 = 0; massRatio2 = 1;
			}
			else if (pair.second->kinematic)
			{
				massRatio1 = 1; massRatio2 = 0;
			}
			else
			{
				massRatio1 = pair.first->mass / (pair.first->mass + pair.second->mass);
				massRatio2 = pair.second->mass / (pair.first->mass + pair.second->mass);
			}
			pair.first->transform.position -= shortest.direction * shortest.depth * massRatio1;
			pair.second->transform.position += shortest.direction * shortest.depth * massRatio2;
			SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
			quickdraw::drawLine(shortest.point, shortest.point + shortest.direction * shortest.depth);
			//quickdraw::drawLineTime(shortest.point, shortest.point + shortest.direction * shortest.depth * 10, 1);
			std::print("\nCollision occured : {0}" , shortest.depth);
		}
	}

public:
	void update(float deltaTime)
	{
		//Update bodies
		for (Rigidbody& body : bodies)
		{
			body.update(deltaTime);
		}

		circleCirclePairs.clear();
		circleBoxPairs.clear();
		boxBoxPairs.clear();
		overlapingPairs.clear();

		//Sweep and prune
		sweepAndPrune();

		//Collision check
		computeCollisions();

		//Collision response
		computeCollisionResponse();
	};

	/// <summary>
	/// Returns a pointer of copy of the given rigidbody, instancied in this simulation
	/// </summary>
	/// <param name="rigidbody">body to copy</param>
	/// <returns>instance of the body in the simulation</returns>
	Rigidbody* createRigidbody(const Rigidbody& rigidbody)
	{
		bodies.push_back(rigidbody);
		return &bodies.back();
	};

	bool removeRigidbody(Rigidbody* rigidbody)
	{
		for (int i = 0; i < bodies.size(); i++)
		{
			if (rigidbody == &bodies[i])
			{
				bodies.erase(bodies.begin() + i);
				return true;
			}
		}

		return false;
	};

	void debugDrawRigidbodies()
	{
		for (Rigidbody& body : bodies)
			quickdraw::drawRigidbody(body);
	};
};