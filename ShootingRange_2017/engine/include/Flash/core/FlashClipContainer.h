#pragma once

#include "IFlashSprite.h"
#include "FlashDisplayObjectContainer.hpp"
#include "FlashUnknown.hpp"

/**
* Контейнер, имеющий определенные размеры, для которого можно задать плоскости отсечения
*/
class FlashClipContainer:
	public FlashDisplayObjectContainer<IFlashSprite>
{
BEGIN_INTERFACE_MAP()
	INTERFACE_ENTRY(IFlashSprite)
	INTERFACE_ENTRY(IFlashDisplayObject)
END_INTERFACE_MAP()

public:
	static const int ClipNone = 0;
	static const int ClipTop = 1;
	static const int ClipLeft = 2;
	static const int ClipBottom = 4;
	static const int ClipRight = 8;
	static const int ClipHorizontal = ClipTop | ClipBottom;
	static const int ClipVertical = ClipLeft | ClipRight;
	static const int ClipAll = ClipHorizontal | ClipVertical;

public:
	FlashClipContainer();
	virtual ~FlashClipContainer();

private:
	FlashClipContainer(FlashClipContainer &_ref);

public:
	//---------------------------------------------------------------------------
	//
	//                          IFlashDisplayObject
	//
	//---------------------------------------------------------------------------
	virtual void render(FlashRender& render) override;
	virtual bool hitTest(float x, float y, IHitTestDelegate* hitTestDelegate) override;
	virtual bool getBounds(float& left, float& top, float& right, float& bottom, IFlashDisplayObject* targetCoordinateSystem) override;

	//---------------------------------------------------------------------------
	//
	//
	//
	//---------------------------------------------------------------------------
	void setClipRect(float x, float y, float width, float height);
	void getClipRect(float &x, float &y, float &width, float &height);

	void setClipping(int mask);
	int getClipping() const;

private:
	float x, y, width, height;
	int clipMask;
};
