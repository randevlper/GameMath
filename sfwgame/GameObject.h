#pragma once
class Transform;
class Collider;

#include <string>

class GameObject
{
public:
	GameObject();
	~GameObject();

	Transform * transform;
	Collider * collider;

	bool isEnabled;
	std::string tag;
	std::string name;

	void update();

private:

};