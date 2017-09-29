#ifndef __PARTIALTEXTURE_H__
#define __PARTIALTEXTURE_H__

#pragma once

#include "Render/Texture.h"

namespace Render {

class Sheet;

/// Псевдо текстура ссылающаяся на участок более другой текстуры.

/// Это псевдотекстура, которая не хранит в себе изображение, а отрисовывает часть родительского Sheet'а. Обращение к унаследованному image и всем его полям небезопасно, ибо данных там нет.
/// Можно рисовать текстуру переопределёнными здесь методами, а так же можно использовать
/// RenderDeviceImpl::TranslateUV.
class PartialTexture : public Texture 
{
	friend class Render::Sheet;

public:
	PartialTexture(Texture* texture, int x, int y, int w, int h, int ix, int iy, int fw, int fh, int aL);

	virtual TextureType GetTextureType() const override { return TextureType::PartialTexture; }

	virtual const void* GetNativeHandle() const override { return texture->GetNativeHandle(); }
	virtual size_t GetMemoryInUse() const override { return sizeof(*this); }

	/// Указывает нужно ли трансформировать текстурные координаты. Если да, то необходимо вызвать TranslateUV перед отрисовкой.
	virtual bool needTranslate() const override { return true; }
	/// Трансформация текстурных координат.
	virtual void TranslateUV(FRect &rect, FRect &uv) const override;

	/// Забиндить текстуру
	virtual void Bind(int channel = 0, int stageOp = 0) override;

	virtual bool isPixelTransparent(int x, int y) const override;
	
	/// Получить текстуру атласа
	Render::Texture* GetBaseTexture() const { return texture.get(); }

	/// Получить параметры изображения
	void GetImageParameters(int16_t& baseX, int16_t& baseY, int16_t& width, int16_t& height, int16_t& innerX, int16_t& innerY, int16_t& frameWidth, int16_t& frameHeight) const;

	// Перенаправление вызовов текстуре атласа
	//
	virtual void Load(ResourceLoadMode load_mode) override;
	virtual void Unload(ResourceLoadMode load_mode) override;
	virtual void GroupLoad(ResourceLoadManager& glm) override;
	virtual void GroupUnload(ResourceLoadManager& glm) override;
	
protected:
	virtual void LoadFromFile(const std::string& filename) override { }
	virtual void LoadFromImage(const Image& image) override { }
	virtual void GetPropertiesFromFile(const std::string& filename) override { }

	/// выгрузить текстуру
	virtual void Release() override;

protected:
	/// Текстура атласа
	TexturePtr texture;
	/// Положение в базовой текстуре
	int16_t baseX, baseY, width, height, innerX, innerY, frameWidth, frameHeight;
	int16_t alphaLimit;
};

typedef boost::intrusive_ptr<PartialTexture> PartialTexturePtr;

} // namespace Render

#endif // __PARTIALTEXTURE_H__
