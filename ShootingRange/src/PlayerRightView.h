#ifndef __PLAYERRIGHTVIEW_H__
#define __PLAYERRIGHTVIEW_H__

#pragma once
#include "PlayerView.h"

class PlayerRightView : public PlayerView
{
public:
	void Init();
	FPoint GetBulletVector(const FPoint& position);
	FPoint GetBulletStartPoint(const FPoint& position);
};

#endif //__PLAYERRIGHTVIEW_H__
