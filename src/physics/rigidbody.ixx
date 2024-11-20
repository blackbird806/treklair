export module treklair:rigidbody;
import :shapes;
import :transform;
import :globals;

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
	bool kinematic = false;
	
	Vec2 centerOfGravity;
	float mass = 1;
	float gravityScale = 1;

	Rigidbody():
		transform(),
		linearVelocity(),
		angularVelocity(),
		freezeRotation(false),
		kinematic(false),
		centerOfGravity(),
		mass(1),
		gravityScale(1)
	{};

	Rigidbody(const Rigidbody& other)
		: 
		shapeType(other.shapeType),
		transform(other.transform),
		linearVelocity(other.linearVelocity),
		angularVelocity(other.angularVelocity),
		freezeRotation(other.freezeRotation),
		kinematic(other.kinematic),
		centerOfGravity(other.centerOfGravity),
		mass(other.mass),
		gravityScale(other.gravityScale)
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
		kinematic(false),
		centerOfGravity(),
		mass(1),
		gravityScale(1)
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
	{
		shapeType = BoxShape;
		box = _box;
	}

	Rigidbody(const Circle& _circle)
	{
		shapeType = CircleShape;
		circle = _circle;
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
		if (kinematic)
		{
			//TO DO: Calculate estimated velocities with last transformations
			return;
		}
		linearVelocity += gravity * gravityScale * deltaTime;
		updateNoCCD(deltaTime);
	};

	void addImpulse(Vec2 impulse)
	{
		if (kinematic)
			return;
		linearVelocity += impulse / mass;
	};

	void addImpulseAtPos(Vec2 impulse, Vec2 position)
	{
		// ?? torque shit
	};

};