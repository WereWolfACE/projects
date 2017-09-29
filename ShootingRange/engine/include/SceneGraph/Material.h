#ifndef __SCENEGRAPH_MATERIAL_H__
#define __SCENEGRAPH_MATERIAL_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "ColorValue.h"
#include "Render/Texture.h"
#include "Render/Target.h"
#include "RefCounter.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Специальный "удаляльщик" для класса boost::shared_ptr<>, который
/// ничего не удаляет. Нужен для того, чтобы "оскопить" умный указатель, т.к.
/// менеджер ресурсов возвращает указатель, которым владеет он сам, и поэтому
/// самостоятельно удалять его НЕЛЬЗЯ.
///
struct null_deleter {
	void operator() (void*) const { }
};


typedef boost::intrusive_ptr<class Material> MaterialPtr;

///
/// Материал.
///
class Material : public RefCounter {
public:
	Material();

	//Material(const Material&);
	//Material& operator = (const Material&);
		// Конструктор копий и оператор присваивания подойдут те,
		// которые по умолчанию создаст компилятор.

	virtual ~Material();

	virtual MaterialPtr Clone();

	static MaterialPtr DefaultMaterial();
		/// Возвращает материал по умолчанию

	const ColorValue& GetAmbientColor() const;
		/// Возвращает значение окружающего цвета

	void SetAmbientColor(const ColorValue& color);
		/// Устанавливает значение окружающего цвета

	const ColorValue& GetDiffuseColor() const;
		/// Возвращает значение рассеиваемого цвета

	void SetDiffuseColor(const ColorValue& color);
		/// Устанавливает значение рассеиваемого цвета

	const ColorValue& GetSpecularColor() const;
		/// Возвращает значение отражаемого цвета

	void SetSpecularColor(const ColorValue& color);
		/// Устанавливает значение отражаемого цвета

	const ColorValue& GetEmissiveColor() const;
		/// Возвращает значение испускаемого цвета

	void SetEmissiveColor(const ColorValue& color);
		/// Устанавливает значение испускаемого цвета

	float GetPower() const;
		/// Возвращает экспоненту отражаемого цвета

	void SetPower(float power);
		/// Устанавливает экспоненту отражаемого цвета

	Render::TexturePtr GetDiffuseTexture() const;
		/// Возвращает объект текстуры

	void SetDiffuseTexture(Render::TexturePtr texture);
		/// Устанавливает объект текстуры

	void SetRenderTarget(Render::Target* target);
		/// Устанавливает объект таргета

	Render::Target* GetRenderTarget() const;
		/// Возвращает объект таргета

private:
	ColorValue _ambient;
		/// Значение окружающего цвета

	ColorValue _diffuse;
		/// Значение рассеиваемого цвета

	ColorValue _specular;
		/// Значение отражаемого цвета

	ColorValue _emissive;
		/// Значение испускаемого цвета

	float _power = 0;
		/// Значение экспоненты отражаемого цвета

	Render::TexturePtr _diffuseTexture;
		/// Текстура

	Render::Target *_target;
		/// Таргет
};

} // namespace SceneGraph

#endif // __SCENEGRAPH_MATERIAL_H__
