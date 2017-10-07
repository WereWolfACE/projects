#include "stdafx.h"
#include "BulletCreator.h"
#include "DestroingBehaviour.h"
#include "ReflectionBehaviour.h"

BulletCreator::BulletCreator()
{
	_collisionTargetBehaviour = std::make_shared<DestroingBehaviour>();
	_collisionBorderBehaviour = std::make_shared<DestroingBehaviour>();
	_collisionBulletBehaviour = nullptr;
	_speed = 150;
	_deleteObjectDelegate = std::make_shared<Delegate<DeleteObjectArgs>>(Delegate<DeleteObjectArgs>::Create<BulletCreator, &BulletCreator::OnDeleteObject>(this));
}

void BulletCreator::ChangeDefaultBehaviour(std::shared_ptr<IBehaviour> collisionTargetBehaviour, std::shared_ptr<IBehaviour> collisionBorderBehaviour, std::shared_ptr<IBehaviour> collisionBulletBehaviour)
{
	if (collisionTargetBehaviour)
	{
		_collisionTargetBehaviour = collisionTargetBehaviour;
	}
	if (collisionBorderBehaviour)
	{
		_collisionBorderBehaviour = collisionBorderBehaviour;
	}
	if (collisionBulletBehaviour)
	{
		_collisionBulletBehaviour = collisionBulletBehaviour;
	}
}

void BulletCreator::ChangeDefaultSpeed(size_t value)
{
	_speed = value;
}

std::shared_ptr<Bullet> BulletCreator::Create()
{
	auto bullet = dynamic_pointer_cast<Bullet>(_pool.Create());
	if (bullet)
	{
		bullet->ChangeTargetCollisionBehaviour(_collisionTargetBehaviour);
		bullet->ChangeBulletCollisionBehaviour(_collisionBulletBehaviour);
		bullet->ChangeBorderCollisionBehaviour(_collisionBorderBehaviour);
		bullet->SetSpeed(_speed);
		bullet->OnDeleteObjectEvent.Add(*_deleteObjectDelegate);
		return bullet;
	}
	return nullptr;
}

void BulletCreator::OnDeleteObject(const void * pSender, DeleteObjectArgs & args)
{
	BaseObject* object = args.GetChangedObject();
	Bullet* bullet = dynamic_cast<Bullet*>(object);
	if (bullet)
	{
		_pool.Delete(bullet);
	}
}
