#include "stdafx.h"
#include "DestroingBehaviour.h"

ChangeCountObjectType DestroingBehaviour::execute(MovedObject* mainObject, std::weak_ptr<BaseObject> collisionObject)
{
	return ChangeCountObjectType::OBJECT_DELETE;
}
