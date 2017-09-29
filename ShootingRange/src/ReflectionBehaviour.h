#ifndef __REFLECTIONBEHAVIOUR_H__
#define __REFLECTIONBEHAVIOUR_H__

#pragma once
#include "IBehaviour.h"
#include "CircleObject.h"
#include "Border.h"

class ReflectionBehaviour : public IBehaviour
{
public:
	ChangeCountObjectType execute(MovedObject* mainObject, std::weak_ptr<BaseObject> collisionObject);
private:
	FPoint Reflection(MovedObject* mainObject, CircleObject* collisionObject);
	FPoint Reflection(MovedObject* mainObject, Border* collisionObject);
};

#endif //__REFLECTIONBEHAVIOUR_H__

