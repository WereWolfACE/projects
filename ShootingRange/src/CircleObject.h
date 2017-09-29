#ifndef __CIRCLEOBJECT_H__
#define __CIRCLEOBJECT_H__

#pragma once
#include "MovedObject.h"
#include "Border.h"

class CircleObject : public MovedObject
{
public:
	CircleObject(float x, float y, int radius);
	int GetRadius() const;
	bool Intersects(std::weak_ptr<BaseObject> object) override;
	bool IsCollision(std::weak_ptr<BaseObject> object, float dt) override;

private:
	bool IntersectsObject(std::weak_ptr<CircleObject> circle);
	bool IsCollisionObject(std::weak_ptr<CircleObject> circle, float dt);
	bool IsCollisionObject(std::weak_ptr<Border> line, float dt);

public:
	int _radius;	
};

#endif //__CIRCLEOBJECT_H__
