#ifndef _GUI_EDITBOX_H_
#define _GUI_EDITBOX_H_

#include "GUI/Widget.h"
#include "Render/RenderFunc.h"

namespace GUI
{

class FTEditBox : public GUI::Widget
{
public:
	FTEditBox(const std::string& name, rapidxml::xml_node<>* elem);
	virtual void Update(float dt) override;
	virtual void Draw() override;
	virtual void AcceptMessage(const Message& message) override;
	virtual Message QueryState(const Message& message) const override;
	virtual bool MouseDown(const IPoint& mouse_pos) override;
	virtual void KeyPressed(int keyCode) override;
	virtual void CharPressed(int keyCode) override;

	void SetChangeCallback(luabind::object obj);

protected:
	std::string GetUtf8Text() const;
	bool SetText(const std::wstring& text);
	bool SetText(const std::string& text);
	bool InsertChar(size_t pos, wchar_t ch);

protected:
	std::wstring _text;
	int _limit;
	size_t _ibeamPos;

	std::string _font;
	std::string _filterFont; // Дополнительный шрифт для фильтрации символов. Может быть полезно, если для вывода текста будет использоваться не тот же шрифт, каким он вводится в EditBox
	bool _centered;
	int _adjustWidth;

	std::set<int> _denied;
	float _timer;
	float _kbTimer;
	bool _enabled;
	bool _uppercase;
    bool _autoShowKeyboard;

	luabind::object _onTextChanged;
};

} // namespace GUI

#endif
