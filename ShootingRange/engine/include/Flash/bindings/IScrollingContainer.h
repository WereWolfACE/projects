#pragma once

#include "../core/IFlashSprite.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

//-----------------------------------------------------------------------------
class KineticScroller;

//-----------------------------------------------------------------------------
struct IScrollingContainer: public IFlashSprite
{
	/**
	* Указывает, что размеры или положение какого-либо элемента контейнера изменились.
	*/
	virtual void contentChanged() = 0;

	/**
	* Устанавливает размер содержимого контейнера (какая область будет доступна для прокрутки)
	*/
	virtual void setContentRect(float left, float top, float right, float bottom) = 0;
	virtual void getContentRect(float& left, float& top, float& right, float& bottom) = 0;

	/**
	* На сколько пикселей допустимо заходить за край контейнера.
	* Вместе с параметром ContentRect образует итоговую область прокрутки.
	* ЗАМЕЧАНИЕ - этот overscroll не имеет отношения к кинетической прокрутке. Он используется для
	* задания дополнительных отступов для autoContentRect.
	*/
	virtual void setOverscroll(float left, float top, float right, float bottom) = 0;
	virtual void getOverscroll(float& left, float& top, float& right, float& bottom) = 0;

	/**
	* Если включить, размер области для прокрутки будет автоматически вычисляться из размера содержимого.
	*/
	virtual void setAutoContentRect(bool autoSize) = 0;
	virtual bool getAutoContentRect() const = 0;

	/**
	* Включает и выключает кинетическую прокрутку пальцем по определенному напрвалению
	*/
	virtual void setHorizontalKineticScroll(bool flag) = 0;
	virtual KineticScroller* getHorizontalScroller() = 0;
	virtual void setVerticalKineticScroll(bool flag) = 0;
	virtual KineticScroller* getVerticalScroller() = 0;

	/**
	* Включена ли прокрутка пальцем по определенному направлению
	*/
	virtual bool isHorizontalKineticScroll() const = 0;
	virtual bool isVerticalKineticScroll() const = 0;
	
	/**
	* Включить возможность прокрутки колесиком мышки
	*/
	virtual void setMouseWheelEnable(bool flag) = 0;
	
	/**
	 * Скорость прокрутки колесиком мышки (в пикселях за один такт, по умолчанию 20)
	 */
	virtual void setMouseWheelSpeed(int value) = 0;
	virtual int getMouseWheelSpeed() const = 0;
	
	/**
	 * Устанавливает смещение для скролбара
	 */
	virtual void setScrollbarOffset(float x, float y) = 0;
	virtual void getScrollbarOffset(float &x, float &y) const = 0;
	
	/**
	* Задает размеры области отсечения
	*/
	virtual void setClipRect(float left, float top, float right, float bottom) = 0;
	virtual void getClipRect(float& left, float& top, float& right, float& bottom) = 0;

	virtual void setHorizontalScroll(float scroll) = 0;
	virtual float getHorizontalScroll() const = 0;
	virtual void setVerticalScroll(float scroll) = 0;
	virtual float getVerticalScroll() const = 0;

	/**
	* Задает максимальную скорость прокрутки
	*/
	virtual float getMaxVelocity() const = 0;
	virtual void setMaxVelocity(float velocity) = 0;

	/**
	* Задает положение контента, когда контент меньше области отсечения (по умолчанию левый верхний угол) 
	* (0.f, 0.f) - левый верхний угол, (0.5f, 0.5f) центр, (1.f, 1.f) - правый нижний угол и т.д.
	*/
	virtual void setContentAlign(float hAlign, float vAlign) = 0;
	virtual void resetScrollPosition() = 0;
	virtual void setScrollerBounceNP(int value) = 0;

	virtual void setScroll(bool horizontal, bool vertical) = 0;

	/**	
	* Добавить магнитную точку в вертикальный скроллер 	*/
	virtual	void AddMagnitPointVertical(float value) = 0;

	/**
	* Добавить магнитную точку в горизонтальный скроллер */
	virtual void AddMagnitPointHorizontal(float value) = 0;
	/**
	* Перемещение скролла по вертикали (иммитация тапа) */
	virtual void SetPositionVertical(float value, float speed) = 0;

	/**
	*  Перемещение скролла по горизонтали (иммитация тапа) */
	virtual void SetPositionHorizontal(float value, float speed) = 0;

	/**
	*  Скролл движется по горизонтали? */
	virtual bool IsScrollingHorizontally() const = 0;

	/**
	*  Скролл движется по вертикали? */
	virtual bool IsScrollingVertically() const = 0;

	/**	
	* Запустить постоянное движение по вертикальному скроллеру
	* MaxSpeed - максимальная скорость, Acceleration - ускорение
	* Дефолтные значения: MaxSpeed = 50, Acceleration = 2
	* Отрицательные значения параметров для прокрутки в противоположную сторону */
	virtual void StartConstantVerticalMotion(float MaxSpeed, float Acceleration) = 0;
	/**	
	* Остановить движение вертикального скроллера */
	virtual void StopConstantVerticalMotion() = 0;

	/**	
	* Запустить постоянное движение по горизонтальному скроллеру
	* MaxSpeed - максимальная скорость, Acceleration - ускорение
	* Дефолтные значения: MaxSpeed = 50, Acceleration = 2
	* Отрицательные значения параметров для прокрутки в противоположную сторону */
	virtual void StartConstantHorizontalMotion(float MaxSpeed, float Acceleration) = 0;
	/**	
	* Остановить движение горизонтального скроллера */
	virtual void StopConstantHorizontalMotion() = 0;

	virtual void StopKineticScroll() = 0;

};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
