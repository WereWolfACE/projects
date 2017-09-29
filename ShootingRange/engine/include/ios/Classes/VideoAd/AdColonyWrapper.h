#ifndef _AD_COLONY_WRAPPER_H_
#define _AD_COLONY_WRAPPER_H_

#include "VideoAdService.h"

// Конфиг сервиса (хэши приложений для продакшена и тестов)
struct AdColonyConfig
{
	std::string adColony_appId_Production;
	std::string adColony_zone0_Production;

	std::string adColony_appId_Test;
	std::string adColony_zone0_Test;
};

class AdColonyWrapper : public VideoAdService
{
public:
	AdColonyWrapper(VideoAdDelegate::Ptr delegate, const AdColonyConfig &config);

	virtual void Init();
	virtual void Play();
	virtual bool CanVideoBeAdded();
};

#endif // ADCOLONYWRAPPER_H
