export module treklair:rigidbody;
import :shapes;

export class Rigidbody
{
public:
	union
	{
		Box box;
		Circle circle;
	};

	ShapeType shapeType;
	//Scale not taken in account
	Transform transform;

	Vec2 linearVelocity;
	
	//In radiants
	float angularVelocity;
	bool freezeRotation;
	
	Vec2 centerOfGravity;
	float mass = 1;

	Rigidbody(ShapeType type)
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

	void UpdateNoCCD(float deltaTime)
	{
		transform.position += linearVelocity * deltaTime;
		transform.rotation += angularVelocity * deltaTime;
	};

//	bool CCD;
//	void UpdateCCD()
//	{
//		//ToDo CCD stuff
//	};

	void Update(float deltaTime)
	{
		UpdateNoCCD(deltaTime);
	};

	void AddImpulse(Vec2 impulse)
	{
		linearVelocity += impulse / mass;
	};

	void AddImpulseAtPos(Vec2 impulse, Vec2 position)
	{
		// ?? torque shit
	};

};