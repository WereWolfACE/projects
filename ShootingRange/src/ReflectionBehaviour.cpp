#include "stdafx.h"
#include "ReflectionBehaviour.h"
#include "Utils.h"

ChangeCountObjectType ReflectionBehaviour::execute(MovedObject* mainObj, std::weak_ptr<BaseObject> collisionObj)
{	
	std::shared_ptr<BaseObject> collisionObject = collisionObj.lock();
	if (collisionObject) {			
		CircleObject* circle = dynamic_cast<CircleObject*> (collisionObject.get());
		if (circle) {
			FPoint vector = Reflection(mainObj, circle);
			mainObj->SetMoveVector(vector);
			return ChangeCountObjectType::OBJECT_DO_NOTHING;
		}
		Border* border = dynamic_cast<Border*> (collisionObject.get());
		if (border) {
			FPoint vector = Reflection(mainObj, border);
			mainObj->SetMoveVector(vector);
			return ChangeCountObjectType::OBJECT_DO_NOTHING;
		}		
	}
	return ChangeCountObjectType::OBJECT_DO_NOTHING;
}

FPoint ReflectionBehaviour::Reflection(MovedObject* mainObject, CircleObject* collisionObject)
{
	return Utils::GetInstance().GetReflectionCircleVector(collisionObject->GetCenter(), mainObject->GetCenter(), mainObject->GetMoveVector());
}

FPoint ReflectionBehaviour::Reflection(MovedObject* mainObject, Border* collisionObject)
{
	return Utils::GetInstance().GetReflectionVector(collisionObject->GetBegin(), collisionObject->GetEnd(), mainObject->GetMoveVector());
}