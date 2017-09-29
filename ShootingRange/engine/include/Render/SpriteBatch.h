#ifndef __SPRITEBATCH_H__
#define __SPRITEBATCH_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif // _MSC_VER

#include "RenderDevice.h"
#include "Utils/Color.h"
#include "Utils/IRect.h"
#include "RefCounter.h"

#include <boost/intrusive_ptr.hpp>

namespace math {
	class Vector3;
};

///
/// Перечисляет режимы сортировки спрайтов
///
enum class SpriteSortMode {
	// Накапливает спрайты во внутреннем буфере, при завершении отрисовывает непрерывными частями
	Deferred,
	// Сразу отрисовывает спрайты по одному
	Immediate,
	// Накапливает спрайты во внутреннем буфере, затем сортирует по текстурам
	Texture,
	// Накапливает спрайты во внутреннем буфере, затем сортирует по шейдерам
	Shader,
	// Накапливает спрайты во внутреннем буфере, затем сортирует по режиму смешивания
	Blend,
	// Накапливает спрайты во внутреннем буфере, затем сортирует от ближнего к дальнему
	BackToFront,
	// Накапливает спрайты во внутреннем буфере, затем сортирует от дальнего к ближнему
	FrontToBack
};

///
/// Перечисляет режимы трансформирования спрайтов
///
enum class SpriteTransformMode {
	// Без трансформирования
	None,
	// Применять текущую трансформацию из устройства
	Auto
};

namespace Render {

class Material;

struct SpriteBatchStat {
	size_t num_instances;
	size_t num_draw_calls;
	size_t num_dip_calls;
};

///
/// Серия спрайтов
///
class SpriteBatch : public RefCounter {
public:
	SpriteBatch();
	~SpriteBatch();
	
	/// Начинает вывод спрайтов
	void Begin(SpriteSortMode sortMode = SpriteSortMode::Deferred, SpriteTransformMode transformMode = SpriteTransformMode::None);
	
	/// Рисует спрайт, преобразует координаты с помощью TranslateUV
	/// Параметр textureRect определяет прямоугольник текстуры в пикселях [0; width/height]
	void DrawRect(const Material& material, const FRect& textureRect, const FRect& screenRect, float z);

	// Все эти методы - варианты вызова обобщённого метода
	void DrawRect(Texture* texture, ShaderProgram* shader, BlendMode blend, const FRect& textureRect, const FRect& screenRect, float z = 0.0f);
	void DrawRect(Texture* texture, BlendMode blend, const FRect& textureRect, const FRect& screenRect);
	void DrawRect(Texture* texture, BlendMode blend, const IRect& textureRect, const math::Vector3& position);
	void DrawRect(Texture* texture, BlendMode blend, const FPoint& position);

	/// Рисует спрайт, не преобразует координаты с помощью TranslateUV
	/// Параметр uv определяет прямоугольник текстуры в нормализованных текстурных координатах [0; 1]
	void Draw(const Material& material, const math::Vector3 vec[4], const Color color[4], const FPoint uvs[4]);

	// Все эти методы - варианты вызова обобщённого метода
	void Draw(Texture* texture, ShaderProgram* shader, BlendMode blend, const math::Vector3 vec[4], const Color color[4], const FPoint uvs[4]);
	void Draw(Texture* texture, BlendMode blend, const math::Vector3 vec[4], const Color color[4], const FPoint uvs[4]);	
	void Draw(Texture* texture, BlendMode blend, const FRect& screenRect, const FRect& uv);
	void Draw(Texture* texture, BlendMode blend, const FRect& screenRect, const FRect& uv, Color color);
	void Draw(Texture* texture, BlendMode blend, const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3, const math::Vector3& v4, Color color, const FRect& uv);
	void Draw(Texture* texture, BlendMode blend, const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3, const math::Vector3& v4, const Color c[4], const FRect& uv);

	/// Принудительно отрисовывает закешированные спрайты
	void Flush();
	
	/// Заканчивает вывод спрайтов
	void End();

	/// Пересоздаёт внутренние графические объекты
	static void Reset();
	/// Удаляет внутренние графические объекты
    static void Shutdown();

	static void ClearStat();
	static const SpriteBatchStat* GetStat();

protected:
	class Impl;
	std::unique_ptr<Impl> _impl;
	
	SpriteTransformMode _transformMode;

private:
	SpriteBatch(const SpriteBatch&) = delete;
	SpriteBatch& operator=(const SpriteBatch&) = delete;
};

typedef boost::intrusive_ptr<SpriteBatch> SpriteBatchPtr;

} // namespace Render

#endif // __SPRITEBATCH_H__
