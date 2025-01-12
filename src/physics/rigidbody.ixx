module;
#define _USE_MATH_DEFINES
import <cmath>;
import <cfloat>;

export module treklair:rigidbody;
import :shapes;
import :transform;
import :globals;
import :matrix2;
import :matrix3;

struct PhysicsMaterial
{
	float elasticity = 0.0f;
	float friction = 5.0f;
};

export struct Rigidbody
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
	Vec2 depenetrationVelocity;

	//In radiants
	float angularVelocity;
	bool freezeRotation = false;

	Vec2 centerOfGravity;
	float inverseMass = 1;
	float inverseInertia = 1;
	float gravityScale = 1;
	PhysicsMaterial material;

	Rigidbody() :
		transform(),
		linearVelocity(),
		angularVelocity(),
		freezeRotation(false),
		centerOfGravity(),
		inverseMass(1),
		gravityScale(1),
		material()
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
		material(other.material)
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
		material()
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
		material()
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
		material()
	{
		shapeType = CircleShape;
		circle = _circle;
	}

	void computeInertiaTensor()
	{
		switch (shapeType)
		{
		case BoxShape:
			inverseInertia = (2.0f/5.0f) * inverseMass / ((box.halfSize.x * box.halfSize.x + box.halfSize.y * box.halfSize.y));
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
		linearVelocity += (depenetrationVelocity + gravity * gravityScale) * deltaTime;
		depenetrationVelocity = Vec2::Zero;
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
		float angularImpact = diff.cross(impulse);
		float torque = angularImpact * inverseInertia ;
		Vec2 accel = impulse * inverseMass;
		linearVelocity += (accel);
		angularVelocity += (torque);
	};

	void addImpulseAtPosLocal(Vec2 impulse, Vec2 position)
	{
		float angularImpact = position.cross(impulse);
		float torque = angularImpact * inverseInertia;
		Vec2 accel = impulse * inverseMass;
		linearVelocity += (accel);
		angularVelocity += (torque);
	}

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
		return normal.tengent() * (angularVelocity * inverseInertia * distance);
	}

	Vec2 angularVelocityVector(Vec2 localPosition)
	{
		return localPosition.tengent() * (-angularVelocity);
	}
};