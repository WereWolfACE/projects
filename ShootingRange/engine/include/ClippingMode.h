#ifndef _CLIPPINGMODE_H_
#define _CLIPPINGMODE_H_

//
// Класс обрезания
//
class ClippingMode {
public:
	static const ClippingMode NONE;
	static const ClippingMode LEFT;
	static const ClippingMode TOP;
	static const ClippingMode RIGHT;
	static const ClippingMode BOTTOM;
	static const ClippingMode LEFT_RIGHT;
	static const ClippingMode TOP_BOTTOM;
	static const ClippingMode ALL;

	ClippingMode(bool isLeft, bool isTop, bool isRight, bool isBottom);

	bool IsLeft() const { return _isLeft; }
	bool IsTop() const { return _isTop; }
	bool IsRight() const { return _isRight; }
	bool IsBottom() const { return _isBottom; }

	friend bool operator==(const ClippingMode& first, const ClippingMode& second);
	friend bool operator!=(const ClippingMode& first, const ClippingMode& second);

	static ClippingMode Add(const ClippingMode& first, const ClippingMode& second);
	static ClippingMode Sub(const ClippingMode& first, const ClippingMode& second);

private:
	bool _isLeft;
	bool _isTop;
	bool _isRight;
	bool _isBottom;
};

inline bool operator==(const ClippingMode& first, const ClippingMode& second) {
	return first._isLeft == second._isLeft && first._isTop == second._isTop && first._isRight == second._isRight && first._isBottom == second._isBottom;
}

inline bool operator!=(const ClippingMode& first, const ClippingMode& second) {
	return !(first == second);
}

inline ClippingMode operator+(const ClippingMode& first, const ClippingMode& second) {
	return ClippingMode::Add(first, second);
}

inline ClippingMode operator-(const ClippingMode& first, const ClippingMode& second) {
	return ClippingMode::Sub(first, second);
}

#endif
