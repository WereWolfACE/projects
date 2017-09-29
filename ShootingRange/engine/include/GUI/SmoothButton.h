#ifndef _SMOOTHBUTTON_H_
#define _SMOOTHBUTTON_H_

#include "types.h"
#include "GUI/Widget.h"
#include "Render/Text.h"
#include "Render/Texture.h"
#include "SmoothTextureChanger.h"

namespace GUI
{

class SmoothButton : public Widget
{
	float _enableTimer;

	float _scaleFont;
	IPoint _textPosition;
	Render::TextPtr textPressed;

	RectOrient _orient;

	Render::Texture* _boardNormal;
	Render::Texture* _boardPress;
	Render::Texture* _boardActive;
	Render::Texture* _boardDeactive;

	SmoothTextureChanger *_smooth;

	Render::TextPtr _text;

	bool _isActive, _useSound;

	static float ENABLE_TIMER;

public:
	SmoothButton(const std::string& name, rapidxml::xml_node<>* xmlElement);

	virtual ~SmoothButton();

	virtual void Draw();
	virtual bool MouseDown(const IPoint& mouse_pos);
	virtual void MouseUp(const IPoint& mouse_pos);
	virtual void AcceptMessage(const Message& message);
	virtual void MouseMove(const IPoint& mouse_pos);
	virtual void Update(float dt);
	virtual bool HitTest(const IPoint& point);
    virtual Message QueryState(const Message& message) const;
	bool IsUnderMouse(const IPoint& point);

	static void SetEnableTimer(float value);
};

}

#endif // _SMOOTHBUTTON_H_
