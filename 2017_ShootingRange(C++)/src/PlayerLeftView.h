#ifndef __PLAYERLEFTVIEW_H__
#define __PLAYERLEFTVIEW_H__

#pragma once
#include "PlayerView.h"

class PlayerLeftView : public PlayerView
{
public:
	void Init();
	FPoint GetBulletVector(const FPoint& position);
	FPoint GetBulletStartPoint(const FPoint& position);
};

#endif //__PLAYERLEFTVIEW_H__

