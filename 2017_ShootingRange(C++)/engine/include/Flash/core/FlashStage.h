#pragma once

#include "FlashDisplayObjectContainer.hpp"
#include "FlashUnknown.hpp"
#include "IFlashSprite.h"
#include "../events/UIEventManager.h"

/*
	FlashStage это контнейнер флешовых объектов + элемнент который раскидывает сообщения по флешовым объектам в случае какого либо пользовательского ввода,
	реакция на собития пользовательского ввода реализова на в классе UIEventManagerBase
*/
class FlashStage:
	public FlashDisplayObjectContainer<IFlashSprite>,
	public UIEventManagerBase
{
BEGIN_INTERFACE_MAP()
	INTERFACE_ENTRY(IFlashSprite)
	INTERFACE_ENTRY(IFlashDisplayObject)
END_INTERFACE_MAP()

public:
	enum class ScaleMode {
		//никогда не изменять масштаб
		NoScale,
		//изменять масштаб с нарушением пропорций
		ExactFit,
		//изменять масштаб до тех пор, пока рамка окна не пропадет
		NoBorder,
		//изменять масштаб до тех пор, пока не упремся в край экрана
		ShowAll
	};

	FlashStage();
	virtual ~FlashStage();

private:
	FlashStage(FlashStage &_ref);

public:
	void setDesign(float width, float height);

	void setScaleMode(ScaleMode scaleMode);
	void setScaleBounds(float minScale, float maxScale);

	ScaleMode getScaleMode() const;

	void setHorizontalAlign(float p);
	void setVerticalAlign(float p);

	IFlashDisplayObject* addChildAt(IFlashDisplayObject* displayObject, int index) override;
	IFlashDisplayObject* removeChildAt(int index) override;

	void setRotation(float x) override;
	void setScale(float x, float y) override;
	void setShear(float x) override;
	void setMatrix(const float* m) override;
	void setPosition(float x, float y) override;

	void setViewport(float x, float y, float width, float height);

	/// Точка начала координат в экранной системе координат
	void getOrigin(float& x, float& y) const;
	/// Размер сцены в экранной системе координат
	void getStageSize(float& width, float& height) const;
	/// Размер игрового экрана, в котором спроектирована игра
	void getDesignSize(float& width, float& height) const;
	void getViewport(float& x, float& y, float& width, float& height) const;

	void resized();

private:
	float originX, originY;
	float viewX, viewY, viewWidth, viewHeight;

	float stageWidth, stageHeight;

	float designWidth, designHeight;

	float horAlign, vertAlign;
	float minScale, maxScale;

	ScaleMode scaleMode;
};