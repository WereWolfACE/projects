#ifndef __PLAYERVIEW_H__
#define __PLAYERVIEW_H__

#pragma once
#include "BaseView.h"

class PlayerView : public BaseView
{
public:	
	virtual FPoint GetBulletVector(const FPoint& position) = 0;
	virtual FPoint GetBulletStartPoint(const FPoint& position) = 0;
};

#endif //__PLAYERVIEW_H__
