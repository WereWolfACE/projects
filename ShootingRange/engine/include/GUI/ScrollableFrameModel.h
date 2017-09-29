#pragma once

#include "GUI/ScrollBarView.h"
#include "GUI/FunctorScrollBarAdapter.h"
#include "Utils/IRect.h"

namespace GUI {

//
// Информация о текущем состоянии прокрутки
//
class ScrollableFrameModel {
public:
	ScrollableFrameModel() = default;

	void Init(const IRect& rectFrame);

	void SetFrameRect(const IRect& rectFrame) { _rectFrame = rectFrame; }
	const IRect& GetFrameRect() const { return _rectFrame; }

	void SetContentShift(const FPoint& shiftContent);
	void IncContentShift(const FPoint& dShift);
	const FPoint& GetContentShift() const { return _shiftContent; }
	FPoint GetContentShiftLimit() const;

	void SetContentWidth(float widthContent);
	void SetContentHeight(float heightContent);
	float GetContentWidth() const { return _sizeContent.x; }
	float GetContentHeight() const { return _sizeContent.y; }

	// Получить смещение, ближайшее указанному, при котором прокрутка не выйдет за границы содержимого
	FPoint GetNearestValidShift(const FPoint& shift) const;

	void MoveTo(const IPoint& point) { _rectFrame.MoveTo(point); }
	bool HitTest(const IPoint& point) const { return _rectFrame.Contains(point); }

	bool NeedHorizontalScroll() const { return (int)GetContentWidth() > _rectFrame.Width(); }
	bool NeedVerticalScroll() const { return (int)GetContentHeight() > _rectFrame.Height(); }
	bool NeedScrollLeft() const { return (int)GetContentShift().x < 0; }
	bool NeedScrollRight() const { return (int)GetContentShift().x > (int)GetContentShiftLimit().x; }
	bool NeedScrollDown() const { return (int)GetContentShift().y < 0; }
	bool NeedScrollUp() const { return (int)GetContentShift().y > (int)GetContentShiftLimit().y; }

	const IScrollBar* GetHorizontalScrollBar() const { return &_horizontalBar; }
	const IScrollBar* GetVerticalScrollBar() const { return &_verticalBar; }

private:
	IRect _rectFrame;			// Прямоугольник фрейма относительно "внешней" системы координат
	FPoint _shiftContent;		// Смещение левого нижнего угла содержимого при прокрутке относительно левого нижнего угла фрейма
	FPoint _sizeContent;		// Размеры прокручиваемого содержимого

	FunctorScrollBarAdapter _horizontalBar;
	FunctorScrollBarAdapter _verticalBar;
};

} // namespace GUI
