#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

namespace math {

class Matrix4;

///
/// Трёхмерный вектор.
///
class Vector3 {
public:
	static const Vector3 Zero;
		/// Нулевой вектор
	
	static const Vector3 One;
		/// Единичный вектор
	
	static const Vector3 UnitX;
		/// Единичный вектор, направленный вдоль оси X
	
	static const Vector3 UnitY;
		/// Единичный вектор, направленный вдоль оси Y
	
	static const Vector3 UnitZ;
		/// Единичный вектор, направленный вдоль оси Z

	Vector3();
	
	explicit Vector3(float n);
	
	explicit Vector3(const float* pv);
	
	Vector3(float x, float y, float z);
	
	Vector3(const Vector3& other);
	
	float& operator () (size_t item);
	
	float operator () (size_t item) const;
	
	explicit operator float* ();
	
	explicit operator const float* () const;
	
	Vector3& operator = (const Vector3& other);
	
	Vector3 operator - () const;
	
	Vector3 operator + (float value) const;
	
	Vector3 operator + (const Vector3& other) const;
	
	Vector3 operator - (float value) const;
	
	Vector3 operator - (const Vector3& other) const;
	
	Vector3 operator * (float value) const;
	
	Vector3 operator * (const Vector3& other) const;
	
	Vector3 operator / (float value) const;
	
	Vector3 operator / (const Vector3& other) const;
	
	Vector3& operator += (float value);
	
	Vector3& operator += (const Vector3& other);
	
	Vector3& operator -= (float value);
	
	Vector3& operator -= (const Vector3& other);
	
	Vector3& operator *= (float value);
	
	Vector3& operator *= (const Vector3& other);
	
	Vector3& operator /= (float value);
	
	Vector3& operator /= (const Vector3& other);
	
	bool operator < (const Vector3& other) const;
	
	bool operator <= (const Vector3& other) const;
	
	bool operator > (const Vector3& other) const;
	
	bool operator >= (const Vector3& other) const;
	
	bool operator == (const Vector3& other) const;
	
	bool operator != (const Vector3& other) const;

	inline float operator ! () const { return Length(); }
	
	bool Equals(const Vector3& other, float delta = 0.00001f) const;
		/// Проверяет, равен ли вектор указанному
	
	float Length() const;
		/// Возвращает длину вектора
	
	float LengthSq() const;
		/// Возвращает квадрат длины вектора
	
	void SetLength(float length);
		/// Устанавливает длину вектора
	
	void Normalize();
		/// Нормализует вектор

	Vector3 Normalized() const;
		/// Возвращает нормализованную копию вектора
	
	float Distance(const Vector3& other) const;
		/// Возвращает расстояние до указанного вектора
	
	float DistanceSq(const Vector3& other) const;
		/// Возвращает квадрат расстояния до указанного вектора
	
	float DotProduct(const Vector3& other) const;
		/// Возвращает скалярное произведение на указанный вектор

	Vector3 CrossProduct(const Vector3& other) const;
		/// Возвращает векторное произведение на указанный вектор
	
	Vector3 TransformCoord(const Matrix4& transform) const;
		/// Трансформирует вектор как точку в пространстве

	Vector3 TransformCoordW(const Matrix4& transform) const;
		/// Трансформирует вектор как точку в пространстве с учётом W
	
	Vector3 TransformNormal(const Matrix4& transform) const;
		/// Трансформирует вектор как нормаль, т.е. без перемещения
	
	Vector3 Lerp(const Vector3& other, float s) const;
		/// Производит линейную интерполяцию двух векторов

public:
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float v[3];
	};
};

Vector3	operator * (float f, const Vector3& v);
float operator & (const Vector3& u, const Vector3& v);
float GetXYVectorAngle(math::Vector3 v1, math::Vector3 v2);
Vector3	operator ^ (const Vector3& u, const Vector3& v);

} // namespace math

void RotateVector(math::Vector3& v, float angle);
math::Vector3 SplineInterpolation(const math::Vector3& v1, const math::Vector3& v2, 
                                  const math::Vector3& g1, const math::Vector3& g2, float t);
math::Vector3 GetGradient(const math::Vector3& v1, const math::Vector3& v2,
                          const math::Vector3& g1, const math::Vector3& g2, float t);

inline std::ostream& operator<<(std::ostream& os, const math::Vector3& v)
{
	os << v.x << " " << v.y << " " << v.z;
	return os;
}

#endif // __VECTOR3_H__
