#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#pragma once

#include "Platform/TargetPlatforms.h"
#include "Render/RenderTypes.h"
#include "Render/Bindable.h"
#include "Render/Drawable.h"
#include "Render/Image.h"
#include "Utils/FPoint.h"
#include "Utils/Math.hpp"
#include "Utils/FRect.h"
#include "Utils/IRect.h"
#include "Utils/OpacityMask.h"
#include "Core/Resource.h"

#include <boost/smart_ptr.hpp>

namespace Core
{
	class ResourceManager;
}

#if defined(ENGINE_TARGET_WIN32)
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#	define DEVTEXTURE GLuint
#endif

#if defined(ENGINE_TARGET_IPHONE)
#	include <OpenGLES/ES2/gl.h>
#	define DEVTEXTURE GLuint
#endif

#if defined(ENGINE_TARGET_ANDROID)
#	include <GLES/gl.h>
#	include <GLES/glext.h>
#	define DEVTEXTURE GLuint
#endif

#ifdef ENGINE_TARGET_MACOS
#	include <OpenGL/OpenGL.h>
#	define DEVTEXTURE GLuint
#endif

class TextureLoader;

namespace Render {

	class RenderDeviceGLES2;
	class PartialTexture;
	class Combinedtexture;

	/// Класс текстуры
	class Texture : public Resource, public Bindable, public Drawable
	{
		friend class RenderDeviceGLES2;
		friend class PartialTexture;
		friend class CombinedTexture;
		friend class ::TextureLoader;

	public:
		Texture();
		Texture(DEVTEXTURE tex, int width, int height, EnginePixelType pixelType);
		Texture(const std::string& filename, int alphaLimit = 0);
		Texture(const Image &image, int alphaLimit = 0);
		Texture(int width, int height, unsigned char fill);

		virtual ~Texture();

		virtual const std::string& GetName() const override { return textureID; }
		virtual size_t GetMemoryInUse() const override;

		/// Возвращает ширину текстуры
		virtual int Width() const override { return static_cast<int>(_bitmap_width); }
		/// Возвращает высоту текстуры
		virtual int Height() const override { return static_cast<int>(_bitmap_height); }

		virtual bool HitTest(int x, int y) const override { return isPixelOpaque(x, y); }

		virtual void Draw(SpriteBatch* batch, const FPoint& position) override;

		virtual void GetGeometry(std::vector<QuadVert>& geometry, float z = 0, Color color = Color::WHITE) const override;

		/// Возвращает размер изображения в текстуре
		IRect getBitmapRect() const { return IRect(0, 0, _bitmap_width, _bitmap_height); }

		/// Текстура не загружена или принудительно выгружена.
		/// Принудительно выгружаются динамические текстуры при переинициализации RenderDeviceImpl. См. Texture::setDynamic
		bool Empty() const override;

		/// забиндить текстуру в заданный канал
		virtual void Bind(int channel = 0, int stageOp = 0) override;

		virtual const void* GetNativeHandle() const override { return reinterpret_cast<const void*>(_tex); }

		virtual TextureType GetTextureType() const override { return TextureType::Texture; }

		void setAlphaLimit(uint8_t limit) { _alphaLimit = limit; }
		void setUseDithering(bool dither) { _useDithering = dither; }

		// Установить формат пикселя
		void setPixelType(EnginePixelType pixelType) { _pixelType = pixelType; }
        EnginePixelType getPixelType() const { return (EnginePixelType)_pixelType; }

		void setPurgeAfterLoading(bool value) { _purgeAfterLoading = value; }
		bool getPurgeAfterLoading() const { return _purgeAfterLoading; }

		virtual void GetPropertiesFromFile(const std::string& filename);

		/// Указывает, нужно ли трансформировать текстурные координаты. Если да, то необходимо вызвать TranslateUV перед отрисовкой.
		virtual bool needTranslate() const;
		/// Трансформация текстурных координат.
		virtual void TranslateUV(FRect &rect, FRect &uv) const;

		/// Заполняет готовый к отрисовке квад
		void FillQuad(QuadVert quad[4], const FRect& screenRect, const FRect& textureRect);

		/// Отрисовка текстуры в заданной позиции. Если текстура ещё не забиндена, она биндится.
		/// Для uv-координат применяется TranslateUV.
		virtual void DrawBinded(const FPoint& position, const RectOrient orient);
		virtual void DrawBinded(const FRect& rect, const FRect& uv);
		virtual void DrawRectBinded(const FRect& screenRect, const FRect& textureRect);
		
		virtual void Draw(const FPoint& position, const RectOrient orient);
		virtual void Draw(const FRect& rect, const FRect& uv);
		virtual void DrawRect(const FRect& screenRect, const FRect& textureRect);

		void DrawRect(const FPoint& position, const FRect& textureRect) {
			FRect screenRect(position.x, position.x + (textureRect.xEnd - textureRect.xStart), position.y, position.y + (textureRect.yEnd - textureRect.yStart));
			DrawRect(screenRect, textureRect);
		}
		
		void Draw(const IPoint& p = IPoint()) {
			Draw(FRect(float(p.x), float(p.x) + _bitmap_width, float(p.y), float(p.y) + _bitmap_height), FRect(0.f, 1.f, 0.f, 1.f));
		}
		
		void Draw(float x, float y) {
			Draw(FRect(x, x + _bitmap_width, y, y + _bitmap_height), FRect(0.f, 1.f, 0.f, 1.f));
		}
		
		void Draw(int x, int y) {
			Draw(FRect(float(x), float(x) + _bitmap_width, float(y), float(y) + _bitmap_height), FRect(0.f, 1.f, 0.f, 1.f));
		}
		
		void Draw(const FPoint& p) {
			Draw(FRect(p.x, p.x + _bitmap_width, p.y, p.y + _bitmap_height), FRect(0.f, 1.f, 0.f, 1.f));
		}
		
		void Draw(const IRect& rect, float xStart, float xEnd, float yStart, float yEnd) {
			Draw(FRect(rect), FRect(xStart, xEnd, yStart, yEnd));
		}
		
		void Draw(const IRect& rect, const FRect& uv) {
			Draw(FRect(rect), uv);
		}
		
		void Draw(float x, float y, float width, float height, const FRect& uv) {
			Draw(FRect(x, x + width, y, y + height), uv);
		}
		
		void DrawCut(const IPoint& p, float cut_factor_x, float cut_factor_y = 1.0f) {
			Draw(FRect(float(p.x), float(p.x) + _bitmap_width * cut_factor_x, float(p.y), float(p.y) + _bitmap_height * cut_factor_y),
				FRect(0.f, cut_factor_x, 0.f, cut_factor_y));
		}
		
		void DrawCut(const FPoint& p, float cut_factor_x, float cut_factor_y = 1.0f) {
			Draw(FRect(p.x, p.x + _bitmap_width * cut_factor_x, p.y, p.y + _bitmap_height * cut_factor_y),
				FRect(0.f, cut_factor_x, 0.f, cut_factor_y));
		}
		
		void Draw(const IPoint& position, const RectOrient orient) {
			Draw(FPoint(position), orient);
		}

		virtual bool isPixelTransparent(int x, int y) const;

		bool isPixelOpaque(int x, int y) const { return !isPixelTransparent(x, y); }
		bool isPixelOpaque(const IPoint& p) const { return !isPixelTransparent(p.x, p.y); }
		
		virtual DEVTEXTURE Detach();

		static Texture* CreateFromXml(rapidxml::xml_node<>* elem);
		
	protected:
		void LoadFromFileWithoutMask(const std::string& filename);
		virtual void LoadFromFile(const std::string& filename);
		virtual void LoadFromImage(const Image& image);

		/// Удаляет данные изображения
		virtual void Purge();

		/// Загружает текстуру
		virtual void Upload(bool bCleanAfterUpload = true);
		/// Выгружает текстуру
		virtual void Release();

	public:
		// Работа с alphaMask
		//
		void CreateAlphaMask(int alphaLimit);
		void CreateAlphaMask(const Render::Image &image, int alphaLimit);
		void LoadAlphaMask(const std::string& filename);
		void ReleaseAlphaMask();

	public:
		std::string textureID;
		std::string path;

		Image image;

		float _prescale_x, _prescale_y;

		/// Размер текстуры в видеопамяти
		int16_t _rect_width, _rect_height;
		/// Размер изображения в текстуре
		int16_t _bitmap_width, _bitmap_height;

	protected:
		DEVTEXTURE _tex; ///< ID текстуры в видеопамяти

		EnginePixelType _pixelType; ///< Формат хранения цвета пикселя
		uint8_t _alphaLimit; ///< Пороговое значение для альфа-маски

		std::unique_ptr<utils::OpacityMask> mask;

		bool _wasUploaded       : 1; ///< Была ли текстура загружена
		bool _useDithering      : 1; ///< Использовать дизеринг при конвертации изображения из большего bpp в меньшее
		bool _purgeAfterLoading : 1; ///< Удалять данные изображения после загрузки текстуры

	private:
		// Texture is not copyable
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
	};

	typedef boost::intrusive_ptr<Texture> TexturePtr;

} // namespace Render

#endif // __TEXTURE_H__
