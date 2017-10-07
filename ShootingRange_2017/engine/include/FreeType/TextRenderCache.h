#pragma once

#include <Render/Target.h>
#include <Render/RenderDevice.h>
#include <RefCounter.h>

namespace freetype
{
class CTextRenderCache: public RefCounter
{
public:
	CTextRenderCache():
		RefCounter(),
		width(0),
		height(0),
		top(0.f),
		left(0.f),
		target(nullptr)
	{
	}

	virtual ~CTextRenderCache()
	{
		if (target != nullptr)
		{
			Render::device.DeleteRenderTarget(target);
		};
	}

public:
	float left;
	float top;
	int width;
	int height;

	Render::Target* target;
	bool isDirty;
};
};
