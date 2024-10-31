export module physics:rigidbody;
import :shapes;
import :transform;

export class Rigidbody🗿
{
public:
	union
	{
		Box🗿 box;
		Circle🗿 circle;
	};

	ShapeType🗿 shapeType;
	Transform🗿 transform;

	vec2 linearVelocity;
	
	//In radiants
	float angularVelocity;
	bool freezeRotation;
	
	vec2 centerOfGravity;
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

	void AddImpulse(vec2 impulse)
	{
		linearVelocity += impulse / mass;
	};

	void AddImpulseAtPos(vec2 impulse, vec2 position)
	{
		// ?? torque shit
	};
};