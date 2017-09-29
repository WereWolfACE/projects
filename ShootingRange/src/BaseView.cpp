#include "stdafx.h"
#include "BaseView.h"

void BaseView::Draw(FPoint position, double offset)
{
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(position.x - offset, position.y - offset, 0);
	_view->Draw();
	Render::device.PopMatrix();
}

std::shared_ptr<AddAnimationArgs> BaseView::GetDestroyAnimationArgs(FPoint position)
{
	return nullptr;
}

std::shared_ptr<AddAnimationArgs> BaseView::GetCreateAnimationArgs(FPoint position)
{
	return nullptr;
}

std::shared_ptr<AddAnimationArgs> BaseView::GetLifeAnimationArgs(FPoint position)
{
	return nullptr;
}

int BaseView::Width() const
{
	return _view->Width();
}

int BaseView::Height() const
{
	return _view->Height();
}
