//◦ Playrix ◦
#ifndef _VIDEO_AD_DELEGATE_H_
#define _VIDEO_AD_DELEGATE_H_

#include "RefCounter.h"

/*
 Реализует логику показа видео-рекламы, специфичную для игры
 */
class VideoAdDelegate : public RefCounter
{
public:
	typedef boost::intrusive_ptr<VideoAdDelegate> Ptr;

public:
	// Вызывается перед инициализацией сервисов видео-рекламы
	virtual void WillInitVideoAdServices() = 0;
	// Вызывается после инициализации сервисов видео-рекламы
	virtual void DidInitVideoAdServices() = 0;

	// Вызывается перед непосредственным запуском ролика (после всех проверок)
	virtual void WillPlayVideoAdByServiceName(const std::string &name) = 0;
	// Вызвыается после запуска ролика
	virtual void DidPlayVideoAdByServiceName(const std::string &name) = 0;

	// Коллбэк на успешный старт ролика
	virtual void OnStartPlayingAdByServiceName(const std::string &name) = 0;
	// Коллбэк на не успешный старт ролика
	virtual void OnFailedToPlayAdByServiceName(const std::string &name) = 0;

	// Текущая версия iOS на устройстве меньше указанной
	virtual bool IsIOSVersionLessThen(const std::string &version) = 0;

	// Сейчас используется тестирование сервисов видео-рекламы?
	// Проверяется только НЕ в продакшене. Если установлен - используются тестовые аккаунты
	virtual bool IsVideoAdTest() = 0;

	// Игра запущена в тестовом окружении?
	virtual bool IsTestEnvironment() = 0;

	// Специфичная для игры проверка на доступность сервисов видео-реклмы
	// Например, запрет показа на слабых девайсах
	virtual bool IsVideoAdServiceDisabled(const std::string &serviceName) = 0;

	// Установка игры на паузу
	virtual void SetPause(bool pause, bool videoAd = false) = 0;

	// На случай падения игры после просмотра видео или на случай вытеснения игры при переходе в браузер из рекламы.
	virtual void SetExpectVideoReward(bool b) = 0;
	// ???
	virtual void SetExpectVideoFinish(bool b, const std::string &serviceName) = 0;
	// Проверяет при старте игры, надо ли дать награду
	virtual void CheckForExpectVideoRewardOnAppStart() = 0;

	// Вызывается, если игрок посмотрел ролик до конца и заслужил награду
	virtual void OnVideoAdReward(const std::string &serviceName) = 0;

	// Вызывается, если игрок не досмотрел рекламу до коцна
	virtual void OnVideoAdCancelled(const std::string &serviceName) = 0;

	// Вызывается при завершении проигрывания видео-рекламы (не зависимо от того, досмотрел до коцна или скипнул)
	virtual void OnVideoAdFinishPlay(const std::string &serviceName) = 0;
	
	virtual bool IsCrossCampaignAllowed() = 0;
	virtual bool IsCrossCampaignDisabled() = 0;
	virtual bool IsCrossCampaignFinished() = 0;
	virtual std::string NextCrossLocation() = 0;
	// Вызывается, когда кликнули по кросс-видео
	virtual void OnCrossVideoAdClick() = 0;

	// Включает/выключает признак того, что сейчас на экране видео-реклама
	virtual void SwitchVideoAdOnScreen(bool onScreen) = 0;
	
	// Проверяет креши в сервисах видео-рекламы при старте игры
	virtual void CheckForVideoAdCrashesOnAppStart() = 0;

	// Сейчас видео-реклама на экране?
	virtual bool IsVideoAdOnScreen() = 0;
	// Обертка над gameInfo, которого теперь нет в движке
	virtual int getConstInt(const std::string &key, int def = 0) = 0;
	virtual bool getConstBool(const std::string &key, bool def = false) = 0;
};
#endif