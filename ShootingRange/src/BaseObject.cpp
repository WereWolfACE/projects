#include "stdafx.h"
#include "BaseObject.h"
#include "Utils.h"


BaseObject::BaseObject(float x, float y)
{
	SetPosition(x, y);	
}

void BaseObject::Init()
{
	_view->Init();
}

void BaseObject::Draw()
{
	_view->Draw(_position);	
}

void BaseObject::ChangeView(std::shared_ptr<BaseView> view)
{
	_view = view;
	_view->Init();
	SetLifeAnimation();
}

void BaseObject::DestroyAnimation()
{
	std::shared_ptr<AddAnimationArgs> args = _view->GetDestroyAnimationArgs(GetCenter());
	if (args) {
		OnAnimationEvent.Invoke(this, *args);
	}
	
}

void BaseObject::CreateAnimation()
{
	std::shared_ptr<AddAnimationArgs> args = _view->GetCreateAnimationArgs(GetCenter());
	if (args) {
		OnAnimationEvent.Invoke(this, *args);
	}
}

void BaseObject::SetLifeAnimation()
{
	std::shared_ptr<AddAnimationArgs> args = _view->GetLifeAnimationArgs(GetCenter());
	if (args) {
		OnAnimationEvent.Invoke(this, *args);
	}
}

FPoint BaseObject::GetCenter() const
{	
	return FPoint(_position.x + Width() / 2, _position.y + Height() / 2);
}

FPoint BaseObject::GetPosition() const
{
	return _position;
}

void BaseObject::SetPosition(float x, float y)
{
	_position = FPoint(x, y);
}

int BaseObject::Width() const
{
	return _view->Width();
}

int BaseObject::Height() const
{
	return _view->Height();
}
