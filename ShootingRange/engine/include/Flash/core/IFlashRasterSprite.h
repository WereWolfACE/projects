#pragma once

#include "IFlashDisplayObject.h"
#include "Render/rtypes.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

struct IFlashRasterSprite: public IFlashDisplayObject
{
	virtual void* getTextureData() = 0;
	/**
	* Устанавливает обрезку спрайта по заданному прямоугольнику.
	* Прямоугольник задается в относительных координатах [0,0]:[1,1]
	*/
	virtual void setCustomBox(float beginX, float beginY, float endX, float endY) = 0;
	virtual void getCustomBox(float& beginX, float& beginY, float& endX, float& endY) = 0;
	
	/**
	 * Устанавливает масштабирование спрайта по заданному прямоугольнику.
	 * Прямоугольник задается в относительных координатах [0,0]:[1,1]
	 */
	virtual void setSlice9Grid(float beginX, float beginY, float endX, float endY) = 0;
	
	/**
	* Отладочные функции
	* Не используйте эти функции, они могут быть удалены в следующих версиях
	*/
	virtual void getUVRect(float& beginX, float& beginY, float& endX, float& endY) = 0;
	virtual void getTexRect(float& beginX, float& beginY, float& endX, float& endY) = 0;

	virtual void setTesselation(int horizontal, int vertical) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
