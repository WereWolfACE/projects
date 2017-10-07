#ifndef __AXISALIGNEDBOUNDINGBOX_H__
#define __AXISALIGNEDBOUNDINGBOX_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "Utils/Vector3.h"

namespace SceneGraph {

///
/// Выровненный по осям ограничивающий прямоугольник.
///
class AxisAlignedBoundingBox {
public:
	AxisAlignedBoundingBox();

	AxisAlignedBoundingBox(const math::Vector3& minEdge, const math::Vector3& maxEdge);

	explicit AxisAlignedBoundingBox(const math::Vector3& point);

	const math::Vector3& GetMinEdge() const;
		/// Возвращает ближний край прямоугольника

	const math::Vector3& GetMaxEdge() const;
		/// Возвращает дальний край прямоугольника

	math::Vector3 GetExtent() const;
		/// Возвращает протяжённость прямоугольника

	math::Vector3 GetCenter() const;
		/// Возвращает центр прямоугольника

	float GetVolume() const;
		/// Возвращает объём прямоугольника

	float GetArea() const;
		/// Возвращает площадь поверхности прямоугольника

	bool Intersects(const AxisAlignedBoundingBox& other) const;
		/// Определяет, пересекается ли данный прямоугольник с другим

	bool Inside(const AxisAlignedBoundingBox& other) const;
		/// Определяет, находится ли указанный прямоугольник внутри данного

	bool Inside(const math::Vector3& point) const;
		/// Определяет, находится указанная точка внутри данного прямоугольника

	void AddInternalPoint(const math::Vector3& point);
		/// Добавляет точку внутрь объёма

	void AddInternalPoint(float x, float y, float z);
		/// Добавляет точку внутрь объёма

	void AddInternalBox(const AxisAlignedBoundingBox& other);
		/// Добавляет прямоугольник внутрь объёма

	void Reset();
		/// Сбрасывает объём в (inf; -inf)

	void Reset(const math::Vector3& point);
		/// Сбрасывает объём в указанную точку

	void Reset(float x, float y, float z);
		/// Сбрасывает объём в указанную точку

	bool Intersects(const math::Vector3 &base, const math::Vector3 &dir) const;
		/// Определяет, пересекается ли данный бокс с лучом

	AxisAlignedBoundingBox Transform(const math::Matrix4& transform) const;
		/// Находит новый ограничивающий параллепипед после матричного преобразования

	bool Empty() const;
		/// Проверяет пустой ли бокс

private:
	math::Vector3 _minEdge;
		/// Ближний край

	math::Vector3 _maxEdge;
		/// Дальний край
};

} // namespace SceneGraph

#endif // __AXISALIGNEDBOUNDINGBOX_H__
