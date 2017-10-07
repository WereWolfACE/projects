#ifndef __RENDERTARGET_H__
#define __RENDERTARGET_H__

#pragma once

#include "Render/Texture.h"
#include "Render/RenderTypes.h"
#include "Render/Bindable.h"
#include "Utils/IRect.h"

namespace Render {

//
// Объект, ведущий себя подобно текстуре, в который можно что-то рисовать.
//
// Особенности:
// 1) тот кто создал объект методом Render::RenderDeviceImpl::CreateRenderTarget,
//    обязан также удалить его методом Render::RenderDeviceImpl::DeleteRenderTarget;
// 2) при создании до вызова Render::device.BeginRenderTo таргет считается пустым (IsEmpty),
//    также он становится пустым при сбросе контекста на андроиде;
//
class Target : public Bindable
{
public:
	virtual ~Target();

	/// Возвращает ширину таргета
	int Width() const { return static_cast<int>(_width); }
	/// Возвращает высоту таргета
	int Height() const { return static_cast<int>(_height); }

	/// Возвращает рисуемую часть таргета
	IRect getBitmapRect() const { return IRect(0, 0, _width, _height); }

	/// Пустой ли таргет.
	/// Пустой таргет есть валидный таргет, но в него ещё не рисовали.
	virtual bool Empty() const override;

	///
	/// Установка текущей текстурой
	///
	virtual void Bind(int cannel = 0, int stageOp = 0) override;

	///
	/// Возвращает тип объекта
	///
	virtual TextureType GetTextureType() const override { return TextureType::Target; }

	///
	/// Возвращает идентификатор объекта в устройстве
	///
	virtual const void* GetNativeHandle() const override { return reinterpret_cast<const void*>(_colorBuffer); }

	///
	/// Рисование текстуры
	///
	void Draw(const FPoint& position);

	///
	/// Использует ли таргет мультисэмплинг
	///
	bool IsMultisampled() const;

	///
	/// Сохраняет содержимое в буфер.
	/// Работает только между вызовами BeginRenderTo()/EndRenderTo().
	/// textureRect не должен выходить за границы текстуры
	///
	bool CopyToImage(Image& image);
	bool CopyToImage(Image& image, const IRect &textureRect);

	///
	/// Сохранить в формате PNG
	/// Особенность: переворачивает текстуру по вертикали
	/// Работает только между вызовами BeginRenderTo()/EndRenderTo().
	/// textureRect не должен выходить за границы текстуры
	///
	void SaveAsPng(const std::string& path);
	void SaveAsPng(const std::string& path, const IRect &textureRect);

	///
	/// Сохранить в формате JPEG
	/// Работает только между вызовами BeginRenderTo()/EndRenderTo().
	/// textureRect не должен выходить за границы текстуры
	///
	void SaveAsJpeg(const std::string& path, int quality = 80);
	void SaveAsJpeg(const std::string& path, const IRect &textureRect, int quality = 80);

private:
	///
	/// Конструктор скрыт для всех кроме RenderDeviceImpl
	///
	Target(int width, int height);

	///
	/// Очистить ресурсы, которыми владеет объект
	///
	void Release();

private:
	friend class RenderDeviceGLES2;

	DEVTEXTURE _colorBuffer;	
	DEVTEXTURE _depthBuffer;
	DEVTEXTURE _stencilBuffer;

#if defined(ENGINE_TARGET_IPHONE)
    DEVTEXTURE _msRB;
    DEVTEXTURE _msDB;
    DEVTEXTURE _msFB;
#endif
	
	int16_t _width;      ///< ширина текстуры, должна быть степенью двойки
	int16_t _height;     ///< высота текстуры, должна быть степенью двойки

	EnginePixelType _pixelType; ///< Формат пикселя
	MultisampleType _msType;    ///< тип мультисэмплинга

	bool _empty : 1; ///< Пуста ли цель рендеринга (после создания ещё не было рисования в неё)
	bool _hasDepthBuffer : 1;
	bool _hasStencilBuffer : 1;
};

} // namespace Render

#endif // __RENDERTARGET_H__
