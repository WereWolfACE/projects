#ifndef __AIMVIEW_H__
#define __AIMVIEW_H__

#pragma once
#include "BaseView.h"

class AimView : public BaseView
{
public:
	void Init();
	std::shared_ptr<AddAnimationArgs> GetDestroyAnimationArgs(FPoint position) override;
};

#endif //__AIMVIEW_H__

