export module treklair:rigidbody;
import :shapes;
import :transform;
import :globals;
import :matrix2;

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
	Matrix2 inverseInertiaTensor;
	float gravityScale = 1;

	Rigidbody():
		transform(),
		linearVelocity(),
		angularVelocity(),
		freezeRotation(false),
		centerOfGravity(),
		inverseMass(1),
		gravityScale(1)
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
		centerOfGravity(),
		inverseMass(1),
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
		:
		transform(),
		linearVelocity(),
		angularVelocity(),
		freezeRotation(false),
		centerOfGravity(),
		inverseMass(1),
		gravityScale(1)
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
		gravityScale(1)
	{
		shapeType = CircleShape;
		circle = _circle;
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

	void addImpulseAtPos(Vec2 impulse, Vec2 position)
	{
		
	};

};