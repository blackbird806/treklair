export module treklair:gyrosystem;

import <print>;
import :rigidbody;
import :vec2;
import :constraints;

export struct Gyrosystem 
{
	Rigidbody* body;
	Rigidbody* wheel;
	DistanceContraint* spring;

	Vec2 constraintPoint;

	void computeConstraintPoint()
	{
		constraintPoint = body->transform.position - wheel->transform.position;
	}
	
	void init()
	{
		computeConstraintPoint();
		spring->distance = constraintPoint.length();
	}

	void update(float deltaTime)
	{

	}

	static void static_update(void* ptr, float deltaTime)
	{
		Gyrosystem* gyrosystem = (Gyrosystem*)ptr;
		gyrosystem->update(deltaTime);
	}
};