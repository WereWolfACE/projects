#ifndef __LIGHT_H__
#define __LIGHT_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "SceneGraph/ColorValue.h"
#include "Utils/Vector3.h"
#include "RefCounter.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Источник света.
///
class Light : public RefCounter {
public:
	typedef boost::intrusive_ptr<Light> HardPtr;
	
	///
	/// Тип источника света.
	///
	enum Type {
		Point,
			/// Точечный
		Spot,
			/// Пятно
		Directional
			/// Направленный
	};
	
	virtual ~Light();
	
	virtual Type GetType() const = 0;
		/// Возвращает тип источника света
	
	virtual Light::HardPtr Clone() = 0;
		/// Клонирует экземпляр объекта

	virtual size_t GetMemoryInUse() const { return sizeof(*this); }
	
	bool Enabled() const;
		/// Активен ли источник света
	
	void Enable(bool enable);
		/// Включает или выключает источник света
	
	const math::Vector3& GetPosition() const;
		/// Возвращает положение источника света
	
	void SetPosition(const math::Vector3& position);
		/// Устанавливает положение источника света
	
	const math::Vector3& GetDirection() const;
		/// Возвращает направление источника света
	
	void SetDirection(const math::Vector3& direction);
		/// Устанавливает направление источника света
	
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
	
protected:
	math::Vector3 _position;
		/// Положение источника света
	
	math::Vector3 _direction;
		/// Направление источника света
	
	ColorValue _ambient;
		/// Значение окружающего цвета
	
	ColorValue _diffuse;
		/// Значение рассеиваемого цвета
	
	ColorValue _specular;
		/// Значение отражаемого цвета
	
	bool _enabled;
		/// Активен ли источник света
	
	Light();
};

} // namespace SceneGraph

#endif // __LIGHT_H__
