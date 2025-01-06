export module treklair:gyrosystem;

import <print>;
import :rigidbody;
import :vec2;
import :constraints;
import :quickRenderer;

export struct Gyrosystem 
{
	union{
		Rigidbody* wheel;
		Rigidbody* firstBody;
	};
	union {
		Rigidbody* body;
		Rigidbody* secondBody;
	}; 

	SpringContraint* spring;

	Vec2 constraintPoint;
	//float linearForce = 10000;
	//float angularForce = 30;

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
		float pairInverseMass = firstBody->inverseMass + secondBody->inverseMass;
		float inverseMassRatio1 = firstBody->inverseMass / pairInverseMass;
		float inverseMassRatio2 = secondBody->inverseMass / pairInverseMass;
		Vec2 difference = secondBody->transform.position - firstBody->transform.position;
		float currentDistance = difference.length();
		
		float angularDifference = secondBody->transform.rotation - firstBody->transform.rotation;
		//float angularCorrectionForce = deltaTime * (signbit(angularDifference) ? -1 : 1) * angularForce;

		// get any velocity difference between 2 bodies
		Vec2 velocityDifference = (firstBody->linearVelocity - secondBody->velocityAtPos(firstBody->transform.position)) * 0.5f / pairInverseMass;
		
		// split the forces between the two bodies and apply them via the hinge point
		firstBody->addImpulse(-velocityDifference);
		secondBody->addImpulseAtPos(velocityDifference , firstBody->transform.position);

		Vec2 positionChange = secondBody->transform.position - (firstBody->transform.position + constraintPoint.rotated(secondBody->transform.rotation));

		firstBody->transform.position += positionChange * inverseMassRatio1;
		secondBody->transform.position -= positionChange * inverseMassRatio2;
	}

	static void static_update(void* ptr, float deltaTime)
	{
		Gyrosystem* gyrosystem = (Gyrosystem*)ptr;
		gyrosystem->update(deltaTime);
	}
};