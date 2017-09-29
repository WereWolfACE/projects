#ifndef __IBEHAVIOUR_H__
#define __IBEHAVIOUR_H__

#pragma once
#include "MovedObject.h"
#include "ChangeCountObjectType.h"

class MovedObject;

class IBehaviour
{
public:
	virtual ChangeCountObjectType execute(MovedObject* mainObject, std::weak_ptr<BaseObject> collisionObject) = 0;
};

#endif //__IBEHAVIOUR_H__