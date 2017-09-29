#pragma once

#include <boost/intrusive_ptr.hpp>

#include "FlashEvent.h"
#include "TouchEvents.h"

/**
* Класс, преобразующий информацию о касаниях в события
*/
class UIEventManagerBase
{
	struct Touch
	{
		explicit Touch(int _id):
			id(_id),
			initX(0),
			initY(0),
			x(0),
			y(0),
			captured(false),
			active(false),
			inStage(false),
			isMouse(false)
		{
		}

		int id;
		float initX, initY;
		float x, y;
		boost::intrusive_ptr<IFlashDisplayObject> relatedObject;
		bool captured;
		bool active;
		bool inStage;
		bool isMouse;
	};

public:
	UIEventManagerBase();
	explicit UIEventManagerBase(IFlashDisplayObject* _root);
	virtual ~UIEventManagerBase();

	/**
	* Указывает, на то что у нас только один палец(указетель миши) и никакого мультитача нету,
		Это значит что все собития тача будут иметь идентификатор 0
	*/
	void setMouseInputAtZero();

	int touchBegin(float x, float y);
	void touchMove(int touchId, float x, float y);
	void touchEnd(int touchId, float x, float y);
	void touchCancel(int touchId);

	void touchLeave(int touchId);
	void touchEnters(int touchId);

	void touchReset();
private:
	/**
		Занимается рассылкой событий(events), когда в течении одного касания сменяются элементы
		под курсором/пальцем(водим им по экрану, не отрывая от экрана),

		таким образом некоторым элементам посылается событи leave, а некоторым enter, чтобы элемент всегда знал находиться ли он под курсором
	*/
	void __touchRelatedObjectChanged(int touchId, IFlashDisplayObject* prev, IFlashDisplayObject* next);
	Touch& __getFreeTouch(); //фабрика структур Touch

protected:
	IFlashDisplayObject* root;
	/*
		это счетчик назначение его знать touch который произведен это точ в сериии касаний или нет

		Т.е представим себе ситуацию когда сцена чиста от касаний(человек ничего не нажимает) тогда первое касание это будет главное,
		и если человек в момент когда произошло главное касание не уберая первого пальца, коснулся вторым это будет вторичное касание,
		и как бы получается группа касаний в рамках одного жеста(несколько сдандартных жестов перечисленны здесь http://developer.android.com/design/patterns/gestures.html)

		Вот чтобы и ловить такие ситуации и призван этот счетчик
	*/
	int capturedTouches;
	std::vector<Touch> touches;

};

class UIEventManager: public UIEventManagerBase
{
public:
	UIEventManager();
	explicit UIEventManager(IFlashDisplayObject* _root);
	virtual ~UIEventManager();

	/**
	* Устанавливает объект, который является корнем иерархии. Корень всегда получает
	* события касания, даже если они не попали ни в один объект.
	*/
	void setRoot(IFlashDisplayObject* _root);
};
