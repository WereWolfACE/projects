#include "stdafx.h"
#include "PlayerDefaultView.h"
#include "Utils.h"

void PlayerDefaultView::Init()
{
	_view = Core::resourceManager.Get<Render::Texture>("DefaultPlayer");
}

FPoint PlayerDefaultView::GetBulletVector(const FPoint & position)
{
	FPoint f1(position.x + Width() / 2, position.y);
	FPoint f2(position.x + Width() / 2, position.y + Height());
	FPoint vector = Utils::GetInstance().GetVector(f1, f2);

	return vector;
}

FPoint PlayerDefaultView::GetBulletStartPoint(const FPoint & position)
{
	FPoint startPoint = FPoint(position.x + Width() / 2 - 15, position.y + Height() - 10);
	return startPoint;
}
