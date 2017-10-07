#include "stdafx.h"
#include "AimView.h"

void AimView::Init() 
{
	_view = Core::resourceManager.Get<Render::Texture>("GreenAim");
}

std::shared_ptr<AddAnimationArgs> AimView::GetDestroyAnimationArgs(FPoint position)
{
	return std::make_shared<AddAnimationArgs>("TargetExplosion", position, AnimationType::ANIMATION_DELETE);
}
