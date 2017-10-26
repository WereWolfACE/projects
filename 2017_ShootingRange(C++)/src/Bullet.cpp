#include "stdafx.h"
#include "Bullet.h"
#include "BulletView.h"
#include "Utils.h"
#include "DestroingBehaviour.h"
#include "ReflectionBehaviour.h"

Bullet::Bullet() : CircleObject(0, 0, 0)
{
	_view = std::make_shared<BulletView>();
	Init();

}

Bullet::Bullet(int x, int y, int radius) : CircleObject(x, y, radius)
{
	_view = std::make_shared<BulletView>();
	Init();
	
}

void Bullet::Init()
{
	CircleObject::Init();
	if (_radius == 0)
	{
		_radius = Width() / 2;
	}
	_targetCollisionBehaviour = std::make_shared<DestroingBehaviour>();
	_bulletCollisionBehaviour = nullptr;
	_borderCollisionBehaviour = std::make_shared<DestroingBehaviour>();
}

