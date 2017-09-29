/*
* В этом файле собраны функции, которые обеспечивают работу ядра libflash с
* нашим движком.
*/

#pragma once

#include <RefCounter.h>

#include "FreeType/FreeTypeText.h"
#include "Flash/fx/FlashFX.h"
#include "Flash/core/IFlashCustomDrawOperation.h"
#include "ICustomTransform.h"
#include "Render/RenderTypes.h"

namespace Render {
	class Texture;
}

#define BUFFER_SIZE 8000

struct lua_State;

namespace Flash {

	struct FlashLibraryRecord;

	/// Инициализирует внутренние структуры. Можно вызывать несколько раз.
	void initialize();

	//Выполните следующие функции в своем проекте, чтобы использовать
	//libFlash из lua.

	///Выполняет привязку в lua новых функций
	void bindLua(lua_State* luaState);

	/// Устанавливает режим смешивания
	void setBlendMode(BlendMode mode);

	void beginClipArea(float x, float y, float width, float height);
	void endClipArea();
	///Выполняет немедленную отрисовку объекта в текущий Render::device.
	void drawFlashDisplayObjectNow(IFlashDisplayObject* displayObject, bool keepContext = false);
	
	IFlashCustomDrawOperation* getTextUnsnapOperation();

	void getRealTextBounds(IFlashText* itext, float& l, float& t, float& r, float& b, IFlashDisplayObject* targetCoordinateSystem);
	void getTextFieldContentMetrics(IFlashText* fText, float& left, float& top, float& right, float& bottom);
	void getContentLineMetrics(IFlashText* fText, float& lineAscender, float& lineSpacing, float& lineLeading);
	int getContentLinesNumber(IFlashText* fText);
	
	/// Устанавливает значение выравнивания текста по пикселям.
	/// Если хоть кто-то в стеке установил false - текст перестает быть выровненым.
	void pushTextSnapping(bool value);
	void popTextSnapping();
	bool isTextSnapped();

	/// Устанавливает раздельный вывод эффектов и непосредственно текста.
	/// При установленном true режим включается глобально, позволяя
	/// отдельным текстам выключать его у объекта.
	void pushSeparateEffect(bool value);
	void popSeparateEffect();
	bool isSeparateEffectEnabled();
	
	freetype::InlinedObject* createFlashInline(IFlashDisplayObject* displayObject, float x, float y, float width, float height);
}
