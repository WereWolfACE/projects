#include "stdafx.h"
#include "MovedObject.h"
#include "Utils.h"
#include "BaseTarget.h"
#include "BaseBullet.h"


MovedObject::MovedObject(float x, float y) : BaseObject(x, y) {}

void MovedObject::Move(double dt)
{
	FPoint point = CalculateNewCenterPositionDt(dt);
	_position = FPoint(point.x - Width()/2, point.y - Height()/2);
}

FPoint MovedObject::CalculateNewCenterPositionDt(double dt)
{
	double x = GetCenter().x + _moveVector.x * dt * _speed;
	double y = GetCenter().y + _moveVector.y * dt * _speed;
	return FPoint(x, y);
}

void MovedObject::SetMoveVector(FPoint moveVector)
{
	float length = Utils::GetInstance().GetVectorLength(moveVector);
	_moveVector.x = moveVector.x / length;
	_moveVector.y = moveVector.y / length;
}

int MovedObject::GetSpeed() const
{
	return _speed;
}

FPoint MovedObject::GetMoveVector() const
{
	return _moveVector;
}

void MovedObject::SetSpeed(int value)
{
	_speed = value;
}

bool MovedObject::IsPlaced() const
{
	return _position.x >= 0 && _position.y >= 0;
}

void MovedObject::ChangeBulletCollisionBehaviour(std::shared_ptr<IBehaviour> behaviour)
{
	_bulletCollisionBehaviour = behaviour;
}

void MovedObject::ChangeBorderCollisionBehaviour(std::shared_ptr<IBehaviour> behaviour)
{
	_borderCollisionBehaviour = behaviour;
}

void MovedObject::ChangeTargetCollisionBehaviour(std::shared_ptr<IBehaviour> behaviour)
{
	_targetCollisionBehaviour = behaviour;
}

void MovedObject::CollisionBehaviour(std::weak_ptr<BaseObject> obj)
{
	std::shared_ptr<BaseObject> object = obj.lock();
	if (object) 
	{
		BaseTarget * target = dynamic_cast<BaseTarget*>(object.get());
		if (target)
		{
			if (_targetCollisionBehaviour)
			{
				ChangeCountObjectType type = _targetCollisionBehaviour->execute(this, object);
				DoTypedAction(type);
			}
			return;
		}

		BaseBullet * bullet = dynamic_cast<BaseBullet*>(object.get());
		if (bullet)
		{
			if (_bulletCollisionBehaviour)
			{
				ChangeCountObjectType type = _bulletCollisionBehaviour->execute(this, object);
				DoTypedAction(type);
			}
			return;
		}

		Border * border = dynamic_cast<Border*>(object.get());
		if (border)
		{
			if (_borderCollisionBehaviour)
			{
				ChangeCountObjectType type = _borderCollisionBehaviour->execute(this, object);
				DoTypedAction(type);
			}
			return;
		}
	}
}

bool MovedObject::IsCanCollisionWithObject(std::weak_ptr<BaseObject> obj)
{
	std::shared_ptr<BaseObject> object = obj.lock();
	if (object) 
	{
		std::shared_ptr<BaseTarget> target = std::dynamic_pointer_cast<BaseTarget>(object);
		if (target)
		{
			return _targetCollisionBehaviour != nullptr;
		}

		std::shared_ptr<BaseBullet> bullet = std::dynamic_pointer_cast<BaseBullet>(object);
		if (bullet)
		{
			return _bulletCollisionBehaviour != nullptr;
		}

		std::shared_ptr<Border> border = std::dynamic_pointer_cast<Border>(object);
		if (border)
		{
			return _borderCollisionBehaviour != nullptr;
		}
	}
	return false;
}


void MovedObject::DoTypedAction(ChangeCountObjectType type)
{
	if (type == ChangeCountObjectType::OBJECT_DELETE)
	{		
		OnDeleteObjectEvent.Invoke(this, DeleteObjectArgs(this));
	}
}

bool MovedObject::CheckHitBorders(std::vector<std::shared_ptr<Border>>& borders, double dt)
{
	for (size_t i = 0; i < borders.size(); i++)
	{
		if (IsCanCollisionWithObject(borders[i])
			&& IsCollision(borders[i], dt) == true)
		{
			CollisionBehaviour(borders[i]);
			return true;
		}
	}
	return false;
}



