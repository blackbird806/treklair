export module treklair:gyrosystem;

import <print>;
import :rigidbody;
import :vec2;
import :constraints;

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
		Vec2 velocityDifference = firstBody->linearVelocity - secondBody->velocityAtPosLocal(-constraintPoint) ;
		
		// split the forces between the two bodies and apply them via the hinge point
		if(secondBody->inverseMass > 0)
			firstBody->addImpulseAtPosLocal(-velocityDifference * inverseMassRatio1 / secondBody->inverseMass, constraintPoint);
		if (firstBody->inverseMass > 0)
			secondBody->addImpulseAtPosLocal(velocityDifference * inverseMassRatio2 / firstBody->inverseMass, -constraintPoint);

		/*if (currentDistance > FLT_EPSILON)
		{
			Vec2 normal = difference / currentDistance;
			Vec2 linearCorrectionForce = normal * (deltaTime * linearForce);
			firstBody->linearVelocity += linearCorrectionForce * inverseMassRatio1;
			secondBody->linearVelocity -= linearCorrectionForce * inverseMassRatio2;
		}
		if (std::abs(angularDifference) > FLT_EPSILON)
		{
			firstBody->angularVelocity += angularCorrectionForce * inverseMassRatio1;
			secondBody->angularVelocity -= angularCorrectionForce * inverseMassRatio2;
		}*/
		Vec2 positionChange = secondBody->transform.position - (firstBody->transform.position + constraintPoint.rotated(firstBody->transform.rotation));

		firstBody->transform.position += positionChange * 0;
		secondBody->transform.position -= positionChange * 1;
		firstBody->transform.rotation += angularDifference * 0;
		secondBody->transform.rotation -= angularDifference * 1;
	}

	static void static_update(void* ptr, float deltaTime)
	{
		Gyrosystem* gyrosystem = (Gyrosystem*)ptr;
		gyrosystem->update(deltaTime);
	}
};