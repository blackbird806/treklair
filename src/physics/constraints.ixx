export module treklair:constraints;

import <cfloat>;
import :rigidbody;
import :vec2;

export struct SpringContraint
{
	Rigidbody* firstBody;
	Rigidbody* secondBody;

	float distance;
	float force;
	float damping;

	float lastDistance = distance;

public:
	void update(float deltaTime)
	{
		Vec2 difference = secondBody->transform.position - firstBody->transform.position;
		float currentDistance = difference.length();
		if (currentDistance <= FLT_EPSILON)
			return;

		Vec2 normal = difference / currentDistance;
		float distanceDiff = currentDistance - distance;
		float currentConstraintForce = force * distanceDiff * deltaTime;

		float deltaDistance = currentDistance - lastDistance;
		float dampingForce = deltaDistance * damping * deltaTime;
		currentConstraintForce += dampingForce;

		float pairInverseMass = firstBody->inverseMass + secondBody->inverseMass;
		float inverseMassRatio1 = firstBody->inverseMass / pairInverseMass;
		float inverseMassRatio2 = secondBody->inverseMass / pairInverseMass;

		firstBody->addImpulse(normal * (currentConstraintForce * inverseMassRatio1));
		secondBody->addImpulse(normal * (-currentConstraintForce * inverseMassRatio2));

		lastDistance = currentDistance;
	}
};

export struct CenteredHingeConstraint
{
	Rigidbody* secondBody;

	Vec2 position; //scond body relative to first body
	Rigidbody* firstBody;

	void update(float deltaTime)
	{
		float pairInverseMass = firstBody->inverseMass + secondBody->inverseMass;
		float inverseMassRatio1 = firstBody->inverseMass / pairInverseMass;
		float inverseMassRatio2 = secondBody->inverseMass / pairInverseMass;
		Vec2 difference = secondBody->transform.position - firstBody->transform.position;
		float currentDistance = difference.length();
		Vec2 normal = difference / currentDistance;


	}
};