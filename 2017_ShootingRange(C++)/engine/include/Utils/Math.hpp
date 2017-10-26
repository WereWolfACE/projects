#ifndef __UTILS_MATH_H__
#define __UTILS_MATH_H__

#pragma once

#include "Utils/FPoint.h"
#include "Utils/FPoint.h"
#include "Utils/Vector3.h"
#include "Utils/Color.h"

#undef min
#undef max

/// Пространство имён для математических функций
namespace math {

extern const float PI;
extern const float OneByPI;
extern const float SqrtPI;

template <typename T>
inline T min(const T& v1, const T& v2) { return v1 < v2? v1 : v2; }

template <typename T>
inline T max(const T& v1, const T& v2) { return v1 > v2? v1 : v2; }
	
/// Округление к ближайшему числу
float round(float value);
	
/// Округление к меньшему числу
float floor(float value);

/// Округление к большему числу
float ceil(float value);

/// Получение модуля числа типа float
float abs(float value);

/// Получение модуля числа типа int
int abs(int value);

/// Получение знака (-1/0/1) числа типа float
int sign(float value);

/// Получение знака (-1/0/1) числа типа int
int sign(int value);

/// Косинус числа типа float
float cos(float value);
	
/// Синус числа типа float
float sin(float value);

/// Квадратный корень числа
float sqrt(float value);

/// Квадрат числа
inline float sqr(float value) { return value * value; }

/// Куб числа
inline float cube(float value) { return value * value * value; }

/// Четвёртая степень числа
inline float quad(float value) { float sqr = value * value; return sqr * sqr; }
	
/// Тангенс
float tan(float value);
	
/// Арктангенс
float atan(float y, float x);

float ease(float t, float a, float b);

// производная для ease
float ease_deriv(float t, float a, float b);

// функция, обратная к ease
float ease_inv(float y, float a, float b);

/// Возвращает точку на кривой Безье
/// start, end, control - начальная, конечная и контрольная точки
/// time - время от 0 до 1
FPoint simpleBezier(const FPoint& start, const FPoint& end, const FPoint& control, float time);

/// Возвращает left, если value < left, right, если value > right, иначе value.
/// Использовать можно только арифметические типы.
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
inline T clamp(T left, T right, T value) {
	return (value < left) ? left : (value > right) ? right : value;
}

template <typename T, typename R = typename std::decay<T>::type>
inline typename std::enable_if<!std::is_integral<R>::value, R>::type lerp(T v1, T v2, float t) {
	if (t <= 0.0f) return v1;
	if (t >= 1.0f) return v2;

	return v1 + (v2 - v1) * t;
}

template <typename T, typename R = typename std::decay<T>::type>
inline typename std::enable_if<std::is_integral<R>::value && std::is_signed<R>::value, R>::type lerp(T v1, T v2, float t) {
	if (t <= 0.0f) return v1;
	if (t >= 1.0f) return v2;

	return v1 + static_cast<R>((v2 - v1) * t);
}

template <typename T, typename R = typename std::decay<T>::type>
inline typename std::enable_if<std::is_integral<R>::value && std::is_unsigned<R>::value, R>::type lerp(T v1, T v2, float t) {
	if (t <= 0.0f) return v1;
	if (t >= 1.0f) return v2;

	return static_cast<R>(v1 * (1 - t) + v2 * t);
}

inline FPoint lerp(const FPoint& v1, const FPoint& v2, float t) {
	if (t <= 0.0f) return v1;
	if (t >= 1.0f) return v2;

	return FPoint(lerp(v1.x, v2.x, t), lerp(v1.y, v2.y, t));
}

inline Color lerp(const Color& v1, const Color& v2, float t) {
	if (t <= 0.0f) return v1;
	if (t >= 1.0f) return v2;

	return Color(
		lerp<int>(v1.red, v2.red, t),
		lerp<int>(v1.green, v2.green, t),
		lerp<int>(v1.blue, v2.blue, t),
		lerp<int>(v1.alpha, v2.alpha, t)
	);
}

/// Возвращает наименьшую степень двойки, бОльшую k.
int GetNextPow2(int k);

/// true, если i - степень двойки
bool IsPow2(int i);

/// Десятичный логарифм
float log10(float v);
    
/// Двоичный логарифм
float log2(float v);
    
// Натуральный логарифм
float log(float v);

/// Сравнивает два вещественных числа на приблизительное равенство
bool IsEqualFloat(float float1, float float2, float delta = 0.00001f);
	
/// Проверяет, является ли число Not a Number
bool IsFloatNaN(float f);
	
/// Возвращает значение Not a Number
float FloatQuietNaN();
	
/// Возвращает значение Infinity
float FloatInfinity();
	
/// Возвращает 1/value
float FloatReciprocal(float value);
	
/// Возвращает sqrt(value)
float FloatSqrt(float value);
	
/// Возвращает 1/sqrt(value)
float FloatReciprocalSqrt(float value);

} // nsmaspace math

#endif // __UTILS_MATH_H__
