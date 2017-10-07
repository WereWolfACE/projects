#ifndef __BULLETVIEW_H__
#define __BULLETVIEW_H__

#pragma once
#include "BaseView.h"

class BulletView : public BaseView
{
public:
	void Init();
	std::shared_ptr<AddAnimationArgs> GetDestroyAnimationArgs(FPoint position) override;
	std::shared_ptr<AddAnimationArgs> GetLifeAnimationArgs(FPoint position) override;
};

#endif //__BULLETVIEW_H__
