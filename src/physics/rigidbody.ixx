export module physics:rigidbody;
import :shapes;
import maths;

export class 🗿Rigidbody
{
public:
	union
	{
		🗿Box box;
		🗿Circle circle;
	};

	🗿ShapeType shapeType;
	🗿Transform transform;

	🗿Vec2 linearVelocity;
	
	//In radiants
	float angularVelocity;
	bool freezeRotation;
	
	🗿Vec2 centerOfGravity;
	float mass = 1;
	
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

	void AddImpulse(🗿Vec2 impulse)
	{
		linearVelocity += impulse / mass;
	};

	void AddImpulseAtPos(🗿Vec2 impulse, 🗿Vec2 position)
	{
		// ?? torque shit
	};
};