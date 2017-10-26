#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "Utils.h"
#include "PlayerDefaultView.h"
#include "PlayerLeftView.h"
#include "PlayerRightView.h"

Player::Player(int x, int y) : BaseObject(x, y)
{	
	_playerViews.push_back(std::make_shared<PlayerDefaultView>());
	_playerViews.push_back(std::make_shared<PlayerLeftView>());
	_playerViews.push_back(std::make_shared<PlayerRightView>());
	_playerViewIndex = 0;
	
	ChangeView();
}

void Player::MoveTo(const FPoint &point)
{
	float xpos = point.x - Width() / 2;
	if (xpos > _limitPosition.x  && (xpos + Width()) < _limitPosition.y)
	{
		_position.x = xpos;
	}
}

void Player::Fire(const FPoint & point)
{	
	auto view = dynamic_pointer_cast<PlayerView>(_view);	
	FPoint startPoint = view->GetBulletStartPoint(_position);
	FPoint vector = view->GetBulletVector(_position);
	std::shared_ptr<Bullet> bullet = _creator.Create();
	bullet->SetPosition(startPoint.x, startPoint.y);
	bullet->SetMoveVector(vector);		
	OnAddObjectsEvent.Invoke(this, AddObjectArgs(bullet));
}

void Player::SetLimitPosition(FPoint xLimit)
{
	_limitPosition = xLimit;
}

void Player::ChangeView()
{	
	if (!_view)
	{
		_view = _playerViews[_playerViewIndex];
		Init();		
	}
	else
	{
		float oldWidth = Width();
		if (_playerViewIndex >= _playerViews.size())
		{
			_playerViewIndex = 0;
		}
		_view = _playerViews[_playerViewIndex];
		Init();
		float newWidth = Width();
		float result = newWidth - oldWidth;
		float xpos = _position.x - result / 2;
		if (xpos <= _limitPosition.x)
		{
			_position.x = _limitPosition.x;
		}
		else if ((xpos + newWidth) > _limitPosition.y)
		{
			_position.x = _limitPosition.y - newWidth;
		}
		else
		{
			_position.x = xpos;
		}
	}

	_playerViewIndex++;
}

void Player::ChangeBulletCollisionBehaviour(std::shared_ptr<IBehaviour> collisionTargetBehaviour, 
											std::shared_ptr<IBehaviour> collisionBorderBehaviour, 
											std::shared_ptr<IBehaviour> collisionBulletBehaviour)
{
	_creator.ChangeDefaultBehaviour(collisionTargetBehaviour, collisionBorderBehaviour, collisionBulletBehaviour);
}

void Player::ChangeBulletSpeed(size_t speed)
{
	_creator.ChangeDefaultSpeed(speed);
}

