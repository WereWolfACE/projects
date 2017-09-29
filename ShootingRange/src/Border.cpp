#include "stdafx.h"
#include "Border.h"

Border::Border(FPoint begin, FPoint end) : BaseObject(begin.x, begin.y)
{	
	_positionEnd = end;
}

FPoint Border::GetBegin() const
{
	return _position;
}

FPoint Border::GetEnd() const
{
	return _positionEnd;
}

void Border::Init()
{
}

void Border::Draw()
{
	Render::device.SetTexturing(false);
	Render::BeginColor(Color(255, 128, 0, 255));
	Render::DrawLine(GetBegin(), GetEnd());
	Render::EndColor();
	Render::device.SetTexturing(true);
}

Border::~Border()
{
}
