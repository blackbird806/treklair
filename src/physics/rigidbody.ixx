module;
#include "../maths/vec2.h"

export module physics:rigidbody;

import <print>
import <format>

export vec2 position;
export vec2 linearVelocity;

//radiants
export float rotation; 
export float angularVelocity;
export bool freezeRotation;

export vec2 centerOfGravity;
export float mass = 1;

export bool CCD;

export class Rigidbody
{
public:
	void Update(float deltaTime)
	{
		if (CCD)
			updateCCD();
		else
			updateNoCCD(deltaTime);
	};

	void UpdateNoCCD()
	{
		position += linearVelocity * deltaTime;
		rotation += angularRotation * deltaTime;
	};

	void UpdateCCD()
	{
		//ToDo CCD stuff
	};

	void AddImpulse(vec2 impulse)
	{
		linearVelocity += impulse / mass;
	};

	void AddImpulseAtPos(vec2 impulse, vec2 position)
	{
		// ?? torque shit
	};
};