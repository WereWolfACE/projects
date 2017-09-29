#include "stdafx.h"
#include "PlayerRightView.h"
#include "Utils.h"

void PlayerRightView::Init()
{
	_view = Core::resourceManager.Get<Render::Texture>("RightPlayer");
}

FPoint PlayerRightView::GetBulletVector(const FPoint & position)
{
	FPoint f1(position.x, position.y);
	FPoint f2(position.x + Width(), position.y + Height());
	FPoint vector = Utils::GetInstance().GetVector(f1, f2);

	return vector;
}

FPoint PlayerRightView::GetBulletStartPoint(const FPoint & position)
{
	FPoint startPoint = FPoint(position.x + Width() - 10, position.y + Height() - 10);
	return startPoint;
}

