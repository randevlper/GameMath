#include "NavalBattery.h"
#include "Transform.h"
#include "Ship.h"
#include "Camera.h"
#include "sfwdraw.h"
#include "Shell.h"
#include "Collider.h"
#include <iostream>

NavalBattery::NavalBattery(vec2 pos, float reload)
{
	isEnabled = true;
	transform = new Transform();

	reloadTime = reload;
	reloadTimer = 0;

	for (int i = 0; i < MAX_SHELLS; ++i) 
	{
		shells[i] = nullptr;
	}
}

NavalBattery::~NavalBattery()
{
}

void NavalBattery::update()
{
	if (!isEnabled) { return; }
	reloadTimer += sfw::getDeltaTime();
	for (int i = 0; i < MAX_SHELLS; ++i)
	{
		if (shells[i] == nullptr || !shells[i]->isEnabled) { continue; }
		shells[i]->update();
	}
}

void NavalBattery::draw()
{
	if (!isEnabled) { return; }
	DrawMatrix(parentShip->cam->mat * transform->GetGlobalTransform(), 20);
	for (int i = 0; i < MAX_SHELLS; ++i) 
	{
		if (shells[i] == nullptr || !shells[i]->isEnabled) { continue; }
		shells[i]->draw();
	}
}

void NavalBattery::shoot(vec2 pos)
{
	if (reloadTimer >= reloadTime) 
	{
		Shell * shell = findNextShell();
		if (shell != nullptr)
		{
			std::cout << "Pos X: " << pos.x << "Y: " << pos.y << std::endl;
			shell->setupShell(shellType1);
			shell->parentShip = parentShip;
			shell->reset();
			shell->transform->position = transform->GetGlobalTransform().c[2].xy;
			//vec2 norm = normal(transform->GetGlobalTransform().c[2].xy - pos);
			shell->collider->velocity = degreeToVector(transform->angle + parentShip->transform->angle,1) * shell->speed;
			//std::cout << "Norm X: " << norm.x << "Y: " << norm.y << std::endl;
			reloadTimer = 0;
		}
	}
}

Shell * NavalBattery::findNextShell()
{
	for (int i = 0; i < MAX_SHELLS; ++i) 
	{
		if (shells[i] == nullptr)
		{
			shells[i] = new Shell();
			++numShells;
			return shells[i];
		}
		if (!shells[i]->isEnabled) 
		{
			return shells[i];
		}
	}
	return nullptr;
}
