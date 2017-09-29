#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "Light.h"

#include <boost/intrusive_ptr.hpp>

namespace SceneGraph {

///
/// Точечный источник света.
///
class PointLight : public Light {
public:
	typedef boost::intrusive_ptr<PointLight> HardPtr;

	Light::HardPtr Clone();

	float GetRange() const;
		/// Возвращает дистанцию до источника света, на которой он действует

	void SetRange(float range);
		/// Устанавливает дистанцию до источника света, на которой он действует

	float GetConstantAttenuation() const;
		/// Возвращает коэффициент постоянного ослабления

	void SetConstantAttenuation(float attenuation);
		/// Устанавливает коэффициент постоянного ослабления

	float GetLinearAttenuation() const;
		/// Возвращает коэффициент линейного ослабления

	void SetLinearAttenuation(float attenuation);
		/// Устанавливает коэффициент линейного ослабления

	float GetQuadraticAttenuation() const;
		/// Возвращает коэффициент квадратичного ослабления

	void SetQuadraticAttenuation(float attenuation);
		/// Устанавливает коэффициент квадратичного ослабления

	Type GetType() const;
		/// Возвращает тип источника света

	static PointLight::HardPtr Create(const math::Vector3& position);

protected:
	float _range;
		/// Дистанция от источника света, на которой источник действует

	float _attenuation0;
		/// Коэффициент постоянного ослабления света

	float _attenuation1;
		/// Коэффициент линейного ослабления света

	float _attenuation2;
		/// Коэффициент квадратичного ослабления света

	explicit PointLight(const math::Vector3& position);
};

} // namespace SceneGraph

#endif // __POINTLIGHT_H__
