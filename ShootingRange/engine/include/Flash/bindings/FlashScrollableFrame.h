#pragma once

#include "GUI/ScrollableFrame.h"
#include "../events/TouchEvents.h"
#include "../events/EventManager.h"
#include "../core/FlashDisplayObject.hpp"
#include "../core/IFlashSprite.h"
#include "../core/FlashUnknown.hpp"
#include "IFlashScrollableFrame.h"

class FlashScrollableFrame :
	public FlashDisplayObject<IFlashScrollableFrame>
{
	BEGIN_INTERFACE_MAP()
		INTERFACE_ENTRY(IFlashScrollableFrame)
		INTERFACE_ENTRY(IFlashSprite)
		INTERFACE_ENTRY(IFlashDisplayObject)
	END_INTERFACE_MAP()

public:
	FlashScrollableFrame();
	virtual ~FlashScrollableFrame();

	//----------------------------------------------------------------------------
	//
	//                          IScrollingContainer
	//
	//----------------------------------------------------------------------------
	virtual void init(int left, int top, int right, int bottom) override;

	virtual void enableScroll(bool horizontal, bool vertical) override;

	virtual void setFrameRect(int left, int top, int right, int bottom) override;
	virtual void getFrameRect(int& left, int& top, int& right, int& bottom) override;

	virtual void enableMouseWheel(bool isEnabled) override;

	virtual void setMouseWheelVertical(bool isVertical) override;
	virtual bool isMouseWheelVertical() const override;

	virtual void getScroll(float& x, float& y) override;
	virtual void getMinScroll(float& x, float& y) override;
	virtual void getMaxScroll(float& x, float& y) override;

	virtual void resetScroll(float x, float y) override;
	virtual void resetScroll(float x, float y, float minusLength) override;

	virtual bool inManualScroll() const override;

	virtual void setAutoScrollScaleFactor(float scaleFactor) override;
	virtual void setAutoScrollTimeConstant(float timeConstant) override;

	virtual void activateAutoScroll() override;
	virtual void deactivateAutoScroll() override;

	virtual void setTarget(float targetX, float targetY) override;
	virtual void setTarget(float targetX, float targetY, float curTimeConstant) override;
	virtual void getTarget(float& targetX, float& targetY) const override;

	virtual bool needHorizontalScroll() const override;
	virtual bool needVerticalScroll() const override;
	virtual bool needScrollLeft() const override;
	virtual bool needScrollRight() const override;
	virtual bool needScrollDown() const override;
	virtual bool needScrollUp() const override;

	virtual void setMagnetStepWidth(float width) override;
	virtual void setMagnetStepHeight(float height) override;
	virtual float getMagnetStepWidth() const override;
	virtual float getMagnetStepHeight() const override;
	virtual void setColumnsCount(int columnsCount) override;
	virtual void setRowsCount(int rowsCount) override;
	virtual int getColumnsCount() override;
	virtual int getRowsCount() override;
	virtual int getCurrentColumn() override;
	virtual int getCurrentRow() override;

	virtual void setTargetColumn(int column) override;
	virtual void setTargetColumn(int column, float curTimeConstant) override;
	virtual void setTargetRow(int row) override;
	virtual void setTargetRow(int row, float curTimeConstant) override;

	virtual int getTargetColumn() const override;
	virtual int getTargetRow() const override;

	virtual void resetMagnetPoint(int column, int row, float minusLength) override;

	virtual void magnetTurnOn() override;
	virtual void magnetTurnOff() override;

	virtual bool isMoving() const override;
	virtual bool isAutoscrollMoving() const override;

	virtual void contentChanged() override;

	virtual void setContentRect(float left, float top, float right, float bottom) override;
	virtual void getContentRect(float& left, float& top, float& right, float& bottom) override;

	virtual void setContentInsets(float left, float top, float right, float bottom) override;
	virtual void getContentInsets(float& left, float& top, float& right, float& bottom) override;

	virtual void enableAutoContentRect(bool isEnabled) override;
	virtual bool isAutoContentRect() override;

	virtual void setContentAlign(float hAlign, float vAlign) override;

	//---------------------------------------------------------------------------
	//
	//                    IFlashDisplayObject
	//
	//---------------------------------------------------------------------------
	virtual void unsafeSetParent(IFlashSprite* _parent) override;
	virtual void render(FlashRender& render) override;
	virtual bool hitTest(float x, float y, IHitTestDelegate* delegate) override;
	virtual bool getBounds(float& left, float& top, float& right, float& bottom, IFlashDisplayObject* targetCoordinateSystem) override;

	virtual void advance(FlashUpdateListener* updateListener, float dt) override;
	virtual void update(float dt) override;
	virtual bool hasUpdate() const override { return true; }

	virtual void setPosition(float x, float y) override;
	virtual void getPosition(float& x, float& y) override;

	virtual void setMatrix(const float matrix[6]) override;

	virtual void mouseWheel(int delta) override;

	//---------------------------------------------------------------------------
	//
	//                       IFlashSprite
	//
	//---------------------------------------------------------------------------
	virtual void changeUpdateListeners(int _value) override;
	virtual IFlashDisplayObject* getChildByName(const std::string& string) override;
	virtual IFlashDisplayObject* addChild(IFlashDisplayObject* displayObject) override;
	virtual IFlashDisplayObject* removeChild(IFlashDisplayObject* displayObject) override;
	virtual IFlashDisplayObject* addChildAt(IFlashDisplayObject* displayObject, int index) override;
	virtual IFlashDisplayObject* removeChildAt(int index) override;
	virtual IFlashDisplayObject* getChildAt(int index) override;
	virtual int getChildrenCount() const override;
	virtual int getChildIndex(IFlashDisplayObject* displayObject) const override;
	virtual void swapChildren(int _index1, int _index2) override;
	virtual void visitDFS(IFlashVisitor* visitor, bool preorder) override;
	virtual void visitBFS(IFlashVisitor* visitor, bool preorder) override;

protected:
	GC_BLACKEN_DECL()
	{
		GC_BLACKEN_CALL(FlashDisplayObject);

		for (auto it : _children)
		{
			GC_SHOULD_MARK(it->object);
		};
	};

private:
	enum class SpatialIndexType {
		QTree,
		Horizontal,
		Vertical,
		None
	};

	struct ChildContainer
	{
		float _left = 0.0f;
		float _top = 0.0f;
		float _right = 0.0f;
		float _bottom = 0.0f;
		IFlashDisplayObject* object = nullptr;
	};

	class SpatialIndex
	{
	public:
		explicit SpatialIndex(FlashScrollableFrame* parent);
		void setIndexType(SpatialIndexType indexType);
		void update(float left, float top, float right, float bottom, std::vector<ChildContainer*>& content);
		void query(float left, float top, float right, float bottom, std::vector<ChildContainer*>& output) const;

		FlashScrollableFrame* _parent = nullptr;
		SpatialIndexType _indexType = SpatialIndexType::None;
	};

private:
	void setIndexType(SpatialIndexType indexType);

	void validateVisibleChildren();
	void validateIndex();
	void updateContentSizeFromContent();
	void updateScrollParams();
	FPoint getNearestValidScroll(const FPoint& scroll) const;

	void onTouchBegin(TouchEvent& event);
	void onTouchMove(TouchEvent& event);
	void onTouchEnd(TouchEvent& event);
	void onTouchCancel(TouchEvent& event);

	void removeListeners();

	void fireScrollEvent();

	//Преобразования из системы координат ScrollableFrame в систему координат Flash объекта и обратно
	FPoint scrollableFrameToFlash(const FPoint& p) const;
	FPoint flashToScrollableFrame(const FPoint& p) const;

private:
	friend class SpatialIndex;

private:
	int _numUpdateListeners = 0;
	SpatialIndex _spatialIndex;
	std::vector<ChildContainer*> _children;
	std::vector<ChildContainer*> _visibleChildren;

	GUI::ScrollableFrame _scrollableFrame;

	FRect _contentRect;
	FRect _contentInsets;

	bool _autoSize = true;
	bool _validContentSize = false;
	float _hAlign = 0.0f;
	float _vAlign = 0.0f;

	bool _visibleChildrenValid = false;
	bool _indexValid = false;

	bool _isMouseWheelEnable = true;
	bool _horizontal = false;
	bool _vertical = false;

	FPoint _scroll;
	FPoint _minScroll;
	FPoint _maxScroll;

	bool _selfCancelDispatch = false;
	bool _scrollStarted = false;

	//Последние координаты точа, для onTouchCancel
	FPoint _lastPos;

	IFlashDisplayObject* _root = nullptr;
	HandlerID _handlers[3];

	// С помощью этой переменных избавимся от проблемы застревания скролла при нажатии второй кнопкой мышки, пока не отпущена первая
	bool _isTouchStarted = false;
};
