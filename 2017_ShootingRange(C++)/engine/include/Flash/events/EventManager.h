#pragma once

#include "Flash/events/FlashEvent.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

struct IFlashDisplayObject;

struct EventHandler{
	boost::function<void(FlashEvent&)> callee;
	size_t hash = 0;

	EventHandler(){
	}

	EventHandler(boost::function<void(FlashEvent&)> callee, size_t hash):callee(callee),hash(hash){
	}
};

template<class E>
struct CallHandlerWithRTCheck{
	explicit CallHandlerWithRTCheck(const boost::function<void(E&)>& f):f(f){
	}

	void operator()(FlashEvent& e){
		f(dynamic_cast<E&>(e));
	}

	boost::function<void(E&)> f;
};

template<>
struct CallHandlerWithRTCheck<FlashEvent>{
	CallHandlerWithRTCheck(const boost::function<void(FlashEvent&)>& f):f(f){
	}

	void operator()(FlashEvent& e){
		f(e);
	}

	boost::function<void(FlashEvent&)> f;
};

template<class C, class E>
EventHandler makeMemberHandler(C* c, void(C::*f)(E&)){
	size_t hash = *((size_t*)c + (sizeof(c)-sizeof(size_t))/sizeof(size_t));
	unsigned char* p = (unsigned char*)*((unsigned char**)&f);
	for ( int i = 0; i < sizeof( void(C::*)(E&) ); i++ ){
		//Делим на половины и меняем их друг с другом местами
		hash = ((hash << (sizeof(size_t) * 8 / 2 + 1) ) | (hash >> (sizeof(size_t) * 8 / 2 - 1))) ^ p[i] + (p[i] << 11);
	}
	return EventHandler(CallHandlerWithRTCheck<E>(boost::function<void(E&)>(boost::bind( f, c, _1 ))), hash);
}

#define MEMBER_HANDLER(c, f) makeMemberHandler(c, &f)

typedef unsigned int HandlerID;

struct EHStorage;

/**
* Служит для регистрации слушателей событий и отправки событий
*/
class EventManager{
public:
	/**
	* Отправляет событие цели.
	* Событие проходит фазу захвата (спускается от корня до цели),
	* затем фазу цели, и, если
	* разрешено, фазу пузырей (поднимается от цели до корня)
	*/
	void dispatch(IFlashDisplayObject* target, FlashEvent& event);
	/**
	* Отправляет событие напрямую цели, игнорируя фазы захвата и пузырей
	*/
	void dispatchTarget(IFlashDisplayObject* target, FlashEvent& event);
	/**
	* Отправляет событие цели root. При этом параметр captureAndBubbles указывает, будут ли
	* присутствовать фазы захвата и пузырей. После того, как цель получила событие, если
	* событие не было отменено при помощи preventDefault, событие отправляется всем дочерним
	* элементам цели.
	*/
	void broadcast(IFlashDisplayObject* root, FlashEvent& event, bool captureAndBubbles = false);

	/**
	* Устанавливает обработчик события handler для типа type на цель target.
	* Параметр isCapture определяет, активируется ли обработчик в фазу захвата (true) либо в фазу
	* цели и пузырей (false).
	* Возвращаемый HandlerID можно использовать для удаления обработчика.
	* Если обработчик handler уже установлен для данного события, то он не будет добавлен повторно.
	*/
	HandlerID addEventListener(IFlashDisplayObject* target, const std::string& type, const EventHandler& handler, bool isCapture = false);

	/**
	* Удаляет обработчик события по HandlerID
	*/
	void removeEventListener(HandlerID id);
	/**
	* Удаляет обработчик события, установленный вызовом addEventListener(target, type, handler)
	*/
	void removeEventListener(IFlashDisplayObject* target, const std::string& type, EventHandler handler);
	/**
	* Удаляет все обработчики событий на цели
	*/
	void removeEventListeners(IFlashDisplayObject* target);
	/**
	* Определяет, есть ли для цели обработчик данного события.
	* Игнорирует обработчики для фазы захвата.
	*/
	bool willTriggerAt(IFlashDisplayObject* target, FlashEvent& event);

	/**
	* Возвращает глобальный экземпляр EventManager
	*/
	static EventManager& Get();
private:

	struct EventHandlerRecord
	{
		EventHandlerRecord(const bool _isCapture, const EventHandler& _handler, const HandlerID _id):
			isCapture(_isCapture),
			handler(_handler),
			id(_id)
		{
		};

		bool isCapture;
		EventHandler handler;
		HandlerID id;
	};

	typedef std::multimap<std::string, EventHandlerRecord> TargetListeners;
	std::map<IFlashDisplayObject*, TargetListeners> allListeners;
	std::map<HandlerID, IFlashDisplayObject*> handlersIndex;

	void collectHandlers(EHStorage* storage, IFlashDisplayObject* target, FlashEvent& event);
	void doDispatch(EHStorage* storage, FlashEvent& event);

	HandlerID nextId;
};
