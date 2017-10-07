#ifndef __IMGUIWIDGET_H__
#define __IMGUIWIDGET_H__

#pragma once

#include "GUI/Widget.h"
#include "Render/Texture.h"
#include "Render/ShaderProgram.h"
#include "KeyboardManager.h"

namespace GUI {

class ImGuiWidget : public Widget, public IKeyboardNotificationReceiver {
public:
	ImGuiWidget(const std::string& name, rapidxml::xml_node<>* elem);
	~ImGuiWidget();

	ImGuiWidget(const ImGuiWidget&) = delete;
	ImGuiWidget& operator=(const ImGuiWidget&) = delete;

	enum ScaleMode { PhysicalScale, LogicalScale };

	ScaleMode GetScaleMode() const { return _scaleMode; }
	void SetScaleMode(ScaleMode scaleMode) { _scaleMode = scaleMode; }

	virtual void OnGUI();

	virtual void Update(float dt) override;
	virtual void Draw() override;

	virtual bool MouseDown(const IPoint& mouse_pos) override;
	virtual void MouseWheel(int wheel) override;
	virtual void MouseCancel() override;
	
	virtual void KeyPressed(int keyCode) override;
	virtual void KeyReleased(int keyCode) override;
	virtual void CharPressed(int unicodeChar) override;

	virtual void AcceptMessage(const Message& message) override;

	virtual void OnKeyboardWillHide() override;

private:
	static void SetClipboardText(const char* text);
	static const char* GetClipboardText();

	void InitializeInput();
	void InitializeResources(rapidxml::xml_node<>* elem);

	void ProcessKeyboard();

	void ProcessButton(int key, uint8_t &_state);

	void FinishProcessInput();

	static bool IsDecimalField(const char *text);

private:
	struct Resources : public RefCounter {
		Render::TexturePtr tex;
		Render::ShaderProgramPtr shader;
	};

	typedef boost::intrusive_ptr<Resources> ResourcesPtr;

	static ResourcesPtr _shared_res;
	ResourcesPtr _res;

	std::unique_ptr<uint8_t[]> _state;

	ScaleMode _scaleMode = LogicalScale;

	bool _processingInputField = false;
	bool _keyboardWillHide = false;
	uint8_t _pressedBackButton = 0;
	uint8_t _pressedEnterButton = 0;
	std::vector<int> _charsPressed;
};

} // namespace GUI

#endif // __IMGUIWIDGET_H__
