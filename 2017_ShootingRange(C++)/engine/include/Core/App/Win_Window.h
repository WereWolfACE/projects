#ifndef _CORE_WINDOW_WIN_H__
#define _CORE_WINDOW_WIN_H__

#pragma once

#include "Render/RenderTypes.h"
#include "Utils/IRect.h"

namespace Core
{

class Window
{
public:
	Window();
	~Window();

	HWND GetHandle() const { return hWnd; }

	void Destroy();

	void Show();

	void Resize(int width, int height);

	void ToggleFullscreen();
	DeviceMode GetWindowMode() const { return _mode; }
	void CheckWindowModeChange();

	IRect GetClientSizes();
	bool IsCursorIn() const;

private:
	static LRESULT CALLBACK WindowFuncStub(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	IRect CalculateWindowRect(DeviceMode mode, DWORD style, DWORD exStyle, int clientWidth, int clientHeight);
	void SetMode(DeviceMode mode);

private:
	std::wstring _className;
	DeviceMode _mode;
	HWND hWnd;
	
	/// этим флагом окно сообщает, что пользователь хочет переключить режим
	bool needChangeMode;
};

}

#endif // _CORE_WINDOW_WIN_H__
