#ifndef __SCENEGRAPH_UTILS_H__
#define __SCENEGRAPH_UTILS_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

namespace math {
	class Vector3;
}

namespace SceneGraph {

//
// Проверка пересечения луча из точки origin направлением dir со сферой радиуса radius в точке (0,0,0)
//
bool IntersectsSphere(const math::Vector3 &origin, const math::Vector3 &dir, float radius);

//
// Проверка пересечения луча из точки origin направлением dir с тругольником (v0,v1,v2)
//
bool IntersectsTriangle(const math::Vector3 &origin, const math::Vector3 &dir, const math::Vector3 &v0, const math::Vector3 &v1, const math::Vector3 &v2);

} // namespace SceneGraph

#endif // __UTILS_H__
