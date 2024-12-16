export module treklair:constraint;

import :rigidbody;

export struct DistanceContraint
{
	Rigidbody* firstBody;
	Rigidbody* secondBody;

	float damping;
	float force;
	float distance;

	void Update(float deltaTime)
	{

	}
};

