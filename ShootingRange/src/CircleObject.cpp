#include "stdafx.h"
#include "CircleObject.h"
#include "Utils.h"

CircleObject::CircleObject(float x, float y, int radius): MovedObject(x, y)
{
	_radius = radius;
}

int CircleObject::GetRadius() const
{
	return _radius;
}

bool CircleObject::IsCollision(std::weak_ptr<BaseObject> obj, float dt)
{
	std::shared_ptr<BaseObject> object = obj.lock();
	if (object) {
		std::shared_ptr<CircleObject> circle = std::dynamic_pointer_cast<CircleObject>(object);
		if (circle) {			
			return IsCollisionObject(circle, dt);
		}
		std::shared_ptr<Border> border = std::dynamic_pointer_cast<Border>(object);
		if (border) {
			return IsCollisionObject(border, dt);
		}
	}
	return false;
	
}

bool CircleObject::Intersects(std::weak_ptr<BaseObject> obj)
{
	std::shared_ptr<BaseObject> object = obj.lock();
	if (object) {
		std::shared_ptr<CircleObject> circle = std::dynamic_pointer_cast<CircleObject>(object);
		if (circle) {
			return IntersectsObject(circle);
		}		
	}
	return false;

}

bool CircleObject::IntersectsObject(std::weak_ptr<CircleObject> circle)
{
	std::shared_ptr<CircleObject> object = circle.lock();
	if (object) {
		float d = GetCenter().GetDistanceTo(object->GetCenter());
		return d <= (GetRadius() + object->GetRadius());
	}
	return false;
}

bool CircleObject::IsCollisionObject(std::weak_ptr<CircleObject> object, float dt)
{
	std::shared_ptr<CircleObject> circle = object.lock();
	if (circle) {
		//вычисляем следующую позицию обоих шаров 
		FPoint newPositionCircle1 = CalculateNewCenterPositionDt(dt);
		FPoint newPositionCircle2 = circle->CalculateNewCenterPositionDt(dt);
		//проверяем расстояние от нее до границы
		float d = Utils::GetInstance().GetDistanseFromPointToPoint(newPositionCircle1, newPositionCircle2);
		//расстояние, при котором шары коснутся друг друга
		float needDistance = GetRadius() + circle->GetRadius();
		//шар не долетит до другого
		if (math::ceil(d) > needDistance) {
			return false;
		}
		if (math::ceil(d) == needDistance) {
			Move(dt);
			circle->Move(dt);
			Draw();
			circle->Draw();
			return true;
		}
		//считаем текущее расстояние до другого шара 
		float nowDist = Utils::GetInstance().GetDistanseFromPointToPoint(CircleObject::GetCenter(), circle->GetCenter());
		//вычисляем время, за которое шары пройдут необходимое расстояние
		float newDt = (nowDist - needDistance) / (GetSpeed() + circle->GetSpeed());
		//передвигаем шары на это время
		Move(newDt);
		Draw();
		circle->Move(newDt);
		circle->Draw();
		return true;
	}
	return false;
}

bool CircleObject::IsCollisionObject(std::weak_ptr<Border> object, float dt)
{
	std::shared_ptr<Border> line = object.lock();
	if (line) {
		FPoint beginLine = line->GetBegin();
		FPoint endLine = line->GetEnd();
		//вычисляем следующую позицию
		FPoint newPosition = CalculateNewCenterPositionDt(dt);
		//проверяем расстояние от нее до границы
		float d = Utils::GetInstance().GetDistanseFromPointToLine(newPosition, beginLine, endLine);
		//считаем текущее расстояние до границы 
		float nowDist = Utils::GetInstance().GetDistanseFromPointToLine(GetCenter(), beginLine, endLine);
		//не перелетел ли наш шар за границу
		bool pointsAtSamePlain = Utils::GetInstance().IsPointsAtSamePlain(GetCenter(), newPosition, beginLine, endLine);
		//шар касается границы
		if (math::ceil(d) == GetRadius() && pointsAtSamePlain) {
			return true;
		}
		//шар не долетел до границы
		if (math::ceil(d) > GetRadius() && pointsAtSamePlain) {
			return false;
		}
		//вычисляем расстояние, которое пролетел шар за момент времени
		float distByDt = pointsAtSamePlain ? (nowDist - d) : (nowDist + d);
		//сколько должен был пролететь шар
		float needDist = nowDist - GetRadius();
		//вычисляем время, за которое шар пройдет необходимое расстояние
		float newDt = (needDist * dt) / distByDt;
		//передвигаем шар на это время
		Move(newDt);
		CircleObject::Draw();

		return true;
	}
	return false;

}