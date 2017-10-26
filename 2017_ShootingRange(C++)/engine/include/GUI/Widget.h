#ifndef __WIDGET_H__
#define __WIDGET_H__

#pragma once

#include "Core/MessageManager.h"
#include "Render/Texture.h"
#include "RefCounter.h"

#include <boost/intrusive_ptr.hpp>

class Layer;

namespace GUI {

	class Widget;
	typedef boost::intrusive_ptr<Widget> WidgetPtr;

	/// Предок всех классов графического интерфейса
	class Widget : public RefCounter
	{
	public:
		typedef std::vector<GUI::WidgetPtr> WidgetContainer;
		typedef std::vector<GUI::WidgetPtr>::iterator WidgetIterator;
		
		Widget(const std::string& name);
		Widget(const std::string& name, rapidxml::xml_node<>* xmlElement);

		void InitWithXml(rapidxml::xml_node<>* xmlElement);

		bool isMouseOver() const;

		void setInputMask(Render::Texture* inputMask);

		int getWidth() const { return width; }
		int getHeight() const { return height; }

		void setClientRect(const IRect& rect);
		const IRect& getClientRect() const { return clientRect; }

		void setPosition(const IPoint& position);
		const IPoint& getPosition() const { return position; }
		IPoint getParentPosition() const;

		const IPoint& getInitPosition() const { return initPosition; }
		void setInitPosition(const IPoint& pos) { initPosition = pos; }

		void SetCenterPosition(const IPoint& pos);
		IPoint GetCenterPosition() const;
		
		void ResetInitPosition();
		void ResetPosition();

		void setActiveWidget();
		Widget* getActiveWidget() const;

		void setColor(const Color& color);
		Color getColor() const;

		void AddChild(Widget* child);

		WidgetContainer& getChildren();

		Widget* getChild(const std::string &name);
		
		// recursive search
		Widget* FindChild(const std::string &name);

		void setParent(Widget* parent);
		Widget* getParent() const;

		void setStatic(bool bStatic);
		bool isStatic() const;

		void setVisible(bool bVis);		
		bool isVisible() const;

		const std::string& getName() const { return name; }

		const std::string& getLayerName() const { return layerName; }
		void setLayerName(const std::string& name) { layerName = name; }

		void DragWidget(const IPoint& mouse_pos);

		virtual bool HitTest(const IPoint& point);

		virtual bool MouseDown(const IPoint& mouse_pos);
		virtual void MouseUp(const IPoint& mouse_pos);
		virtual void MouseMove(const IPoint& mouse_pos);
		virtual void MouseWheel(int delta);
		virtual void MouseCancel();

		virtual bool PinchBegan(float scale, const IPoint &position);
		virtual void PinchChanged(float scale, const IPoint &position);
		virtual void PinchEnded();

		virtual void KeyPressed(int keyCode);
		virtual void KeyReleased(int keyCode);
		virtual void CharPressed(int unicodeChar);
		virtual void KeyFocusCancel();

		virtual void Draw();
		virtual void Update(float dt);

		virtual void Reset();

		virtual void AcceptMessage(const Message& message);
		virtual Message QueryState(const Message& message) const;
		
		virtual bool InternalMouseDown(const IPoint& mouse_pos);
		virtual void InternalMouseUp(const IPoint& mouse_pos);
		virtual void InternalMouseMove(const IPoint& mouse_pos);
		virtual void InternalMouseWheel(int delta);
		
		virtual bool InternalPinchBegan(float scale, const IPoint &position);
		virtual void InternalPinchChanged(float scale, const IPoint &position);
		virtual void InternalPinchEnded();

		virtual void InternalDraw();
		virtual void InternalUpdate(float dt);
		
	protected:
		std::string name;
		std::string layerName;

		Widget* _parent;
		WidgetContainer _children;

		bool _isFreeze;
		bool _isStatic;
		bool _isVisible;
		bool _isMouseCheck;
		bool _isMouseClick;
		bool _isMouseOver;

		IPoint _mouse_down;
		IPoint _click_pos;

		Color _color;

		Render::Texture* _inputMask;
		int _clientOffset;

		IPoint initPosition;
		IPoint position;

		int width;
		int height;
		IRect clientRect;
	
	private:
		Widget(const Widget&);
		Widget& operator=(const Widget&);
	};

} // namespace GUI

#endif // __WIDGET_H__
