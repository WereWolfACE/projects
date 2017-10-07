#ifndef _CORE_CURSOR_H_
#define _CORE_CURSOR_H_

#include "Render/Texture.h"

namespace GUI
{

class Cursor
{
public:
	static IPoint HOT_POINT_REMOVAL;

	Cursor();
	virtual ~Cursor();

	virtual void setNormal();
	virtual void setActive();
	virtual void setPressed();
	virtual void setForbidden();
	virtual void Draw(const IPoint& pos);
	virtual bool isActive() const;
	virtual bool isPressed() const;
	virtual bool isForbidden() const;
	virtual void Update(float dt);

	virtual void Hide();
	virtual void Show();
	virtual bool isHidden() const;

	// Показать системный курсор мыши.
	static void ShowSystem();
	// Скрыть системный курсор мыши. Обычно чтобы его самим нарисовать.
	static void HideSystem();

	static Cursor* getCursor();

protected:
	Render::TexturePtr _normal;
	Render::TexturePtr _active;
	Render::TexturePtr _pressed;
	Render::TexturePtr _forbidden;
	Render::Texture* _current;
	bool _isActive;
	bool _isPressed;
	bool _hidden;

	static Cursor* _cursor;
};

} // namespace GUI


namespace Core
{

class CursorManager
{
public:

	bool enableCursorHandling;
	GUI::Cursor *cursor;

	CursorManager();
	~CursorManager();

	void CheckCursorState();
	void DrawCursor();

protected:

	bool showCustomCursor;
};

}

#endif
