#ifndef __BULLETCREATOR_H__
#define __BULLETCREATOR_H__

#pragma once
#include "Bullet.h"
#include "IBehaviour.h"
#include "ObjectPool.h"

class BulletCreator
{
public:
	BulletCreator();
	void ChangeDefaultBehaviour(std::shared_ptr<IBehaviour> collisionTargetBehaviour,
			  std::shared_ptr<IBehaviour> collisionBorderBehaviour,
		      std::shared_ptr<IBehaviour> collisionBulletBehaviour);
	void ChangeDefaultSpeed(size_t value);
	std::shared_ptr<Bullet> Create();
	void OnDeleteObject(const void * pSender, DeleteObjectArgs & args);
private:
	std::shared_ptr<IBehaviour> _collisionTargetBehaviour;
	std::shared_ptr<IBehaviour> _collisionBorderBehaviour;
	std::shared_ptr<IBehaviour> _collisionBulletBehaviour;
	size_t _speed;
	ObjectPool<Bullet> _pool;
	std::shared_ptr<Delegate<DeleteObjectArgs>> _deleteObjectDelegate;
};

#endif //__BULLETCREATOR_H__

