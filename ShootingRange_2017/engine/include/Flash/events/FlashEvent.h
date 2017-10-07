#pragma once

#include "Flash/core/FlashCommon.h"

struct IFlashDisplayObject;
class EventManager;

/**
* Фаза распространения события.
* Фаза захвата - спуск события от корня иерархии до целевого узла.
* Фаза цели - обработка события в целевом узле.
* Фаза пузырей - подъем события от целевого узла до корня иерархии.
*/
enum class EventPhaseType {
	Capture,
	Target,
	Bubbles
};

/**
* Базовый класс для события.
* Для собственных событий нужно наследоваться от этого класса.
*/
class FlashEvent {
public:
	/**
	* @param bubbles определяет, должна ли для события запускаться фаза пузырей
	*/
	FlashEvent(const std::string& type, bool bubbles = true);
	virtual ~FlashEvent() { }

	const std::string& getType() const;
	/**
	* Получает текущую фазу распространения события
	*/
	EventPhaseType getPhase() const;

	/**
	* Останавливает распространение события.
	* Отменяет дальнейшее распространение события за пределы узла, в котором то обрабатывается в ланный момент.
	*/
	void stopPropagation();
	/**
	* Останавливает распространение события, включая оставшиеся обработчики в текущем узле.
	*/
	void stopImmediatePropagation();

	/**
	* Возвращает целевой узел события.
	*/
	IFlashDisplayObject* getTarget() const;
	/**
	* Возвращает имя целевого узла события если существует.
	*/
	const std::string getTargetName() const;
	/**
	* Возвращает текущий узел, в котором ведется обработка события.
	*/
	IFlashDisplayObject* getCurrentTarget() const;

	/**
	* Вызывается при смене текущего узла события
	*/
	virtual void visitNode(IFlashDisplayObject* node){}

	/**
	* Отменяет поведение по умолчанию для события.
	* Для разных событий поведение по умолчанию может иметь разный смысл
	*/
	void preventDefault();
	/**
	* Определяет, был ли вызван метод preventDefault ранее
	*/
	bool isDefaultPrevented() const;
private:
	FlashEvent(const FlashEvent& event);

	const FlashEvent& operator=(const FlashEvent& event);

	std::string type;
	EventPhaseType phase;

	IFlashDisplayObject* target;
	IFlashDisplayObject* currentTarget;

	bool bubbles, propagationStopped, propagationStoppedImmediatly;
	bool defaultPrevented;

	friend class EventManager;
};