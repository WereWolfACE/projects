#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma once
#include "BaseObject.h"
#include "AddObjectArgs.h"
#include "PlayerView.h"
#include "BulletCreator.h"

class AddObjectArgs;

class Player : public BaseObject
{
public:
	Player(int x = 0, int y = 0);
	void MoveTo(const FPoint &point);
	void Fire(const FPoint &point);
	void SetLimitPosition(FPoint xLimit);
	void ChangeView();
	void ChangeBulletCollisionBehaviour(std::shared_ptr<IBehaviour> collisionTargetBehaviour,
										std::shared_ptr<IBehaviour> collisionBorderBehaviour,
										std::shared_ptr<IBehaviour> collisionBulletBehaviour);
	void ChangeBulletSpeed(size_t speed);
public:
	Event<AddObjectArgs> OnAddObjectsEvent;
private:
	FPoint _limitPosition;
	std::vector<std::shared_ptr<PlayerView>> _playerViews;
	int _playerViewIndex;
	BulletCreator _creator;
};

#endif //__PLAYER_H__

