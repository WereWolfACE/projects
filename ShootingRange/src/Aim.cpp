#include "stdafx.h"
#include "Aim.h"
#include "AimView.h"
#include "Utils.h"
#include "Bullet.h"
#include "ReflectionBehaviour.h"
#include "DestroingBehaviour.h"

Aim::Aim(float x, float y, int radius) : CircleObject(x, y, radius)
{
	_view = std::make_shared<AimView>();
	Init();	
}

void Aim::Init()
{
	CircleObject::Init();
	if (_radius == 0)
	{
		_radius = Width() / 2;
	}
	_targetCollisionBehaviour = std::make_shared<ReflectionBehaviour>();
	_bulletCollisionBehaviour = std::make_shared<DestroingBehaviour>();
	_borderCollisionBehaviour = std::make_shared<ReflectionBehaviour>();
}

bool Aim::operator < (const Aim &aim) const
{
	return this->GetRadius() < aim.GetRadius();
}



