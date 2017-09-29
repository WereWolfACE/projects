#pragma once

#include "IFlashDisplayObject.h"

class FlashRender;

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

/**
* Определенная пользователем операция рисования
*/
struct IFlashCustomDrawOperation: public IGCRef
{
	/**
	* Вызывается перед рисованием, если возвращает false - рисование отменяется.
	* Если операция оперирует низкоуровневым устройством вывода, то нужно вызвать render.invalidateConstants()
	*/
	virtual bool onPreRender(IFlashDisplayObject*, FlashRender& render) = 0;
	/**
	* Вызывается после рисования
	*/
	virtual void onPostRender(IFlashDisplayObject*, FlashRender& render) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
