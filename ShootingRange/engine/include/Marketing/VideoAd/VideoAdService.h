//◦ Playrix ◦
#ifndef _VIDEO_AD_SERVICE_H_
#define _VIDEO_AD_SERVICE_H_

#include "RefCounter.h"
#include "VideoAdDelegate.h"

/*
 Базовый класс поддерживаемых видео-сервисов
 */
class VideoAdService : public RefCounter
{
public:
	typedef boost::intrusive_ptr<VideoAdService> Ptr;
	typedef std::vector<VideoAdService::Ptr> Collection;

public:
	// В конструкторе не должно быть инициализации самого сервиса, только установка значений переменных
	// Вся инициализация в методе Init
	VideoAdService(VideoAdDelegate::Ptr delegate);
	virtual ~VideoAdService();

	// Инициализация сервиса видео-рекламы, вызовы функций SDK и т.д.
	// Вызывается из менеджера видео-рекламы
	virtual void Init();

	// Запускает видео
	virtual void Play();

	// Можно ли показать видео-рекламу этого сервиса? Она может быть запрещена настройками или
	// внутренним состоянием сервиса
	virtual bool CanVideoBeAdded();

	// Запрещен ли этот сервис?
	bool IsDisabled();
	bool IsEnabled();

	// Запрет на показ на слабых девайсах
	bool IsLowMemoryDisabled();
	bool IsMediumMemoryDisabled();

	// Запрет на детект крешей
	bool IsCrashDetectionDisabled();

	// Возвращает приоритет сервиса видео-рекламы
	// Если не определен, возвращает -1
	int GetOrder() const;

	// Устанавливает приоритет сервиса. Может быть изменен после запуска по разным причинам
	// (например, сервис часто падает - ставится в конец очереди)
	void SetOrder(int order);

	// Вовзращает идентификатор сервиса видео-рекламы
	const std::string& GetName() const { return _name; }

protected:
	// Инициализиует порядок параметром из констант (обычно скачиваемая xml-ка)
	// Вызывать в конструкторе наследника после инициализации _name
	int GetOrderFromConsts() const;

protected:
	VideoAdDelegate::Ptr _delegate;

	// Идентификатор (имя) сервиса видео-рекламы
	std::string _name;

	// Приоритет показа сервиса. Должен инициализироваться из xml в конструкторе наследника,
	// может быть изменен в методе Init
	int _order;
};

#endif