#pragma once

#include <Utils/Vector3.h>

struct Vector2f
{
	float x = 0, y = 0;

	Vector2f()
	{
	}

	Vector2f(float a_x, float a_y):
		x(a_x),
		y(a_y)
	{
	}

	//! addition
	const Vector2f operator +(const Vector2f &v) const
	{
		return Vector2f(x+v.x, y+v.y);
	}

	//! subtraction
	const Vector2f operator -(const Vector2f &v) const
	{
		return Vector2f(x-v.x, y-v.y);
	}

	//! uniform scaling
	const Vector2f &operator *=(const float num)
	{
		x*=num; y*=num;
		return *this;
	}

	//! uniform scaling
	const Vector2f &operator /=(const float num)
	{
		x/=num; y/=num;
		return *this;
	}

	operator math::Vector3()
	{
		return math::Vector3(x, y, 0);
	}
};
