#ifndef __BULLET_H__
#define __BULLET_H__

#pragma once
#include "BaseBullet.h"
#include "Border.h"
#include "CircleObject.h"

class Bullet : public BaseBullet, public CircleObject
{
public:
	Bullet();
	Bullet(int x, int y, int radius);	
	void Init() override;
};

#endif //__BULLET_H__

