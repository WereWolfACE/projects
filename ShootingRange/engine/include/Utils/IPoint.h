#ifndef _UTILS_IPOINT_
#define _UTILS_IPOINT_

class IRect;

class IPoint
{
public:
	IPoint();
	IPoint(int x, int y);

	/// Инициализация точки левым нижним углом прямоугольника
	explicit IPoint(const IRect& rect);

	int x;
	int y;

	/// Оператор унарный минус
	IPoint operator - () const;
	IPoint& operator += (const IPoint& p);
	IPoint& operator -= (const IPoint& p);
	IPoint operator * (int i) const;
};

inline IPoint operator + (const IPoint& a, const IPoint& b)
{
	return IPoint(a.x + b.x, a.y + b.y);
}

inline IPoint operator - (const IPoint& a, const IPoint& b)
{
	return IPoint(a.x - b.x, a.y - b.y);
}

inline bool operator == (const IPoint& a, const IPoint& b)
{
	return a.x == b.x && a.y == b.y;
}

inline bool operator != (const IPoint&a, const IPoint& b)
{
	return a.x != b.x || a.y != b.y;
}

inline std::ostream& operator<<(std::ostream& os, const IPoint& p)
{
	os << p.x << " " << p.y;
	return os;
}

#endif
