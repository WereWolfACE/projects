#include "stdafx.h"
#include "BulletView.h"

void BulletView::Init()
{
	_view = Core::resourceManager.Get<Render::Texture>("Bullet");
}

std::shared_ptr<AddAnimationArgs> BulletView::GetDestroyAnimationArgs(FPoint position)
{
	return std::make_shared<AddAnimationArgs>("BulletExplosion", position, AnimationType::ANIMATION_DELETE);
}

std::shared_ptr<AddAnimationArgs> BulletView::GetLifeAnimationArgs(FPoint position)
{
	return std::make_shared<AddAnimationArgs>("BulletTrace", position, AnimationType::ANIMATION_SET_UNLIMITED);
}
