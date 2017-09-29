#pragma once

#include "./rtypes.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

/**
* Используется рендером для вывода текстов
*/
struct IFlashTextRenderingEngine
{
	// отрисовывает текст без дополнительных трансформаций
	virtual void renderText(FlashRender* render, TextObjectOutputParams& params) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
