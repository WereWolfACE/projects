#ifndef _FPOINT_H_
#define _FPOINT_H_

#include "Utils/IPoint.h"

/// Класс работы с двумерными точками с координатами типа float.
/// Инвариант: значения по модулю не больше максимального целого числа,
/// представимого типом float абсолютно точно.
class FPoint
{
private:

	/// от -INT_FLOAT_RANGE до INT_FLOAT_RANGE все целые числа
	/// представляются типом float абсолютно точно
	static const int INT_FLOAT_RANGE = 16777216;

public:

	/// x-координата точки
	float x;

	/// y-координата точки
	float y;

	/// Конструктор по умолчанию - заполнение нулями.
	FPoint() : x(0.f), y(0.f) {}

	/// Конструктор из значений отдельных координат.
	FPoint(float x, float y);

	/// Конструктор получения из целочисленной точки.
	FPoint(const IPoint &p); // non-explicit

	/// Получение x и y компонент.
	/// Не GetX и GetY - это намёк использовать операции над FPoint,
	/// а не реализовывать покомпонентные операции самостоятельно.
	void GetXY(float &x, float &y) const;

	/// Округление к целочисленной точке.
	IPoint Rounded() const;

	/// Округление до целых координат.
	void Round();

	/// Вычитание точки.
	FPoint operator - (const FPoint& second) const { return FPoint(x - second.x, y - second.y); }

	/// Добавление точки.
	FPoint operator + (const FPoint& second) const { return FPoint(x + second.x, y + second.y); }

	/// Вычитание с присваиванием.
	FPoint& operator -= (const FPoint& second) { x -= second.x; y -= second.y; return *this; }

	/// Добавление с присваиванием.
	FPoint& operator += (const FPoint& second) { x += second.x; y += second.y; return *this; }

	FPoint& operator *= (float f) { x *= f; y *= f; return *this; }
	FPoint& operator /= (float f) { x /= f; y /= f; return *this; }

	/// Унарный минус.
	FPoint operator - () const { return FPoint(-x, -y); }

	/// Сравнение точек на точное (!) равенство.
	/// Равенство можно гарантировать только после присваивания,
	/// но не как результат вычисления по формулам.
	bool operator == (const FPoint& second) const { return (x == second.x) && (y == second.y); }

	/// Сравнение точек на точное (!) неравенство.
	/// Неравные на самом деле точки как правило неравны и при точном сравнении.
	/// См. также замечание к сравнению на равенство.
	bool operator != (const FPoint& second) const { return (x != second.x) || (y != second.y); }

	/// Вращение вокруг начала координат против часовой стрелки на angle радиан.
	FPoint Rotated(float angle) const;

	void Rotate(float angle);

	/// Растяжение относительно начала координат: X в xScale раз, Y в yScale раз.
	FPoint Scaled(float xScale, float yScale) const { return FPoint(x * xScale, y * yScale); }

	void Scale(float xScale, float yScale);

	/// Получение расстояния от точки до начала координат.
	float GetDistanceToOrigin() const;

	/// Получение расстояния от точки до начала координат.
	float Length() const { return GetDistanceToOrigin(); }

	/// Получение расстояния от этой точки до другой.
	float GetDistanceTo(const FPoint& second) const;

	/// Получение угла точки в полярной системе координат
	/// Ошибка, если точка совпадает с началом координат.
	/// Результат - угол в радианах, может быть отрицательным.
	/// Угол нулевой у точки (1, 0).
	float GetAngle() const;

	/// Получение ориентированного угла поворота от текущей точки ко второй
	/// против часовой стрелки относительно начала координат.
	/// Ошибка, если какая-нибудь точка совпадает с началом координат.
	/// Результат - угол в радианах, может быть отрицательным.
	float GetDirectedAngle(const FPoint& second) const;

	/// Аналог GetDirectedAngle, приводящий результат к [0; 2PI).
	float GetDirectedAngleNormalize(const FPoint& second) const;

	/// Z-координата векторного произведения двух векторов,
	/// идущих из начала координат в эту и вторую точку.
	///
	/// Модуль получаемого числа является площадью параллелограмма
	/// с вершинами в 0, this, second и this + second.
	float GetVectorProduct(const FPoint& second) const;

	/// Вернуть скалярное произведение
	float GetDotProduct(const FPoint& second) const;

	/// Нормализует вектор
	FPoint Normalized() const;

	/// Нормализует вектор на месте
	void Normalize();
};

/// Умножение скаляра на точку.
inline FPoint operator * (float scale, const FPoint& p) { return p.Scaled(scale, scale); }

/// Умножение точки на скаляр.
inline FPoint operator * (const FPoint& p, float scale) { return p.Scaled(scale, scale); }

/// Деление точки на скаляр.
inline FPoint operator / (const FPoint& p, float scale) { Assert(scale != 0); return p.Scaled(1.0f / scale, 1.0f / scale); }

/// Умножение на другой вектор
inline FPoint operator * (const FPoint& p1, const FPoint& p2) { return p1.Scaled(p2.x, p2.y); }

inline std::ostream& operator<<(std::ostream& os, const FPoint& p)
{
	os << p.x << " " << p.y;
	return os;
}

#endif
