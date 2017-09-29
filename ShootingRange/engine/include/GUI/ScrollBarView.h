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
// ����� ��������� ��������� � ������ ���������
//
class IScrollBar {
public:
	virtual ~IScrollBar() { }
	virtual float GetFirstValue() const = 0;
	virtual float GetLastValue() const = 0;
	virtual float GetCurValue() const = 0;
};

//
// ����� ��������� �������� ��������� ������ ���������
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
// ������������� ������ ���������
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
	float GetCurSliderShift() const;			// ������� �������� �������� ������������ �������� ��� GetFirstValue()
	float GetCurSliderShiftFromEdge() const;	// ������� �������� �������� ������������ ���� �������� ������
	float GetCurSliderTextureShift() const;		// ������� �������� �������� ��������, ������������ ���� �������� ������
	float GetSliderLength() const;
	float GetSliderSize() const;
	IRect GetSliderRect() const { return _texSlider->getBitmapRect(); }

private:
	const IScrollBar* _scrollBar;
	IScrollBarBehaviorPtr _behavior;

	FPoint _pos;						// ������� ������ ���������
	Render::Texture* _texTrace;			// �������� ����
	Render::Texture* _texSlider;		// �������� �������
	float _maxSliderShift;				// ����������� ��������� �������� �������
	float _firstIndent;					// ������ ����� ������� �������� ������ � ������� ����, �� �������� �������� �������
	float _lastIndent;					// ������ ����� ������ �������� ������ � ������ ����, �� �������� �������� �������
	float _ortoIndent;					// ������ ����� ������, �� ������� �������� �������, � ������������ ������-��������� �������� ������ ���������
	ScrollBarOrientation _orientation;
};

typedef boost::intrusive_ptr<ScrollBarView> ScrollBarViewPtr;

} // namespace GUI
