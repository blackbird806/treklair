﻿module;
#define _USE_MATH_DEFINES
#include <cmath>

export module treklair:rigidbody;
import :shapes;
import :transform;
import :globals;
import :matrix2;
import :matrix3;

export class Rigidbody
{
public:
	union
	{
		Box box;
		Circle circle;
	};

	unsigned int ID = 0;

	ShapeType shapeType;
	//Scale not taken in account
	Transform transform;

	Vec2 linearVelocity;
	
	//In radiants
	float angularVelocity;
	bool freezeRotation = false;
	
	Vec2 centerOfGravity;
	float inverseMass = 1;
	float inverseInertia = 1;
	float gravityScale = 1;
	float elasticity = 1;

	Rigidbody():
		transform(),
		linearVelocity(),
		angularVelocity(),
		freezeRotation(false),
		centerOfGravity(),
		inverseMass(1),
		gravityScale(1),
		elasticity(1)
	{};

	Rigidbody(const Rigidbody& other)
		: 
		shapeType(other.shapeType),
		transform(other.transform),
		linearVelocity(other.linearVelocity),
		angularVelocity(other.angularVelocity),
		freezeRotation(other.freezeRotation),
		centerOfGravity(other.centerOfGravity),
		inverseMass(other.inverseMass),
		gravityScale(other.gravityScale),
		elasticity(other.elasticity)
	{
		switch (shapeType)
		{
		case BoxShape:
			box = other.box;
			break;
		case CircleShape:
			circle = other.circle;
			break;
		default:
			break;
		}
	};

	Rigidbody(ShapeType type)
		:
		transform(),
		linearVelocity(),
		angularVelocity(),
		freezeRotation(false),
		centerOfGravity(),
		inverseMass(1),
		gravityScale(1),
		elasticity(1)
	{
		shapeType = type;
		switch (shapeType)
		{
		case BoxShape:
			box = Box();
			break;
		case CircleShape:
			circle = Circle();
			break;
		default:
			break;
		}
	}

	Rigidbody(const Box& _box)
		:
		transform(),
		linearVelocity(),
		angularVelocity(),
		freezeRotation(false),
		centerOfGravity(),
		inverseMass(1),
		gravityScale(1),
		elasticity(1)
	{
		shapeType = BoxShape;
		box = _box;
	}

	Rigidbody(const Circle& _circle)
		:
		transform(),
		linearVelocity(),
		angularVelocity(),
		freezeRotation(false),
		centerOfGravity(),
		inverseMass(1),
		gravityScale(1),
		elasticity(1)
	{
		shapeType = CircleShape;
		circle = _circle;
	}

	void computeInertiaTensor()
	{
		switch (shapeType)
		{
		case BoxShape:
			inverseInertia = inverseMass / ( (box.halfSize.x * box.halfSize.x + box.halfSize.y * box.halfSize.y) * 1);
			break;
		case CircleShape:
			inverseInertia = 0.5 * inverseMass / (circle.radius * circle.radius);
			break;
		default:
			break;
		}
	}

	bool isKinematic()
	{
		return inverseMass == 0;
	}


	void updateNoCCD(float deltaTime)
	{
		transform.position += linearVelocity * deltaTime;
		transform.rotation += angularVelocity * deltaTime;
	};

//	bool CCD;
//	void UpdateCCD()
//	{
//		//ToDo CCD stuff
//	};

	void update(float deltaTime)
	{
		if (isKinematic())
		{
			//TO DO: Calculate estimated velocities with last transformations
			return;
		}
		linearVelocity += gravity * gravityScale * deltaTime;
		updateNoCCD(deltaTime);
	};

	void addImpulse(Vec2 impulse)
	{
		if (isKinematic())
			return;
		linearVelocity += impulse * inverseMass;
	};

	void addAngularImpulse(float impulse)
	{
		angularVelocity += impulse * inverseInertia;
	};

	void addImpulseAtPos(Vec2 impulse, Vec2 position)
	{
		Vec2 diff = position - transform.position;
		float dist = diff.length();
		Vec2 normal = diff / dist;
		float angularImpact = normal.cross(impulse) * dist;
		float torque = angularImpact * inverseInertia;
		Vec2 accel = impulse * inverseMass;
		linearVelocity += (accel);
		angularVelocity += (torque);
	};

	Vec2 velocityAtPos(Vec2 position)
	{
		Vec2 diff = (position - transform.position);
		float dist = diff.length();
		return linearVelocity + angularVelocityVector(position, dist);
	}

	Vec2 velocityAtPosLocal(Vec2 position)
	{
		float dist = (position).length();
		return linearVelocity + angularVelocityVector(position, dist);
	}

	Vec2 angularVelocityVector(Vec2 normal, float distance)
	{
		return normal.tengent() * (angularVelocity / distance * inverseInertia);
	}
};