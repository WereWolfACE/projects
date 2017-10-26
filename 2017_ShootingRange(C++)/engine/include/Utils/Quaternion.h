#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

namespace math {

class Vector3;
class Matrix4;

///
/// Кватернион Гамильтона.
///
class Quaternion {
public:
	static const Quaternion Identity;
		/// Единичный кватернион

	static const Quaternion Zero;
		/// Нулевой кватернион

	Quaternion();

	Quaternion(float x, float y, float z, float w);

	explicit Quaternion(float* pv);

	Quaternion(float yaw, float pitch, float roll);

	Quaternion(Vector3 axis, float angle);

	explicit Quaternion(const Matrix4& rotation);

	float& operator () (size_t item);

	float operator () (size_t item) const;

	operator float* ();

	operator const float* () const;

	Quaternion operator * (const Quaternion& other) const;

	bool operator == (const Quaternion& other) const;

	bool operator != (const Quaternion& other) const;

	bool Equals(const Quaternion& other, float delta = 0.00001f) const;
		/// Проверяет, равен ли кватернион указанному

	float Length() const;
		/// Возвращает длину кватерниона

	float LengthSq() const;
		/// Возвращает квадрат длины кватерниона

	Quaternion& Normalize();
		/// Нормализует кватернион

	Quaternion Conjugate() const;
		/// Возвращает сопряжённый кватернион

	Quaternion Slerp(const Quaternion& other, float s) const;
		/// Сферическая интерполяция между данным и указанным кватернионами

	Matrix4 RotationMatrix() const;
		/// Возвращает матрицу поворота

public:
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float v[4];
	};
};

} // namespace math

inline std::ostream& operator<<(std::ostream& os, const math::Quaternion& q)
{
	os << q.x << " " << q.y << " " << q.z << " " << q.w;
	return os;
}

#endif // __QUATERNION_H__
