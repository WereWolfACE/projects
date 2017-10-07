#pragma once

#include "Vector2f.h"

struct Matrix3x2
{
	float elm[6];

	explicit Matrix3x2(float (&a_elm)[6])
	{
		for(int l_i=0; l_i < 6; ++l_i)
		{
			elm[l_i] = a_elm[l_i];
		}
	}

	Matrix3x2(float m00, float m01, float m02,
	          float m10, float m11, float m12)
	{
	   elm[0]= m00, elm[1]= m01, elm[2]= m02;
	   elm[3]= m10, elm[4]= m11, elm[5]= m12;
	}

	//! multiplication by a vector
	Vector2f operator *(const Vector2f &v) const
	{
		return Vector2f(v.x*elm[0] + v.y*elm[1] + elm[2],
		                v.x*elm[3] + v.y*elm[4] + elm[5]);
	}

	//! multiplication by a scalar
	const Matrix3x2 &operator *=(const float s)
	{
		elm[0]*= s, elm[1]*= s, elm[2]*= s;
		elm[3]*= s, elm[4]*= s, elm[5]*= s;

		return *this;
	}

	//! division by a scalar
	const Matrix3x2 &operator /=(const float s)
	{
		elm[0]/= s, elm[1]/= s, elm[2]/= s;
		elm[3]/= s, elm[4]/= s, elm[5]/= s;

		return *this;
	}

	//! addition	
	const Matrix3x2 operator +(const Matrix3x2  &m) const
	{
		return Matrix3x2(elm[0] + m.elm[0], elm[1] + m.elm[1], elm[2] + m.elm[2],
		                 elm[3] + m.elm[3], elm[4] + m.elm[4], elm[5] + m.elm[5]);
	}

	//! addition
	const Matrix3x2 &operator +=(const Matrix3x2 &m)
	{
		elm[0]+= m.elm[0], elm[1]+= m.elm[1], elm[2]+= m.elm[2];
		elm[3]+= m.elm[3], elm[4]+= m.elm[4], elm[5]+= m.elm[5];

		return *this;
	}

	//! subtraction	
	const Matrix3x2 operator -(const Matrix3x2 &m) const
	{
		return Matrix3x2(elm[0] - m.elm[0], elm[1] - m.elm[1], elm[2] - m.elm[2],
		                 elm[3] - m.elm[3], elm[4] - m.elm[4], elm[5] - m.elm[5]);
	}

	//! subtraction
	const Matrix3x2 &operator -=(const Matrix3x2 &m)
	{
		elm[0]-= m.elm[0], elm[1]-= m.elm[1], elm[2]-= m.elm[2];
		elm[3]-= m.elm[3], elm[4]-= m.elm[4], elm[5]-= m.elm[5];

		return *this;
	}

	//! multiple
	/* — точки зрени€ линейной алгебры эта операци€ недопустима
	однако мы в уме держим что у нас матрица на самом деле 3x3 с последней строчкой
	0 0 1, на которую попросту не тратим лишние байты
	*/
	const Matrix3x2 operator *(const Matrix3x2 &m)
	{
		//„тоб ыне испытывать судьбу делаем так, потому как в таком случае все компил€торо моложе 2008 ода умеют уже оптимизировать RVO
		return Matrix3x2(
			elm[0] * m.elm[0] + elm[1] * m.elm[3], elm[0] * m.elm[1] + elm[1] * m.elm[4], elm[0] * m.elm[2] + elm[1] * m.elm[5] + elm[2],
			elm[3] * m.elm[0] + elm[4] * m.elm[3], elm[3] * m.elm[1] + elm[4] * m.elm[4], elm[3] * m.elm[2] + elm[4] * m.elm[5] + elm[5]
		);
	}

	const Matrix3x2 &operator *=(const Matrix3x2 &m)
	{
		Matrix3x2 l_tmp(
			elm[0] * m.elm[0] + elm[1] * m.elm[3], elm[0] * m.elm[1] + elm[1] * m.elm[4], elm[0] * m.elm[2] + elm[1] * m.elm[5] + elm[2],
			elm[3] * m.elm[0] + elm[4] * m.elm[3], elm[3] * m.elm[1] + elm[4] * m.elm[4], elm[3] * m.elm[2] + elm[4] * m.elm[5] + elm[5]
		);

		return *this = l_tmp;
	}

};
