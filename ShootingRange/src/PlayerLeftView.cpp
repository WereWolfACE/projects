#include "stdafx.h"
#include "PlayerLeftView.h"
#include "Utils.h"

void PlayerLeftView::Init()
{
	_view = Core::resourceManager.Get<Render::Texture>("LeftPlayer");
}

FPoint PlayerLeftView::GetBulletVector(const FPoint & position)
{
	FPoint f1(position.x + Width(), position.y);
	FPoint f2(position.x, position.y + Height());
	FPoint vector = Utils::GetInstance().GetVector(f1, f2);

	return vector;
}

FPoint PlayerLeftView::GetBulletStartPoint(const FPoint & position)
{
	FPoint startPoint = FPoint(position.x - 10, position.y + Height() - 10);
	return startPoint;
}
