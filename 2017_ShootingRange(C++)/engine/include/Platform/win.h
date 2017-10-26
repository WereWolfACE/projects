#ifndef _PLATFORM_WIN_H_
#define _PLATFORM_WIN_H_

#include "IO/Stream.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <string>

namespace utils {

bool IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor);
bool AllowAllAccess(const std::string& theFileName);

void MakeWindowForeground(HWND window);
void CenterWindow(HWND window);

float GetScreenDpiScale();

bool ValidateXmlFile(const std::string& filename);
bool ValidateXmlFile(const IO::InputStreamPtr& stream, const std::string& filename = std::string());

bool IsPushNotificationsEnabled();

//Класс для проверки существования окна приложения.
//Мютекс создается в конструкторе и хранится до самого закрытия.
class SingleInstance {
public:
	SingleInstance(const std::string &name, const std::string &wndClass, const std::string &wndCaption);
	~SingleInstance();

	bool Check() const;

private:
	HANDLE _mutex;
	std::wstring _name;
	std::wstring _wndClass;
	std::wstring _wndCaption;
	bool _alreadyRunnging;
};

}
#endif
