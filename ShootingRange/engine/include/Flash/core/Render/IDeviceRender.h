#pragma once

#include <Utils/FRect.h>
#include <Utils/Color.h>

#include "./rtypes.h"
#include "ClippingMode.h"

//---------------------------------------------------------------------------
namespace freetype
{
	class Text;
};

class FlashRender;

//-----------------------------------------------------------------------------
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

/**
* Абстракция для устройства рисования
* Важно использовать эту абстракцию, а не конкретный RenderDevice. С ее помощью можно реализовать
* кэширование отрисовки, если в качестве реализации интерфейса записывать команды, а затем воспроизводить их.
*/
class IDeviceRender
{
public:
	virtual void setShader(FlashRender* render, FlashConstantId id, FlashShader shader) = 0;
	virtual void setConstantEnum(FlashRender* render, FlashConstantId id, FlashEnum x) = 0;
	virtual void setConstantFloat(FlashRender* render, FlashConstantId id, float x) = 0;
	virtual void setConstantVector(FlashRender* render, FlashConstantId id, float* v) = 0;
	virtual void setConstantMatrix3x2(FlashRender* render, FlashConstantId id, float* m) = 0;
	virtual void setConstantMatrix4x4(FlashRender* render, FlashConstantId id, float* m) = 0;
	virtual void setTexture(FlashRender* render, FlashConstantId id, void* texture) = 0;

	virtual void beginClipPlane(float x1, float y1, float x2, float y2) = 0;
	virtual void endClipPlane() = 0;

	virtual void beginScissorTest(const IRect& rect, ClippingMode clippingMode) = 0;
	virtual void endScissorTest() = 0;

	virtual freetype::Text* getText(TextObjectOutputParams& _params) = 0;
	virtual void DrawFreeTypeText(TextObjectOutputParams& _params) = 0;
	virtual void Draw(const math::Vector3& vec1, const math::Vector3& vec2, const math::Vector3& vec3, const math::Vector3& vec4, Color color, const FRect& uv) = 0;
	virtual void Flush() = 0;
	virtual void Begin() = 0;
	virtual void End() = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
