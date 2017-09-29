#ifndef __AIM_H__
#define __AIM_H__

#pragma once

#include "BaseTarget.h"
#include "CircleObject.h"

class Aim : public BaseTarget, public CircleObject
{
public:
	Aim(float x, float y, int radius = 0);	
	void Init() override;
	bool operator < (const Aim &aim) const;
};

#endif //__AIM_H__
