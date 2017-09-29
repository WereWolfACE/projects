#ifndef __PLAYERDEFAULTVIEW_H__
#define __PLAYERDEFAULTVIEW_H__

#pragma once
#include "PlayerView.h"

class PlayerDefaultView : public PlayerView
{
public:
	void Init();
	FPoint GetBulletVector(const FPoint& position);
	FPoint GetBulletStartPoint(const FPoint& position);
};

#endif //__PLAYERDEFAULTVIEW_H__

