export module treklair:updatable;

export class IUpdatable
{
public:
	void virtual update(float deltaTime);
};