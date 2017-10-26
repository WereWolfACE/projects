#ifndef UTILS_IRECT_H_INCLUDED
#define UTILS_IRECT_H_INCLUDED

#include "Utils/IPoint.h"
#include "Render/RenderTypes.h"

class FRect;

//
// Целочисленый прямоугольник
//
class IRect
{
public:
	// Конструктор по умолчанию - создает "нулевой" прямоугольник
	IRect();

	// Покомпонентный конструктор
	IRect(int x, int y, int width, int height);
	
	// Конструктор из точки, ширины и высоты
	IRect(const IPoint& p, int width, int height);

	// Конструктор из FRect
	explicit IRect(const FRect& r);
	
	// Возвращает ширину прямоугольника
	int Width() const;
	
	// Возвращает высоту прямоугольника
	int Height() const;
	
	// Возвращает точку левого верхнего угла
	IPoint LeftTop() const;
	
	// Возвращает точку левого нижнего угла
	IPoint LeftBottom() const;
	
	// Возвращает точку правого верхнего угла
	IPoint RightTop() const;
	
	// Возвращает точку правого нижнего угла
	IPoint RightBottom() const;

	// Содержит ли прямоугольник точку point
	// Не "захватывает" точки снизу и справа.
	bool Contains(const IPoint& point) const;

	// Содержит ли другой прямоугольник полностью
	bool Contains(const IRect& rect) const;
	
	// Пересекается ли с прямоугольником rect
	bool Intersects(const IRect& rect) const;

	// Расширить прямоугольник с каждой стороны на delta
	void Inflate(int delta);
	
	// Возвращает новый прямоугольик с расширенными на delta сторонами
	IRect Inflated(int delta) const;

	// Установка начала прямоугольника в точку origin
	void MoveTo(const IPoint& origin);
	
	// Возвращает новый прямоугольник с указанными координатами
	IRect MovedTo(const IPoint& origin) const;

	// Смещение начала прямоугольника на (delta.x, delta.y)
	void MoveBy(const IPoint& delta);
	
	// Возвращает новый прямоугольник со смещёнными координатами
	IRect MovedBy(const IPoint& delta) const;

	// Результатом функции является
	// отсечение от текущего прямоугольника всего, что не попадает в second
	// Если прямоугольники имеют нулевую площадь пересечения - результат
	// вырождается в точку или отрезок на стороне second.
	// В случае отсутствия у прямоугольников хотя бы одной общей точки
	// функция некоммутативна
	IRect CutBy(const IRect& second) const;

	// Проверка на равенство с другим экземпляром
	bool operator == (const IRect& rect) const;

	bool operator != (const IRect& rect) const;

	void Rotate(RectOrient orient);

public:
	int x;

	int y;

	int width;

	int height;
};

inline std::ostream& operator<<(std::ostream& os, const IRect& r)
{
	os << r.x << " " << r.y << " " << r.width << " " << r.height;
	return os;
}

#endif // UTILS_IRECT_H_INCLUDED
