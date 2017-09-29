#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

namespace math {

class Vector3;
class Quaternion;

///
/// Матрица трансформации 4x4.
///
class BOOST_ALIGNMENT(16) Matrix4 {
public:
	static const Matrix4 Identity;
		/// Единичная матрица
	
	static const Matrix4 Zero;
		/// Нулевая матрица
	
	Matrix4();
	
	Matrix4(float* pv);
	
	Matrix4(
		float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44
	);
	
	float& operator () (size_t row, size_t col);
	
	float operator () (size_t row, size_t col) const;
	
	operator float* ();
	
	operator const float* () const;
	
	Matrix4 operator - () const;
	
	Matrix4 operator * (const Matrix4& other) const;
	
	Matrix4 operator + (const Matrix4& other) const;
	
	Matrix4 operator - (const Matrix4& other) const;
	
	Matrix4 operator * (float value) const;
	
	Matrix4 operator / (float value) const;
	
	Matrix4& operator *= (const Matrix4& other);
	
	Matrix4& operator += (const Matrix4& other);
	
	Matrix4& operator -= (const Matrix4& other);
	
	Matrix4& operator *= (float value);
	
	Matrix4& operator /= (float value);
	
	bool operator == (const Matrix4& other) const;
	
	bool operator != (const Matrix4& other) const;
	
	bool Equals(const Matrix4& other, float delta = 0.00001f) const;
		/// Проверяет, равна ли матрица указанной
	
	Matrix4 Transpose() const;
		/// Транспонирует матрицу
	
	Matrix4 Inverse(float* determinant = 0) const;
		/// Инвертирует матрицу.
		/// Если инвертирование невозможно, возвращает нулевую матрицу.
	
	Matrix4 Lerp(const Matrix4& other, float s) const;
		/// Производит линейную интерполяцию двух матриц
	
	float Determinant() const;
		/// Возвращает определитель матрицы
	
	bool IsIdentity() const;
		/// Проверяет, является ли матрица единичной
	
	bool IsZero() const;
		/// Проверяет, является ли матрица нулевой
	
	bool IsNaN() const;
		/// Проверяет, имеет ли матрица хотя бы один член, равный NaN
	
	bool IsInfinite() const;
		/// Проверяет, имеет ли матрица хотя бы один член, равный Inf
	
	Vector3 GetPosition() const;
		/// Возвращает компонент перемещения
	
	Vector3 GetScale() const;
		/// Возвращает компонент масштабирования
	
	void Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) const;
		/// Разделяет матрицу преобразования на компоненты SRT
	
	static Matrix4 Compose(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
		/// Создаёт матрицу преобразования из компонентов SRT
	
	static Matrix4 RotationX(float angle);
		/// Создаёт матрицу поворота вокруг оси X
	
	static Matrix4 RotationY(float angle);
		/// Создаёт матрицу поворота вокруг оси Y
	
	static Matrix4 RotationZ(float angle);
		/// Создаёт матрицу поворота вокруг оси Z
	
	static Matrix4 RotationYawPitchRoll(float yaw, float pitch, float roll);
		/// Создаёт матрицу поворота из углов Эйлера
	
	static Matrix4 RotationYawPitchRoll(const math::Vector3& angles);
		/// Создаёт матрицу поворота из углов Эйлера
	
	static Matrix4 RotationAxis(Vector3 axis, float angle);
		/// Создаёт матрицу поворота вокруг вектора
	
	static Matrix4 RotationQuaternion(const Quaternion& q);
		/// Создаёт матрицу поворота из кватерниона
	
	static Matrix4 Scaling(float sx, float sy, float sz);
		/// Создаёт матрицу масштабирования
	
	static Matrix4 Scaling(const Vector3& scale);
		/// Создаёт матрицу масштабирования
	
	static Matrix4 Translation(float x, float y, float z);
		/// Создаёт матрицу перемещения
	
	static Matrix4 Translation(const Vector3& translate);
		/// Создаёт матрицу перемещения
	
	static Matrix4 TextureTranslation(float x, float y);
		/// Создаёт матрицу смещения текстурных координат
	
	static Matrix4 TextureScaling(float sx, float sy);
		/// Создаёт матрицу масштабирования текстурных координат
	
	static Matrix4 TextureScalingCenter(float sx, float sy);
		/// Создаёт матрицу масштабирования текстурных координат относительно центра текстуры
	
	static Matrix4 PerspectiveFovLH(float fieldOfView, float aspectRatio, float zNear, float zFar);
		/// Создаёт матрицу перспективной проекции в левосторонней системе координат
	
	static Matrix4 PerspectiveOffCenterLH(float left, float top, float right, float bottom, float zNear, float zFar);
		/// Создаёт матрицу перспективной проекции в левосторонней системе координат

	static Matrix4 PerspectiveOffCenterRH(float left, float top, float right, float bottom, float zNear, float zFar);
		/// Создаёт матрицу перспективной проекции в правосторонней системе координат

	static Matrix4 LookAt(const Vector3& eye, const Vector3& at, const Vector3& up);
		/// Создаёт look-at матрицу

	static Matrix4 LookAtRH(const Vector3& eye, const Vector3& at, const Vector3& up);
		/// Создаёт правостороннюю look-at матрицу

public:
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
		float v[16];
	};
};

/** Умножение матриц, выровненных на 16 байт **/
Matrix4 aligned16Mul(const Matrix4 &, const Matrix4 &);

} // namespace math

inline std::ostream& operator<<(std::ostream& os, const math::Matrix4& m)
{
	os << m.v[0];
	for (int i = 1; i < 16; ++i) {
		os << " " << m.v[i];
	}
	return os;
}

#endif // __MATRIX4_H__
