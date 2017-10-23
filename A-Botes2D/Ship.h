#pragma once
#include "GameObject.h"

class Hull;

class Ship : public GameObject
{
public:
	Ship();
	Ship(Hull ** h, int hc);
	~Ship();

	float heading;

	//Hull
	Hull ** hull;
	int HULL_COUNT;
	//Main Batteries
	//Torpedo tubes
	//Engine
	float enginePower;
	float horsepower;


	void update();
	void draw();
};

