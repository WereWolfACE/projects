#ifndef __ARCBALL_H__
#define __ARCBALL_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#include "Utils/Vector3.h"
#include "Utils/Matrix4.h"
#include "Utils/Quaternion.h"

class IPoint;

namespace SceneGraph {

///
///  ласс дл€ пространственного поворота мышью.
///
class Arcball {
public:
	Arcball();
	
	void SetBounds(int width, int height);

	void MouseDown(const IPoint& mouse_pos);
	void MouseMove(const IPoint& mouse_pos);
	void MouseUp();
	
	const math::Quaternion& GetRotation() const { return _rotation; }
	math::Matrix4 GetRotationMatrix() const;
	
	void Reset();

private:
	void MapToSphere(const IPoint& pt, math::Vector3& vec) const;

private:
	float _adjustWidth;
	float _adjustHeight;
	math::Vector3 _clickVector;
	math::Vector3 _dragVector;
	math::Quaternion _rotation;
	bool _clicked;
};

} // namespace SceneGraph

#endif // __ARCBALL_H__
