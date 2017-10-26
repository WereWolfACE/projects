#ifndef __BASEOBJECT_H__
#define __BASEOBJECT_H__

#pragma once
#include "BaseView.h"
#include "DeleteObjectArgs.h"
#include "AddAnimationArgs.h"

class DeleteObjectArgs;
class AddAnimationArgs;

class BaseObject
{
public:
	BaseObject(float x, float y);

	virtual void Init();
	virtual void Draw();	
	virtual void ChangeView(std::shared_ptr<BaseView> view);
	virtual void DestroyAnimation();
	virtual void CreateAnimation();
	virtual void SetLifeAnimation();
	virtual FPoint GetCenter() const;
	virtual FPoint GetPosition() const;
	virtual void SetPosition(float x, float y);

	virtual int Width() const;
	virtual int Height() const;	
public:
	Event<DeleteObjectArgs> OnDeleteObjectEvent;
	Event<AddAnimationArgs> OnAnimationEvent;
protected:	
	std::shared_ptr<BaseView> _view;
	FPoint _position;	
};

#endif //__BASEOBJECT_H__

