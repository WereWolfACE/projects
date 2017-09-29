#ifndef __MOVEDOBJECT_H__
#define __MOVEDOBJECT_H__

#pragma once
#include "BaseObject.h"
#include "Border.h"
#include "IBehaviour.h"
#include "ChangeCountObjectType.h"

class IBehaviour;

class MovedObject : public BaseObject
{
public:
	MovedObject(float x, float y);
	bool IsPlaced() const;
	virtual void Move(double dt);
	virtual int GetSpeed() const;
	virtual FPoint GetMoveVector() const;
	virtual void SetMoveVector(FPoint moveVector);
	virtual void SetSpeed(int value);
	virtual void CollisionBehaviour(std::weak_ptr<BaseObject> object);
	virtual void ChangeTargetCollisionBehaviour(std::shared_ptr<IBehaviour> behaviour);
	virtual void ChangeBulletCollisionBehaviour(std::shared_ptr<IBehaviour> behaviour);
	virtual void ChangeBorderCollisionBehaviour(std::shared_ptr<IBehaviour> behaviour);
	virtual bool IsCanCollisionWithObject(std::weak_ptr<BaseObject> object);
	virtual bool CheckHitBorders(std::vector<std::shared_ptr<Border>>& borders, double dt);

	virtual bool Intersects(std::weak_ptr<BaseObject> object) = 0;
	virtual bool IsCollision(std::weak_ptr<BaseObject> object, float dt) = 0;

protected:
	virtual FPoint CalculateNewCenterPositionDt(double dt);
	virtual void DoTypedAction(ChangeCountObjectType type);

protected:
	FPoint _moveVector;
	double _speed;	
	std::shared_ptr<IBehaviour> _targetCollisionBehaviour;
	std::shared_ptr<IBehaviour> _bulletCollisionBehaviour;
	std::shared_ptr<IBehaviour> _borderCollisionBehaviour;
};
#endif //__MOVEDOBJECT_H__

