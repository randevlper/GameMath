#include "Ship.h"

#include "vec2.h"
#include "mathutils.h"

#include "Transform.h"
#include "Collider.h"
#include "sfwdraw.h"
#include "Camera.h"
#include "Physics.h"
#include "Rigidbody.h"

#include "Hull.h"
#include "NavalBattery.h"
#include "TorpedoMount.h"

#include <iostream>
Ship::Ship()
{

}

Ship::Ship(std::string t, std::string n, Physics * phys, 
	Hull ** h, int hc, 
	NavalBattery ** mG, int mGC, 
	TorpedoMount ** tM, int tMC)
{
	tag = "Ship";
	name = n;

	hull = h;
	mainGuns = mG;
	torpedoMounts = tM;

	HULL_COUNT = hc;
	MAIN_GUNS_COUNT = mGC;
	TORPEDO_MOUNT_COUNT = tMC;

	transform = new Transform(this);
	//collider = new Collider(this, phys);
	rigidbody = new Rigidbody(this);
	isEnabled = true;

	for (int i = 0; i < HULL_COUNT; ++i)
	{
		hull[i]->transform->e_parent = transform;
		hull[i]->parentShip = this;
	}

	for (int i = 0; i < MAIN_GUNS_COUNT; ++i)
	{
		mainGuns[i]->transform->e_parent = transform;
		mainGuns[i]->parentShip = this;
	}

	for (int i = 0; i < TORPEDO_MOUNT_COUNT; ++i)
	{
		torpedoMounts[i]->transform->e_parent = transform;
		torpedoMounts[i]->parentShip = this;
	}
}

Ship::~Ship()
{

}

void Ship::setup(std::string t, std::string n, Physics * phys, Hull ** h, int hc, NavalBattery ** mG, int mGC)
{
	tag = "Ship";
	name = n;

	hull = h;
	mainGuns = mG;
	HULL_COUNT = hc;
	MAIN_GUNS_COUNT = mGC;

	transform = new Transform(this);
	rigidbody = new Rigidbody(this);
	//collider = new Collider(this, phys);
	isEnabled = true;

	for (int i = 0; i < HULL_COUNT; ++i)
	{
		hull[i]->transform->e_parent = transform;
	}

	for (int i = 0; i < MAIN_GUNS_COUNT; ++i)
	{
		mainGuns[i]->transform->e_parent = transform;
		mainGuns[i]->parentShip = this;
	}
}

void Ship::shootAllGuns(vec2 pos)
{
	for (int i = 0; i < MAIN_GUNS_COUNT; ++i) 
	{
		mainGuns[i]->shoot(pos);
	}
}

void Ship::launchAllTorpedoes(vec2 pos)
{
	for (int i = 0; i < TORPEDO_MOUNT_COUNT; ++i)
	{
		torpedoMounts[i]->launchAll(pos);
	}
}

void Ship::setWeaponsAngle(vec2 pos)
{
	for (int i = 0; i < MAIN_GUNS_COUNT; ++i) 
	{
		vec2 normalVec = 
			normal(pos - (cam->mat * mainGuns[i]->transform->GetGlobalTransform()).c[2].xy);
		mainGuns[i]->transform->angle = VectorToDegree(normalVec) - transform->angle;
		//std::cout << mainGuns[i]->transform->angle - transform->angle << std::endl;
	}
	for (int i = 0; i < TORPEDO_MOUNT_COUNT; ++i)
	{
		vec2 normalVec = 
			normal(pos - (cam->mat * torpedoMounts[i]->transform->GetGlobalTransform()).c[2].xy);
		torpedoMounts[i]->transform->angle = VectorToDegree(normalVec) - transform->angle;
	}
}

void Ship::update(float dt)
{
	enginePower = clamp(enginePower,1.0f,-0.25f);
	rigidbody->velocity = degreeToVector( transform->angle, 1) * ((horsepower / rigidbody->mass) * enginePower) * 10;
	rigidbody->update(dt);

	//rigidbody->integrate(transform, dt);

	for (int i = 0; i < HULL_COUNT; ++i)
	{
		hull[i]->update();
	}
	for (int i = 0; i < MAIN_GUNS_COUNT; ++i) 
	{
		mainGuns[i]->update(dt);
	}
	for (int i = 0; i < TORPEDO_MOUNT_COUNT; ++i)
	{
		torpedoMounts[i]->update(dt);
	}
}

void Ship::draw()
{
	DrawMatrix(cam->mat * transform->GetGlobalTransform(), 20);

	for (int i = 0; i < HULL_COUNT; ++i) 
	{
		if (!hull[i]->isEnabled) { continue; }
		//vec2 pos = (cam->mat * hull[i]->transform->GetGlobalTransform()).c[2].xy;
		//DrawMatrix(cam->mat * hull[i]->transform->GetGlobalTransform(), 20);
		//sfw::drawCircle(pos.x, pos.y, hull[i]->rigidbody->radius, 12, WHITE);
		hull[i]->draw(cam->mat);
	}
	for (int i = 0; i < MAIN_GUNS_COUNT; ++i)
	{
		vec2 pos = (cam->mat * mainGuns[i]->transform->GetGlobalTransform()).c[2].xy;
		//DrawMatrix(cam->mat * mainGuns[i]->transform->GetGlobalTransform(), 20);
		//sfw::drawCircle(pos.x, pos.y, 10, 12, YELLOW);
		mainGuns[i]->draw();
	}
	for (int i = 0; i < TORPEDO_MOUNT_COUNT; ++i)
	{
		if (!torpedoMounts[i]->isEnabled) { continue; }
		torpedoMounts[i]->draw(cam->mat);
	}
	//DrawMatrix(transform->GetGlobalTransform(), 30);
}
