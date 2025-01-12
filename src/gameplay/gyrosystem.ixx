export module treklair:gyrosystem;

import std;
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

	Vec2 constraintDirection;
	float constraintBaseLength;
	float motorMaxForce = 500000;
	float motorDamp = 3000000;
	float bendForce = 250000;
	float bendInput;
	float springInput;

	float lastAngle;
	//float angularForce = 30;

	void computeConstraintPoint()
	{
		constraintDirection = body->transform.position - wheel->transform.position;
		constraintBaseLength = constraintDirection.length();
		constraintDirection = constraintDirection / constraintBaseLength;
	}
	
	void init()
	{
		computeConstraintPoint();
		spring->distance = constraintBaseLength;
	}

	void update(float deltaTime)
	{
		spring->distance = constraintBaseLength * springInput;

		float pairInverseMass = firstBody->inverseMass + secondBody->inverseMass;
		float inverseMassRatio1 = firstBody->inverseMass / pairInverseMass;
		float inverseMassRatio2 = secondBody->inverseMass / pairInverseMass;
		Vec2 difference = secondBody->transform.position - firstBody->transform.position;
		float currentDistance = difference.length();

		Vec2 groundNormal = Vec2::Up;
		Vec2 bodyUp = body->transform.rotate(Vec2::Up);
		float motorAngularImpulse;
		float angleToCorrect = bodyUp.cross(groundNormal);

		if(angleToCorrect < 0.8)
			body->addAngularImpulse(bendInput * bendForce * deltaTime);

		std::print("\nAngle to correct : {0}", angleToCorrect);
		motorAngularImpulse = (angleToCorrect) * motorMaxForce * deltaTime;

		float deltaAngle = angleToCorrect - lastAngle;
		float damping = deltaAngle * deltaTime * motorDamp;
		motorAngularImpulse += damping;

		firstBody->addAngularImpulse(-motorAngularImpulse);
		secondBody->addAngularImpulse(motorAngularImpulse);

		// get any velocity difference between 2 bodies
		Vec2 velocityDifference = (firstBody->linearVelocity - secondBody->velocityAtPos(firstBody->transform.position)) * 0.5f / pairInverseMass;

		// split the forces between the two bodies and apply them via the hinge point
		firstBody->addImpulse(-velocityDifference);
		secondBody->addImpulseAtPos(velocityDifference, firstBody->transform.position);

		Vec2 positionChange = secondBody->transform.position - (firstBody->transform.position + constraintDirection.rotated(secondBody->transform.rotation)	* currentDistance);

		firstBody->transform.position += positionChange * inverseMassRatio1;
		secondBody->transform.position -= positionChange * inverseMassRatio2;

		lastAngle = angleToCorrect;
	};

	void setBendInput(float input)
	{
		bendInput = input;
	};

	void setSpringInput(float input)
	{
		springInput = input * 1 + 1;
	};

	static void static_update(void* ptr, float deltaTime)
	{
		Gyrosystem* gyrosystem = (Gyrosystem*)ptr;
		gyrosystem->update(deltaTime);
	}
};