#ifndef UTILS_FRECT_H_INCLUDED
#define UTILS_FRECT_H_INCLUDED

#include "Utils/FPoint.h"

// Нецелочисленный прямоугольник
class FRect
{
public:
	// Конструктор по умолчанию (заполняет нулями)
	FRect();

	// Покомпонентный конструктор
	FRect(float xStart, float xEnd, float yStart, float yEnd);
	
	// Конструктор из целочисленного прямоугольника
	explicit FRect(const IRect& r);

	// Округление к целочисленному
	IRect Rounded() const;

	// Возвращает ширину прямоугольника
	float Width() const;
	
	// Возвращает высоту прямоугольника
	float Height() const;
	
	// Возвращает точку левого верхнего угла
	FPoint LeftTop() const;
	
	// Возвращает точку левого нижнего угла
	FPoint LeftBottom() const;
	
	// Возвращает точку правого верхнего угла
	FPoint RightTop() const;
	
	// Возвращает точку правого нижнего угла
	FPoint RightBottom() const;
	
	// Возвращает центральную точку прямоугольника
	FPoint CenterPoint() const;
	
	// Содержит ли прямоугольник точку point
	// Не "захватывает" точки снизу и справа.
	bool Contains(const FPoint& point) const;

	// Содержит ли другой прямоугольник полностью
	bool Contains(const FRect& rect) const;
	
	// Пересекается ли с прямоугольником rect
	bool Intersects(const FRect& rect) const;
	
	// Отмасштабировать прямоугольник
	void Scale(float s);

	// Отмасштабировать прямоугольник c разным масштабом для оХ и оУ
	void Scale(float sx, float sy);
	
	// ВОзвращает отмасштабированный прямоугольник
	FRect Scaled(float s) const;

	// ВОзвращает отмасштабированный прямоугольник c разным масштабом для оХ и оУ
	FRect Scaled(float sx, float sy) const;

	// Расширить прямоугольник с каждой стороны на delta
	void Inflate(float delta);
	
	// Возвращает новый прямоугольик с расширенными на delta сторонами
	FRect Inflated(float delta) const;

	// Установка начала прямоугольника в точку origin
	void MoveTo(const FPoint& origin);
	
	// Возвращает новый прямоугольник с указанными координатами
	FRect MovedTo(const FPoint& origin) const;

	// Смещение начала прямоугольника на (delta.x, delta.y)
	void MoveBy(const FPoint& delta);
	
	// Возвращает новый прямоугольник со смещёнными координатами
	FRect MovedBy(const FPoint& delta) const;

	// Результатом функции является
	// отсечение от текущего прямоугольника всего, что не попадает в second
	// Если прямоугольники имеют нулевую площадь пересечения - результат
	// вырождается в точку или отрезок на стороне second.
	// В случае отсутствия у прямоугольников хотя бы одной общей точки
	// функция некоммутативна
	FRect CutBy(const FRect& second) const;

	// Проверка на равенство с другим экземпляром
	bool operator == (const FRect& rect);

	// Проверка на неравенство
	bool operator != (const FRect& rect);

public:    
	float xStart;

	float xEnd;

	float yStart;

	float yEnd;
};

inline std::ostream& operator<<(std::ostream& os, const FRect& r)
{
	os << r.xStart << " " << r.xEnd << " " << r.yStart << " " << r.yEnd;
	return os;
}

#endif // UTILS_FRECT_H_INCLUDED