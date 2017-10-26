#pragma once

#include "Scroller.h"
#include "../events/TouchEvents.h"
#include "../events/EventManager.h"
#include "../core/FlashDisplayObject.hpp"
#include "../core/IFlashSprite.h"
#include "../core/FlashUnknown.hpp"
#include "IScrollingContainer.h"

class ScrollingContainer:
	public FlashDisplayObject<IScrollingContainer>
{
BEGIN_INTERFACE_MAP()
	INTERFACE_ENTRY(IScrollingContainer)
	INTERFACE_ENTRY(IFlashSprite)
	INTERFACE_ENTRY(IFlashDisplayObject)
END_INTERFACE_MAP()

public:
	ScrollingContainer();
	virtual ~ScrollingContainer();

	enum class SpatialIndexType {
		QTree,
		Horizontal,
		Vertical,
		None
	};

	void resetScrollbar(IFlashDisplayObject* scrollbar = 0);

	/**
	* ”ÒÚ‡Ì‡‚ÎË‚‡ÂÚ ÚËÔ ÔÓÒÚ‡ÌÒÚ‚ÂÌÌÓ„Ó ËÌ‰ÂÍÒ‡ ‰Îˇ ÍÓÌÚÂÈÌÂ‡.
	* ≈ÒÎË ÍÓÌÚÂÈÌÂ ·Û‰ÂÚ „ÓËÁÓÌÚ‡Î¸Ì˚Ï ËÎË ‚ÂÚËÍ‡Î¸Ì˚Ï ÒÔËÒÍÓÏ, Ì‡‰Ó ‚˚·Ë‡Ú¸ ÚËÔ ËÌ‰ÂÍÒ‡
	* IndexHorizontal Ë IndexVertical ÒÓÓÚ‚ÂÚÒÚ‚ÂÌÌÓ
	* ≈ÒÎË ÍÓÌÚÂÈÌÂ ·Û‰ÂÚ ‰‚ÛıÏÂÌ˚Ï, Ì‡‰Ó ‚˚·Ë‡Ú¸ ÚËÔ IndexQTree
	* ≈ÒÎË ˜ËÒÎÓ Ó·˙ÂÍÚÓ‚ ‚ ÍÓÌÚÂÈÌÂÂ ·Û‰ÂÚ ÌÂ·ÓÎ¸¯ËÏ (<100), ÎË·Ó ·ÓÎ¸¯‡ˇ Ëı ˜‡ÒÚ¸ (>25%)
	* ‚ÒÂ„‰‡ ·Û‰ÂÚ ‚Ë‰Ì‡, ÏÓÊÌÓ ‚˚·Ë‡Ú¸ ÚËÔ IndexNone
	*/
	void setIndexType(SpatialIndexType indexType);

	void setScroll(bool horizontal, bool vertical);

	void StartConstantVerticalMotion();
	void StartConstantHorizontalMotion();

	//----------------------------------------------------------------------------
	//
	//                          IScrollingContainer
	//
	//----------------------------------------------------------------------------
	void contentChanged() override;
	void setContentRect(float left, float top, float right, float bottom) override;
	void getContentRect(float& left, float& top, float& right, float& bottom) override;
	void setOverscroll(float left, float top, float right, float bottom) override;
	void getOverscroll(float& left, float& top, float& right, float& bottom) override;
	void setAutoContentRect(bool autoSize) override;
	bool getAutoContentRect() const override;
	void setHorizontalKineticScroll(bool flag) override;
	KineticScroller* getHorizontalScroller() override;
	void setVerticalKineticScroll(bool flag) override;
	KineticScroller* getVerticalScroller() override;
	void setClipRect(float left, float top, float right, float bottom) override;
	void getClipRect(float& left, float& top, float& right, float& bottom) override;
	void setHorizontalScroll(float scroll) override;
	float getHorizontalScroll() const override;
	void setVerticalScroll(float scroll) override;
	float getVerticalScroll() const override;
	float getMaxVelocity() const override;
	void setMaxVelocity(float velocity) override;
	void setContentAlign(float hAlign, float vAlign) override;
	void resetScrollPosition() override;
	void setScrollerBounceNP(int value) override;
	bool isHorizontalKineticScroll() const override;
	bool isVerticalKineticScroll() const override;

	void AddMagnitPointVertical(float value) override;
	void AddMagnitPointHorizontal(float value) override;
	void SetPositionVertical(float value, float speed) override;
	void SetPositionHorizontal(float value, float speed) override;
	void StartConstantVerticalMotion(float MaxSpeed, float Acceleration) override;
	void StopConstantVerticalMotion() override;
	void StartConstantHorizontalMotion(float MaxSpeed, float Acceleration) override;
	void StopConstantHorizontalMotion() override;
	bool IsScrollingHorizontally() const override;
	bool IsScrollingVertically() const override;

	void StopKineticScroll() override;

	void setMouseWheelEnable(bool flag) override;
	void setMouseWheelSpeed(int value) override;
	int getMouseWheelSpeed() const override;
	void setScrollbarOffset(float x, float y) override;
	void getScrollbarOffset(float &x, float &y) const override;

	//---------------------------------------------------------------------------
	//
	//                    IFlashDisplayObject
	//
	//---------------------------------------------------------------------------
	void unsafeSetParent(IFlashSprite* _parent) override;
	void render(FlashRender& render) override;
	bool hitTest(float x, float y, IHitTestDelegate* delegate) override;
	bool getBounds(float& left, float& top, float& right, float& bottom, IFlashDisplayObject* targetCoordinateSystem) override;

	void advance(FlashUpdateListener* updateListener, float dt) override;
	void update(float dt) override;
	bool hasUpdate() const override { return true; }

	void setPosition(float x, float y) override;
	void getPosition(float& x, float& y) override;

	void setMatrix(const float matrix[6]) override;

	void mouseWheel(int delta) override;

	//---------------------------------------------------------------------------
	//
	//                       IFlashSprite
	//
	//---------------------------------------------------------------------------
	void changeUpdateListeners(int _value) override;
	IFlashDisplayObject* getChildByName(const std::string& string) override;
	IFlashDisplayObject* addChild(IFlashDisplayObject* displayObject) override;
	IFlashDisplayObject* removeChild(IFlashDisplayObject* displayObject) override;
	IFlashDisplayObject* addChildAt(IFlashDisplayObject* displayObject, int index) override;
	IFlashDisplayObject* removeChildAt(int index) override;
	IFlashDisplayObject* getChildAt(int index) override;
	int getChildrenCount() const override;
	int getChildIndex(IFlashDisplayObject* displayObject) const override;
	void swapChildren(int _index1, int _index2) override;
	void visitDFS(IFlashVisitor* visitor, bool preorder) override;
	void visitBFS(IFlashVisitor* visitor, bool preorder) override;

protected:
	GC_BLACKEN_DECL()
	{
		GC_BLACKEN_CALL(FlashDisplayObject);

		for ( auto it : children )
		{
			GC_SHOULD_MARK(it->object);
		};
	};

private:
	struct ChildContainer
	{
		float left = 0, top = 0, right = 0, bottom = 0;
		IFlashDisplayObject* object = nullptr;
	};

	class SpatialIndex
	{
	public:
		explicit SpatialIndex(ScrollingContainer* parent);
		void setIndexType(SpatialIndexType indexType);
		void update(float left, float top, float right, float bottom, std::vector<ChildContainer*>& content);
		void query(float left, float top, float right, float bottom, std::vector<ChildContainer*>& output) const;

		ScrollingContainer* parent;
		SpatialIndexType indexType;
	};

private:
	void validateVisibleChildren();
	void validateIndex();
	void updateContentSizeFromContent();
	void updateScrollParams();

	void onTouchBegin(TouchEvent& event);
	void onTouchMove(TouchEvent& event);
	void onTouchEnd(TouchEvent& event);
	void onTouchCancel(TouchEvent& event);
	void onRollOver(TouchEvent& event);
	void onRollOut(TouchEvent& event);

	void startScroll();
	void stopScroll();
	void removeListeners();

	void fireScrollEvent();

private:
	friend class SpatialIndex;

private:
	static const int mouseWheelDefaultSpeed = 20;

	int numUpdateListeners = 0;
	SpatialIndex spatialIndex;
	std::vector<ChildContainer*> children;
	std::vector<ChildContainer*> visibleChildren;

	IFlashDisplayObject* scrollbar_ = nullptr;
	float scrollbarOffsetX = 0, scrollbarOffsetY = 0;
	int mouseWheelSpeed = 0;

	float left = 0, top = 0, right = 0, bottom = 0;

	float contentLeft = 0, contentTop = 0, contentRight = 0, contentBottom = 0;
	float overscrollLeft = 0, overscrollTop = 0, overscrollRight = 0, overscrollBottom = 0;
	bool autoSize, validContentSize;
	float hAlign = 0, vAlign = 0;

	bool horizontalKineticScroll, verticalKineticScroll;
	KineticScroller horizontalScroller, verticalScroller;

	bool visibleChildrenValid;
	bool indexValid;

	bool isMouseWheelEnable;
	bool horizontal;
	bool vertical;

	float xScroll = 0, yScroll = 0;

	bool selfCancelDispatch;
	bool scrollStarted;

	float grabX = 0, grabY = 0, lastX = 0, lastY = 0;
	float scrollGrabX = 0, scrollGrabY = 0;
	IFlashDisplayObject* root = nullptr;
	HandlerID handlers[3];

	// С помощью этой переменных избавимся от проблемы застревания скролла при нажатии второй кнопкой мышки, пока не отпущена первая
	bool isTouchStarted;
};
