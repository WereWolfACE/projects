#ifndef __BASEVIEW_H__
#define __BASEVIEW_H__

#pragma once
#include "AddAnimationArgs.h"

class BaseView
{
public:
	virtual void Draw(FPoint position, double offset = 0.f);
	virtual void Init() = 0;
	virtual std::shared_ptr<AddAnimationArgs> GetDestroyAnimationArgs(FPoint position);
	virtual std::shared_ptr<AddAnimationArgs> GetCreateAnimationArgs(FPoint position);
	virtual std::shared_ptr<AddAnimationArgs> GetLifeAnimationArgs(FPoint position);
	virtual int Width() const;
	virtual int Height() const;

protected:
	Render::Texture* _view;
};

#endif //__BASEVIEW_H__

