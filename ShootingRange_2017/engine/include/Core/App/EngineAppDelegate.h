#ifndef _CORE_ENGINEAPPDELEGATE_H_
#define _CORE_ENGINEAPPDELEGATE_H_

#include "Render/RenderTypes.h"


enum class ScreenOrientation
{
	Undefined, // ориентация неопределена из-за ошибки или горизонтального положения устройства
	LandscapeLeft, // кнопки слева
	Portrait, // кнопки снизу
	LandscapeRight, // кнопки справа
	PortraitUpsideDown, // кнопки сверху
};

enum class ViewOrientations
{
	LandscapeOnly,
	PortraitOnly,
	LandscapeAndPortrait
};

enum class NotificationType
{
	Local,
	Push
};

enum class ApplicationStatus
{
	AlreadyRunning,
	LaunchedNow
};

enum class CursorType
{
	System,
	Custom
};

enum class SoundSettings
{
	WithSound,
	WithoutSound
};

namespace GUI
{
	class Cursor;
}


namespace Core
{

class EngineAppDelegate
{
public:

	EngineAppDelegate()
		: _isLoadDelayed(false)
	{}
	virtual ~EngineAppDelegate() {}

	virtual void OnAppBeginLaunching() {}
	virtual void OnAppLaunched() {} // запуск - вызывается 1 раз
	virtual void OnAppTerminated() {} // остановка - вызывается 1 раз
	virtual void OnAppShown() {} // показ приложения - вызывается после запуска на всех платформах и при разворачивании на ios, android
	virtual void OnAppCollapsed() {} // скрытие приложения - вызывается перед остановкой на всех платформах и при сворачивании на ios, android
	virtual void OnAppActivated() {} // получение фокуса / снятие с паузы
	virtual void OnAppDeactivated() {} // потеря фокуса / постановка на паузу

	virtual void OnContextRecreated() {} // сброс контекста - android

	virtual void OnScreenOrientationChanged(ScreenOrientation newOrientation) {}
		// ios, android | вызывается после OnScreenSizeChanged, либо сам по себе | может вызываться до OnAppLaunched
	virtual ViewOrientations SupportedViewOrientations() { return ViewOrientations::LandscapeOnly; } // ios, android | на андроиде вызывается из ui-потока

	virtual void GameContentSize(int deviceWidth, int deviceHeight, int &width, int &height) {}
		// deviceWidth, deviceHeight - размеры view на ios, android, максимальные размеры клиентской области окна на win, mac
	virtual void OnScreenSizeChanged(int newWidth, int newHeight) {}
		// изменение размеров окна или view, в т.ч. при повороте, в момент вызова рендер уже перенастроен

	virtual void ScreenMode(DeviceMode &mode) {} // win, mac
	virtual void OnEnterFullscreenMode() {} // win, mac
	virtual void OnEnterWindowMode() {} // win, mac

	virtual float RefreshRateFactor() { return 1.0f; } // 1 - 60 fps, 2 - 30 fps и т.д., на ios значение округляется
	virtual void ResetRefreshRateFactor() {}           // Обновляет fps у glview

	virtual void ActiveCursorType(CursorType &cursorType) {} // mac + win?
	virtual void CursorObject(GUI::Cursor **cursor) {} // mac + win?

	virtual void OnFrameBegin() {}
	virtual void OnUpdate(float dt) {}
	virtual void OnPreDraw() {}
	virtual void OnPostDraw() {} // здесь можно рисовать паузу, fps и т.д.
	virtual void OnDrawSplash() {} // mac | здесь можно отрисовать сплеш, чтобы окно показалось сразу с картинкой, а не с серым фоном

	void InitPushNotifications(SoundSettings setting);
	virtual void NotificationActions(bool &disabled, std::string &title) { disabled = false; title = ""; }
	virtual void OnNotificationReceived(NotificationType type, ApplicationStatus appStatus, const std::string &payload, void *rawData) {}
		// ios, android, mac | при запуске через уведомление вызывается до OnAppLaunched
	virtual void OnPushTokenReceived(void *token) {} // mac, ios

	virtual bool OnUrlActivate(const std::string &url, const std::string &app) { return false; } // ios, mac, android | может вызываться до OnAppLaunched на маке и андроиде

	virtual void OnResourceLoaded() {} // вызывается при загрузке файла текстов, слоя или описания ресурса (бывший LoadScreen::Update)
	virtual void PreloadResources() {} // загрузка минимально-необходимых для запуска игры данных
	virtual void LoadResources() {} // загрузка всех остальных ресурсов
	virtual void OnMemoryWarning() {} // ios, android

	virtual void RegisterTypes() {} // регистрация виджетов и типов в lua

	virtual void OnDragFiles(const std::vector<std::string> &filenames) {} // win, mac?

	virtual std::string ActiveGameLocale() { return "en"; }

	virtual void OnNextFrameWithLoadDelayed() {}
	bool IsLoadDelayed() const { return _isLoadDelayed; }
	void DelayLoad() { _isLoadDelayed = true; }
	void ResumeLoad();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void OnAppLaunchedInternal();
	void OnAppTerminatedInternal();
	void OnAppShownInternal();
	void OnAppCollapsedInternal();
	void OnAppActivatedInternal();
	void OnAppDeactivatedInternal();

	void OnFrameBeginInternal();
	void OnUpdateInternal(float dt);
	void OnPreDrawInternal();
	void OnPostDrawInternal();

private:

	bool _isLoadDelayed;
};

extern EngineAppDelegate *appDelegate;

}

#endif //_CORE_ENGINEAPPDELEGATE_H_
