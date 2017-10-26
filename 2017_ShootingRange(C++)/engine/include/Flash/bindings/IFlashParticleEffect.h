#pragma once

#include "../core/IFlashDisplayObject.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

struct IFlashParticleEffect: public IFlashDisplayObject
{
	virtual bool isEnd() = 0; // Закончился ли эффект
	virtual void reset() = 0; // Перезапустить эффект
	virtual void finish() = 0; // Закончить эффект
	virtual void pause() = 0; // Поставить эффект на паузу
	virtual void resume() = 0; // Продолжить выполнение эффекта
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
