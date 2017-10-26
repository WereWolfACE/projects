#ifndef _CHARTBOOST_WRAPPER_H_
#define _CHARTBOOST_WRAPPER_H_

#include "VideoAdService.h"

// Конфиг сервиса (хэши приложений для продакшена и теста)
struct ChartboostConfig
{
	std::string chartboost_appId_Production;
	std::string chartboost_appSignature_Production;
	std::string chartboost_appId_Test;
	std::string chartboost_appSignature_Test;
};

class ChartboostWrapper : public VideoAdService
{
public:
	ChartboostWrapper(VideoAdDelegate::Ptr delegate, const ChartboostConfig &config);
	
	virtual void Init();
	virtual void Play();
	virtual bool CanVideoBeAdded();
};

class ChartboostCrossWrapper : public VideoAdService
{
public:
	ChartboostCrossWrapper(VideoAdDelegate::Ptr delegate, const ChartboostConfig &config);
	
	virtual void Init();
	virtual void Play();
	virtual bool CanVideoBeAdded();
};

#endif // CHARTBOOSTWRAPPER_H
