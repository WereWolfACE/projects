#pragma once

#include <boost/intrusive_ptr.hpp>

#include <Render/RenderDevice.h>
#include <Render/SpriteBatch.h>
#include <Render/Texture.h>
#include <Render/VertexBufferBuilder.h>
#include <FreeType/bindings/FreeTypeEngineBinding.h>

#include "Flash/core/Render/FlashRender.h"

namespace Flash {
	bool isTextSnapped();
}

class FlashEngineRenderDevice: public IDeviceRender
{
public:
	FlashEngineRenderDevice();

	void setSriteBatchWithText(bool);
	bool getSriteBatchWithText() const;

	void setShader(FlashRender* render, FlashConstantId id, FlashShader shader) override;
	void setConstantEnum(FlashRender* render, FlashConstantId id, FlashEnum x) override;

	void setConstantFloat(FlashRender* render, FlashConstantId id, float x) override;
	void setConstantVector(FlashRender* render, FlashConstantId id, float* v) override;
	void setConstantMatrix3x2(FlashRender* render, FlashConstantId id, float* m) override;
	void setConstantMatrix4x4(FlashRender* render, FlashConstantId id, float* m) override;

	void setTexture(FlashRender* render, FlashConstantId id, void* _texture) override;

	freetype::Text* getText(TextObjectOutputParams& _params) override;
	void DrawFreeTypeText(TextObjectOutputParams& _params) override;
	void Draw(const math::Vector3& vec1, const math::Vector3& vec2, const math::Vector3& vec3, const math::Vector3& vec4, Color color, const FRect& uv) override;
	void Flush() override;
	void Begin() override;
	void End() override;

	void beginClipPlane(float x1, float y1, float x2, float y2) override;
	void endClipPlane() override;
	void updateClipPlanes();

	void beginScissorTest(const IRect& rect, ClippingMode clippingMode) override;
	void endScissorTest() override;

private:
	struct ClipPlane
	{
		float x1, y1, x2, y2;

		ClipPlane(float x1, float y1, float x2, float y2){
			this->x1 = x1;
			this->y1 = y1;
			this->x2 = x2;
			this->y2 = y2;
		}
	};

private:
	boost::intrusive_ptr<Render::SpriteBatch> __spriteBatch;
	Render::Texture* __spriteBatchTexture;
	uint16_t __spriteBatchStarted;
	//Флаг нужен для того чтобы определить рисуются ли freeetype тексты единым батчем вместе с флешевыми спрайтами,
	//но нужно понимать что в этом случае для кажого четырехуголника глифа будут на CPU вычисляться коодинаты вершин,
	//и в определнных случаях это может привести даже к замедлению, поэтому этот флаг нужно высталять осознанно черз вызов метода setSriteBatchWithText(true),
	//по умолчанию флаг не активен(false)
	bool __spriteBatchWithText;

	std::vector<ClipPlane> clipPlanes;
	std::vector<ClipPlane> usedClipPlanes;
};