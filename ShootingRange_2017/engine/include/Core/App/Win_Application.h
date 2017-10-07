#ifndef __CORE_APPLICATION_WIN_H__
#define __CORE_APPLICATION_WIN_H__

#pragma once

namespace Core
{

class EngineAppDelegate;
class Window;

void RunApplicationWithDelegate(EngineAppDelegate *dlgt);

class Application : public ApplicationBase {
public:
	// название окна программы (должно быть заполнено до вызова RunApplicationWithDelegate())
	static std::wstring APPLICATION_NAME;

	Application();
	~Application();

	Window* GetMainWindow() { return wnd.get(); }

	void Init();
	
	// основной цикл приложения
	void Start();
	void Stop();

	void CloseWindow();

	bool IsFullscreen() const;
	void ToggleFullscreen();

protected:
	std::unique_ptr<Window> wnd;

	bool mainLoop;

	virtual void RegisterTypes() override;
};

} // namespace Core

#endif // __CORE_APPLICATION_WIN_H__
