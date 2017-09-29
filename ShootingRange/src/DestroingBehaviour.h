#ifndef __DESTROINGBEHAVIOUR_H__
#define __DESTROINGBEHAVIOUR_H__

#pragma once
#include "IBehaviour.h"

class DestroingBehaviour : public IBehaviour
{
public:
	ChangeCountObjectType execute(MovedObject* mainObject, std::weak_ptr<BaseObject> collisionObject);
};

#endif //__DESTROINGBEHAVIOUR_H__