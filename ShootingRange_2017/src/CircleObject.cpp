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
		//��������� ��������� ������� ����� ����� 
		FPoint newPositionCircle1 = CalculateNewCenterPositionDt(dt);
		FPoint newPositionCircle2 = circle->CalculateNewCenterPositionDt(dt);
		//��������� ���������� �� ��� �� �������
		float d = Utils::GetInstance().GetDistanseFromPointToPoint(newPositionCircle1, newPositionCircle2);
		//����������, ��� ������� ���� �������� ���� �����
		float needDistance = GetRadius() + circle->GetRadius();
		//��� �� ������� �� �������
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
		//������� ������� ���������� �� ������� ���� 
		float nowDist = Utils::GetInstance().GetDistanseFromPointToPoint(CircleObject::GetCenter(), circle->GetCenter());
		//��������� �����, �� ������� ���� ������� ����������� ����������
		float newDt = (nowDist - needDistance) / (GetSpeed() + circle->GetSpeed());
		//����������� ���� �� ��� �����
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
		//��������� ��������� �������
		FPoint newPosition = CalculateNewCenterPositionDt(dt);
		//��������� ���������� �� ��� �� �������
		float d = Utils::GetInstance().GetDistanseFromPointToLine(newPosition, beginLine, endLine);
		//������� ������� ���������� �� ������� 
		float nowDist = Utils::GetInstance().GetDistanseFromPointToLine(GetCenter(), beginLine, endLine);
		//�� ��������� �� ��� ��� �� �������
		bool pointsAtSamePlain = Utils::GetInstance().IsPointsAtSamePlain(GetCenter(), newPosition, beginLine, endLine);
		//��� �������� �������
		if (math::ceil(d) == GetRadius() && pointsAtSamePlain) {
			return true;
		}
		//��� �� ������� �� �������
		if (math::ceil(d) > GetRadius() && pointsAtSamePlain) {
			return false;
		}
		//��������� ����������, ������� �������� ��� �� ������ �������
		float distByDt = pointsAtSamePlain ? (nowDist - d) : (nowDist + d);
		//������� ������ ��� ��������� ���
		float needDist = nowDist - GetRadius();
		//��������� �����, �� ������� ��� ������� ����������� ����������
		float newDt = (needDist * dt) / distByDt;
		//����������� ��� �� ��� �����
		Move(newDt);
		CircleObject::Draw();

		return true;
	}
	return false;

}