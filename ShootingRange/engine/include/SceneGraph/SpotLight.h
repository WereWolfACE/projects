#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "PointLight.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Конусообразный источник света (световое пятно).
///
class SpotLight : public PointLight {
public:
	typedef boost::intrusive_ptr<SpotLight> HardPtr;

	Light::HardPtr Clone();

	float GetInnerCone() const;
		/// Возвращает радиус внутреннего конуса источника света

	void SetInnerCone(float radians);
		/// Устанавливает радиус внутреннего конуса источника света

	float GetOuterCone() const;
		/// Возвращает радиус внешнего конуса источника света

	void SetOuterCone(float radians);
		/// Устанавливает радиус внешнего конуса источника света

	float GetFalloff() const;
		/// Возвращает коэффициент уменьшения освещения между внутренним и внешним световыми конусами

	void SetFalloff(float falloff);
		/// Устанавливает коэффициент уменьшения освещения между внутренним и внешним световыми конусами

	Type GetType() const;
		/// Возвращает тип источника света

	static SpotLight::HardPtr Create(const math::Vector3& position, const math::Vector3& direction);

protected:
	float _innerCone;
		/// Радиус внутреннего светового конуса в радианах

	float _outerCone;
		/// Радиус внешнего светового конуса в радианах

	float _falloff;
		/// Коэффициент уменьшения освещения между внутренним и внешним световыми конусами

	SpotLight(const math::Vector3& position, const math::Vector3& direction = math::Vector3(0.0f, -1.0f, 0.0f));
};

} // namespace SceneGraph

#endif // __SPOTLIGHT_H__
