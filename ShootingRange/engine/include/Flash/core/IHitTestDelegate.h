#pragma once

struct IFlashDisplayObject;

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif
/**
* Коллбек для процедуры IFlashDisplayObject::hitTest
*/
struct IHitTestDelegate
{
	/**
	* Вызывается для каждого объекта иерархии, в который попал hitTest
	*/
	virtual void receiveNext(IFlashDisplayObject* displayObject, float localX, float localY) = 0;
};
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
