#pragma once

#include <Utils/Color.h>

#include "fquery.h"
#include "../core/FlashStage.h"
#include "../core/FlashMarkSweep.h"

namespace fquery {
namespace ui {

class UIElement: public IGCRef
{
public:
	UIElement()
	{
	}

	virtual ~UIElement()
	{
	}

	void setWrapedObject(IFlashDisplayObject* _displayObject)
	{
		displayObject = _displayObject;
	};

	IFlashDisplayObject* getWrapedObject()
	{
		return displayObject;
	};

protected:
	GC_BLACKEN_DECL()
	{
				//ÌÂ ÌÛÊÌÓ ÔÓÏÂ˜‡Ú¸ displayObject - ˝ÚÓ ÒÎ‡·‡ˇ ÒÒ˚ÎÍ‡
	}

protected:
	IFlashDisplayObject* displayObject = nullptr; //weak ref
};

class Button: public UIElement
{
public:
	Button()
		: root(nullptr)
	{
	}

	virtual ~Button()
	{
		unbind();
	}

	void setWrapedObject(IFlashDisplayObject* _displayObject)
	{
		UIElement::setWrapedObject(_displayObject);
		fQuery(displayObject)
					.bind(TouchBegin, MEMBER_HANDLER(this, Button::onTouchBegin))
					.bind(TouchRollOver, MEMBER_HANDLER(this, Button::onTouchOver))
					.bind(TouchRollOut, MEMBER_HANDLER(this, Button::onTouchOut))
					.bind("disable", MEMBER_HANDLER(this, Button::onDisable))
					.bind("enable", MEMBER_HANDLER(this, Button::onEnable));

		isTouchOver = false;
		isTouching = false;
		isEnabled = true;

		active = fQuery("active", displayObject)[0];
		pressed = fQuery("pressed", displayObject)[0];
		IFlashDisplayObject* back = fQuery("hitTest", displayObject)[0];

		if(back)
		{
#ifdef ENGINE_TARGET_MACOS
			if(active)
			{
				IFlashSprite* l_container = displayObject->Sprite();
				l_container->removeChild(back);

				back = active->Prototype()->Clone()->DisplayObject();
				back->setName("hitTest");
				l_container->addChild(back);
			}
#endif

			//ÒÛÚ¸ ˝ÚÓ„Ó ıËÚÓÔÓÔÒÚ‚‡, ‚ ÚÓÏ ˜ÚÓ ÂÒÎË ·˚ Ï˚ ˝ÎÂÏÂÌÚ˚ ÔÓÒÚ‡‚ËÎË setVisible(false), ÓÌ ÌÂ ÔËÌËÏ‡Î ·˚ Û˜‡ÒÚËÂ ‚ ‡Ò˜ÂÚÂ hittetst ÒÏ ÍÓ‰ ‚ FlashDisplayObjectContainer.hpp
			back->setAlpha(0.f);
		}
#ifdef ENGINE_TARGET_MACOS
		else if(active)
		{
			back = active->Prototype()->Clone()->DisplayObject();
			back->setAlpha(0.f);
			back->setName("hitTest");

			IFlashSprite* l_container = displayObject->Sprite();
			l_container->addChild(back);
		}
#endif

		updateState();
	};

protected:
	const Color GRAY_COLOR = Color(0x88, 0x88, 0x88, 0x88);
	const Color WHITE_COLOR = Color::WHITE;

	void onTouchBegin(TouchEvent& event)
	{
		if (isEnabled)
		{
			isTouching = true;
			root = displayObject->getRoot();
			fQuery(root)
				.bind(TouchEnd, MEMBER_HANDLER(this, Button::onTouchEnd))
				.bind(TouchCancel, MEMBER_HANDLER(this, Button::onTouchCancel));
			updateState();
		}
	}

	void onTouchOver(TouchEvent& event)
	{
		if (isEnabled)
		{
			isTouchOver = true;
			updateState();
		}
	}

	void onTouchOut(TouchEvent& event)
	{
		if (isEnabled)
		{
			isTouchOver = false;
			updateState();
		}
	}

	void onTouchEnd(TouchEvent& event)
	{
		isTouching = false;
		unbind();
		updateState();
		if (isTouchOver) {
			FlashEvent event("click");
			EventManager::Get().dispatchTarget(displayObject, event);
		}
	}

	void onTouchCancel(TouchEvent& event){
		isTouching = false;
		unbind();
		updateState();
	}

	void onDisable(FlashEvent& event)
	{
		isEnabled = false;
		updateState();
	}

	void onEnable(FlashEvent& event)
	{
		isEnabled = true;
		updateState();
	}

	void updateState()
	{
		if (isEnabled)
		{
			if (isTouching && isTouchOver)
			{
				if (active) {
					active->setVisible(false);
				}

				if (pressed)
				{
					pressed->setVisible(true);
					if (isEnabled)
					{
						pressed->setColor(WHITE_COLOR);
					}
					else
					{
						pressed->setColor(GRAY_COLOR);
					}
				}
			}
			else
			{
				if (active)
				{
					active->setVisible(true);
					if (isEnabled)
					{
						active->setColor(WHITE_COLOR);
					}
					else
					{
						active->setColor(GRAY_COLOR);
					}

				}

				if (pressed) {
					pressed->setVisible(false);
				}
			}
		};
	}

	void unbind(){
		if (!root) {
			return;
		}
		if(!isTouching)	// если isTouching true, значит unbind вызван деструктором и мы закрываем игру с зажатой кнопкой. root в этот момент может быть уже уничтоженным и указатель стал невалидным
		{
			fQuery(root)
				.unbind(TouchEnd, MEMBER_HANDLER(this, Button::onTouchEnd))
				.unbind(TouchCancel, MEMBER_HANDLER(this, Button::onTouchCancel));
		}
		root = nullptr;
	}

	bool isEnabled;
	bool isTouchOver;
	bool isTouching;

	IFlashDisplayObject* root = nullptr;
	IFlashDisplayObject* active = nullptr;
	IFlashDisplayObject* pressed = nullptr;
};

class Checkbox: public UIElement
{
public:
	Checkbox()
	{
	}

	virtual ~Checkbox()
	{
	}

	void setWrapedObject(IFlashDisplayObject* _displayObject)
	{
		UIElement::setWrapedObject(_displayObject);
		fQuery(displayObject)
			.bind(TouchBegin, MEMBER_HANDLER(this, Checkbox::onTouchBegin))
			.bind("check", MEMBER_HANDLER(this, Checkbox::onCheck))
			.bind("uncheck", MEMBER_HANDLER(this, Checkbox::onUncheck))
			.bind("queryBoolValue", MEMBER_HANDLER(this, Checkbox::onQueryValue));

		check = fQuery("check", displayObject)[0];
		state = false;

		IFlashDisplayObject* back = fQuery("hitTest", displayObject)[0];

		if(back)
		{
#ifdef ENGINE_TARGET_MACOS
			if(check)
			{
				IFlashSprite* l_container = displayObject->Sprite();
				l_container->removeChild(back);

				back = check->Prototype()->Clone()->DisplayObject();
				back->setName("hitTest");
				l_container->addChild(back);
			}
#endif

			//ÒÛÚ¸ ˝ÚÓ„Ó ıËÚÓÔÓÔÒÚ‚‡, ‚ ÚÓÏ ˜ÚÓ ÂÒÎË ·˚ Ï˚ ˝ÎÂÏÂÌÚ˚ ÔÓÒÚ‡‚ËÎË setVisible(false), ÓÌ ÌÂ ÔËÌËÏ‡Î ·˚ Û˜‡ÒÚËÂ ‚ ‡Ò˜ÂÚÂ hittetst ÒÏ ÍÓ‰ ‚ FlashDisplayObjectContainer.hpp
			back->setAlpha(0.f);
			}
#ifdef ENGINE_TARGET_MACOS
		else if(check)
		{
			back = check->Prototype()->Clone()->DisplayObject();
			back->setAlpha(0.f);
			back->setName("hitTest");

			IFlashSprite* l_container = displayObject->Sprite();
			l_container->addChild(back);
		}
#endif
		updateState();
	};

protected:
			void onTouchBegin(TouchEvent& /*event*/){
				state = !state;
				if ( state ){
					FlashEvent event("toggledOn");
					EventManager::Get().dispatchTarget(displayObject, event);
				}else{
					FlashEvent event("toggledOff");
					EventManager::Get().dispatchTarget(displayObject, event);
				}
				updateState();
			}

			void onQueryValue(QueryEvent<bool>& event){
				event.respond(state);
			}

			void onCheck(FlashEvent& event){
				state = true;
				updateState();
			}

			void onUncheck(FlashEvent& event){
				state = false;
				updateState();
			}

			void updateState(){
				if ( state ){
					if ( check ){
						check->setVisible(true);
					}
				}else{
					if ( check ){
						check->setVisible(false);
					}
				}
			};

protected:
	bool state;
	IFlashDisplayObject* root = nullptr;
	IFlashDisplayObject* check = nullptr;
};

class ToggleButton: public UIElement
{
public:
	ToggleButton()
	{
	}

	virtual ~ToggleButton()
	{
	}

	void setWrapedObject(IFlashDisplayObject* _displayObject)
	{
		UIElement::setWrapedObject(_displayObject);
		fQuery(displayObject)
			.bind(TouchBegin, MEMBER_HANDLER(this, ToggleButton::onTouchBegin))
			.bind("check", MEMBER_HANDLER(this, ToggleButton::onCheck))
			.bind("uncheck", MEMBER_HANDLER(this, ToggleButton::onUncheck))
			.bind("queryBoolValue", MEMBER_HANDLER(this, ToggleButton::onQueryValue));

		active = fQuery("active", displayObject)[0];
		pressed = fQuery("pressed", displayObject)[0];
		state = false;

#ifdef ENGINE_TARGET_MACOS
		IFlashDisplayObject* back = fQuery("hitTest", displayObject)[0];

		if(back)
		{
			if(active)
			{
				IFlashSprite* l_container = displayObject->Sprite();
				l_container->removeChild(back);

				back = active->Prototype()->Clone()->DisplayObject();
				back->setName("hitTest");
				l_container->addChild(back);
			}


			//ÒÛÚ¸ ˝ÚÓ„Ó ıËÚÓÔÓÔÒÚ‚‡, ‚ ÚÓÏ ˜ÚÓ ÂÒÎË ·˚ Ï˚ ˝ÎÂÏÂÌÚ˚ ÔÓÒÚ‡‚ËÎË setVisible(false), ÓÌ ÌÂ ÔËÌËÏ‡Î ·˚ Û˜‡ÒÚËÂ ‚ ‡Ò˜ÂÚÂ hittetst ÒÏ ÍÓ‰ ‚ FlashDisplayObjectContainer.hpp
			back->setAlpha(0.f);
		}
#endif
		updateState();
	};

protected:
			void onTouchBegin(TouchEvent& event){
				state = !state;
				if ( state ){
					FlashEvent event("toggledOn");
					EventManager::Get().dispatchTarget(displayObject, event);
				}else{
					FlashEvent event("toggledOff");
					EventManager::Get().dispatchTarget(displayObject, event);
				}
				updateState();
			}

			void onQueryValue(QueryEvent<bool>& event){
				event.respond(state);
			}

			void onCheck(FlashEvent& event){
				state = true;
				updateState();
			}

			void onUncheck(FlashEvent& event){
				state = false;
				updateState();
			}

			void updateState(){
				if ( state ){
					if ( active ){
						active->setVisible(false);
					}
					if ( pressed ){
						pressed->setVisible(true);
					}
				}else{
					if ( active ){
						active->setVisible(true);
					}
					if ( pressed ){
						pressed->setVisible(false);
					}
				}
			}

protected:
	bool state = false;
	// IFlashDisplayObject* root; // not in use
	IFlashDisplayObject* active = nullptr;
	IFlashDisplayObject* pressed = nullptr;
};

///  Î‡ÒÒ, Â‡ÎËÁÛ˛˘ËÈ ÔÓ‚Â‰ÂÌËÂ ÔË‚ˇÁÍË Ó·˙ÂÍÚ‡ Í ÒˆÂÌÂ
class Anchor : public UIElement
{
public:
	enum Type
	{
		None,
		Left = 1 << 0,
		Top = 1 << 1,
		Right = 1 << 2,
		Bottom = 1 << 3
	};

	Anchor()
	{
	}

	virtual ~Anchor()
	{
	}

	void setWrapedObject(IFlashDisplayObject* _displayObject)
	{
		UIElement::setWrapedObject(_displayObject);
		FlashStage* stage = dynamic_cast<FlashStage*>(displayObject->getRoot());

		if (!stage)
		{
			Halt("display object must be put into the stage");
			return;
		}

		stage->getOrigin(initStageX, initStageY);
		stage->getStageSize(initStageWidth, initStageHeight);

		displayObject->getBounds(left, top, right, bottom, NULL);

		fQuery(displayObject).bind(Resize, MEMBER_HANDLER(this, Anchor::onResize));
	};

	void setAnchor(unsigned int _anchor)
	{
		anchor = _anchor;
	};

	unsigned int getAnchor()
	{
		return anchor;
	};

protected:
	void onResize(ResizeEvent& event) {
		FlashStage* stage = dynamic_cast<FlashStage*>(displayObject->getRoot());
		if (!stage) {
			Halt("display object must be put into the stage");
			return;
		}

		// œË‚ˇÁÍ‡ Í Í‡ˇÏ ËÏÂÂÚ ÒÏ˚ÒÎ ÚÓÎ¸ÍÓ ‚ ÂÊËÏÂ ·ÂÁ Ï‡Ò¯Ú‡·ËÓ‚‡ÌËˇ
		if (stage->getScaleMode() != FlashStage::ScaleMode::NoScale) {
			return;
		}

		float origX, origY;
		stage->getOrigin(origX, origY);

		float stageWidth, stageHeight;
		stage->getStageSize(stageWidth, stageHeight);

		float dx0 = initStageX - origX;
		float dx1 = initStageX - origX + initStageWidth - stageWidth;
		float dy0 = initStageY - origY;
		float dy1 = initStageY - origY + initStageHeight - stageHeight;

		float newLeft = left, newTop = top, newRight = right, newBottom = bottom;

		if (anchor & Left || anchor & Right) {
			if (anchor & Left && anchor & Right) {
				newLeft -= dx0;
				newRight -= dx1;
			} else if (anchor & Left) {
				newLeft -= dx0;
				newRight -= dx0;
			} else if (anchor & Right) {
				newLeft -= dx1;
				newRight -= dx1;
			}
		}

		if (anchor & Top || anchor & Bottom) {
			if (anchor & Top && anchor & Bottom) {
				newTop -= dy0;
				newBottom -= dy1;
			} else if (anchor & Top) {
				newTop -= dy0;
				newBottom -= dy0;
			} else if (anchor & Bottom) {
				newTop -= dy1;
				newBottom -= dy1;
			}
		}

		displayObject->setPosition(left, top);
		displayObject->setScale((newRight - newLeft) / (right - left), (newBottom - newTop) / (bottom - top));
	}

protected:
	unsigned int anchor;
	float initStageX = 0, initStageY = 0, initStageWidth = 0, initStageHeight = 0;
	float left = 0, top = 0, right = 0, bottom = 0;
};

void makeButton(IFlashDisplayObject* displayObject);
void makeCheckbox(IFlashDisplayObject* displayObject);
void makeToggleButton(IFlashDisplayObject* displayObject);
void makeAnchor(IFlashDisplayObject* displayObject, unsigned int anchor);

FQuerySelector& button(FQuerySelector& selector);
FQuerySelector& checkbox(FQuerySelector& selector);
FQuerySelector& toggleButton(FQuerySelector& selector);
FQuerySelector& anchor(FQuerySelector& selector, unsigned int anchor);

} // namespace ui
} // namespace fquery

#include "fquery_ui_press.h"
#include "fquery_ui_radio.h"
#include "fquery_ui_scroller.h"
