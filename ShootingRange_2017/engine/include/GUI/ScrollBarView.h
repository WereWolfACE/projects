#pragma once

#include "Utils/FPoint.h"
#include "Utils/IRect.h"
#include "Render/Texture.h"

namespace Render {
	class Texture;
}

namespace GUI {

enum class ScrollBarOrientation {
	Vertical,
	Horizontal
};

class ScrollBarView;

//
// ќбщий интерфейс обращени€ к полосе прокрутки
//
class IScrollBar {
public:
	virtual ~IScrollBar() { }
	virtual float GetFirstValue() const = 0;
	virtual float GetLastValue() const = 0;
	virtual float GetCurValue() const = 0;
};

//
// ќбщий интерфейс объектов поведени€ полосы прокрутки
//
class IScrollBarBehavior : public RefCounter {
public:
	virtual ~IScrollBarBehavior() { }
	virtual void Init(ScrollBarView* scrollBarView) = 0;
	virtual void Update(float dt) = 0;
	virtual void BeforeDraw() = 0;
	virtual void AfterDraw() = 0;
};

typedef boost::intrusive_ptr<IScrollBarBehavior> IScrollBarBehaviorPtr;

//
// ѕредставление полосы прокрутки
//
class ScrollBarView : public RefCounter {
public:
	ScrollBarView();

	void Init(Render::Texture* texTrace, Render::Texture* texSlider, ScrollBarOrientation orientation = ScrollBarOrientation::Vertical, bool withTraceIndents = false);

	void SetScrollBar(const IScrollBar* scrollBar) { _scrollBar = scrollBar; }
	void SetPos(const FPoint& pos) { _pos = pos; }

	void SetBehavior(IScrollBarBehaviorPtr behavior);
	const IScrollBar* GetScrollBar() const { return _scrollBar; }

	void IncIndents(float dIndent);
    void IncFirstIndent(float dIndent);
    void IncLastIndent(float dIndent);
    void SetOrtoIndent(float ortoIndent) { _ortoIndent = ortoIndent; }
    
    float GetFirstIndent() const { return _firstIndent; }
    float GetLastIndent() const { return _lastIndent; }
    float GetIndents() const { return _firstIndent + _lastIndent; }
    float GetOrtoIndent() const { return _ortoIndent; }

	void Draw();
	void Update(float dt);

private:
	float GetCurSliderShift() const;			// “екущее смещение слайдера относительно значени€ при GetFirstValue()
	float GetCurSliderShiftFromEdge() const;	// “екущее смещение слайдера относительно кра€ текстуры полосы
	float GetCurSliderTextureShift() const;		// “екущее смещение текстуры слайдера, относительно кра€ текстуры полосы
	float GetSliderLength() const;
	float GetSliderSize() const;
	IRect GetSliderRect() const { return _texSlider->getBitmapRect(); }

private:
	const IScrollBar* _scrollBar;
	IScrollBarBehaviorPtr _behavior;

	FPoint _pos;						// ѕозици€ полосы прокрутки
	Render::Texture* _texTrace;			// “екстура фона
	Render::Texture* _texSlider;		// “екстура бегунка
	float _maxSliderShift;				// ћаксимально возможное смещение бегунка
	float _firstIndent;					// ќтступ между началом текстуры полосы и началом пути, по которому движитс€ бегунок
	float _lastIndent;					// ќтступ между концом текстуры полосы и концом пути, по которому движетс€ бегунок
	float _ortoIndent;					// ќтступ между линией, по которой движетс€ бегунок, и параллельной линией-серединой текстуры полосы прокрутки
	ScrollBarOrientation _orientation;
};

typedef boost::intrusive_ptr<ScrollBarView> ScrollBarViewPtr;

} // namespace GUI
