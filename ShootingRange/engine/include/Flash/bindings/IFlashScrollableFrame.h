#pragma once

#include "../core/IFlashSprite.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif


//-----------------------------------------------------------------------------
struct IFlashScrollableFrame : public IFlashSprite
{
	/**
	* »нициализаци€ с указанием frame rect
	*/
	virtual void init(int left, int top, int right, int bottom) = 0;

	/**
	* ¬ключение/выключение вертикального/горизонтального скролла.
	* «начение false означает, что не будет работать скролл пальцем и автоскролл в данном направлении,
	* но можно скроллить функцией resetScroll.
	*/
	virtual void enableScroll(bool horizontal, bool vertical) = 0;

	/**
	* «адать область скроллируемого фрейма
	*/
	virtual void setFrameRect(int left, int top, int right, int bottom) = 0;
	virtual void getFrameRect(int& left, int& top, int& right, int& bottom) = 0;

	/**
	* ¬ключить возможность прокрутки колесиком мышки
	*/
	virtual void enableMouseWheel(bool isEnabled) = 0;

	/**
	* «адать ориентацию скролла колЄсиком
	*/
	virtual void setMouseWheelVertical(bool isVertical) = 0;
	virtual bool isMouseWheelVertical() const = 0;

	/**
	* “екущее значениек скролла.
	* scroll находитс€ в пределах от getMinScroll() до getMaxScroll()
	*/
	virtual void getScroll(float& x, float& y) = 0;
	virtual void getMinScroll(float& x, float& y) = 0;
	virtual void getMaxScroll(float& x, float& y) = 0;

	/**
	* ”становить текущее и целевое смещение прокрутки в указанное значение
	* јвтопрокрутка будет прекращена.
	*/
	virtual void resetScroll(float x, float y) = 0;
	virtual void resetScroll(float x, float y, float minusLength) = 0;

	virtual bool inManualScroll() const = 0;

	virtual void setAutoScrollScaleFactor(float scaleFactor) = 0;
	virtual void setAutoScrollTimeConstant(float timeConstant) = 0;

	virtual void activateAutoScroll() = 0;
	virtual void deactivateAutoScroll() = 0;

	virtual void setTarget(float targetX, float targetY) = 0;
	virtual void setTarget(float targetX, float targetY, float curTimeConstant) = 0;
	virtual void getTarget(float& targetX, float& targetY) const = 0;

	virtual bool needHorizontalScroll() const = 0;
	virtual bool needVerticalScroll() const = 0;
	virtual bool needScrollLeft() const = 0;
	virtual bool needScrollRight() const = 0;
	virtual bool needScrollDown() const = 0;
	virtual bool needScrollUp() const = 0;

	virtual void setMagnetStepWidth(float width) = 0;
	virtual void setMagnetStepHeight(float height) = 0;
	virtual float getMagnetStepWidth() const = 0;
	virtual float getMagnetStepHeight() const = 0;
	virtual void setColumnsCount(int columnsCount) = 0;
	virtual void setRowsCount(int rowsCount) = 0;
	virtual int getColumnsCount() = 0;
	virtual int getRowsCount() = 0;
	virtual int getCurrentColumn() = 0;
	virtual int getCurrentRow() = 0;

	virtual void setTargetColumn(int column) = 0;
	virtual void setTargetColumn(int column, float curTimeConstant) = 0;
	virtual void setTargetRow(int row) = 0;
	virtual void setTargetRow(int row, float curTimeConstant) = 0;

	virtual int getTargetColumn() const = 0;
	virtual int getTargetRow() const = 0;

	virtual void resetMagnetPoint(int column, int row, float minusLength) = 0;

	virtual void magnetTurnOn() = 0;
	virtual void magnetTurnOff() = 0;

	virtual bool isMoving() const = 0;
	virtual bool isAutoscrollMoving() const = 0;

	/**
	* ”казывает, что размеры или положение какого-либо элемента контейнера изменились.
	*/
	virtual void contentChanged() = 0;

	/**
	* ”станавливает размер содержимого контейнера (кака€ область будет доступна дл€ прокрутки)
	*/
	virtual void setContentRect(float left, float top, float right, float bottom) = 0;
	virtual void getContentRect(float& left, float& top, float& right, float& bottom) = 0;

	/**
	* ќтступы от области контента (ContentRect).
	* ¬месте с параметром ContentRect образует итоговую область прокрутки.
	* »спользуетс€ дл€ задани€ дополнительных отступов дл€ autoContentRect.
	*/
	virtual void setContentInsets(float left, float top, float right, float bottom) = 0;
	virtual void getContentInsets(float& left, float& top, float& right, float& bottom) = 0;

	/**
	* ≈сли включить, размер области дл€ прокрутки будет автоматически вычисл€тьс€ из размера содержимого.
	*/
	virtual void enableAutoContentRect(bool isEnabled) = 0;
	virtual bool isAutoContentRect() = 0;

	/**
	* «адает положение контента, когда контент меньше области отсечени€ (по умолчанию левый верхний угол)
	* (0.f, 0.f) - левый верхний угол, (0.5f, 0.5f) центр, (1.f, 1.f) - правый нижний угол и т.д.
	*/
	virtual void setContentAlign(float hAlign, float vAlign) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
