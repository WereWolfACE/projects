//◦ Playrix ◦
#ifndef _VIDEO_AD_SERVICE_MANAGER_H_
#define _VIDEO_AD_SERVICE_MANAGER_H_

#include "VideoAdService.h"
#include "VideoAdDelegate.h"

/*
 Менеджер видео-рекламы
 Управляет поддерживаемыми сервисами - инициализирует, запускает
 Игровая логика должна быть реализована в делегате
 */
class VideoAdServiceManager
{
public:
    static VideoAdServiceManager& instance();

	// Инициализация сервисов видео-рекламы, вызовы к sdk
	void Init(VideoAdService::Collection services, VideoAdDelegate::Ptr delegate);
	bool IsInited() const { return _videoAdInited; }

	// Проигрывает видео-рекламу сервиса по его приоритету
	void PlayVideoByServiceOrder(int order);
	// Проигрывает видео-рекламу сервиса по его имени
	void PlayVideoAdByServiceName(const std::string &name);

	// Возвращает указатель на сервис по имени или nullptr, если сервиса нет
	VideoAdService::Ptr GetServiceByName(const std::string &serviceName);
	// Возвращает указатель на сервис по его порядку или nullptr, если сервис с таким порядком не найден
	VideoAdService::Ptr GetServiceByOrder(int order);

	const VideoAdService::Collection & GetVideoAdServices() const { return _services; }

private:
    VideoAdServiceManager();

private:
	// Список поддерживаемых сервисов
	VideoAdService::Collection _services;

    // Инициализирован ли менеджер видео-рекламы
    bool _videoAdInited;

	VideoAdDelegate::Ptr _delegate;
};

#endif // _VIDEO_AD_H_