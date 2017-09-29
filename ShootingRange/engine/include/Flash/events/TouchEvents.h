#pragma once

#include "Flash/events/FlashEvent.h"

///Событие начала касания
extern const std::string& TouchBegin;
///Событие окончания касания
extern const std::string& TouchEnd;
///Событие перемещения касания
extern const std::string& TouchMove;
///Событие, означающее, что точка касания покинула пределы объекта
extern const std::string& TouchLeave;
///Событие, означающее, что точка касания вошла в пределы объекта
extern const std::string& TouchEnter;
///Событие перемещения курсора мыши без нажатой кнопки
extern const std::string& TouchPassiveMove;
///Событие, означающее, что касание было отменено
extern const std::string& TouchCancel;

///Событие, означающее, что касание покинуло пределы сцены (окна приложения)
extern const std::string& TouchLeaveStage;
///Событие, означающее, что касание вернулось в пределы сцены
extern const std::string& TouchEnterStage;

///Событие, означающее, что точка касания вошла в пределы объекта.
///Различие с TouchEnter в том, что TouchRollOver не имеет фазы пузырей и проще в обработке.
///Поведение скопировано с событий MouseEvent.MOUSE_OVER / MouseEvent.ROLL_OVER в flash player.
///Скорее всего вам нужно именно TouchRollOver.
extern const std::string& TouchRollOver;
///Событие, означающее, что точка касания покинула пределы объекта.
extern const std::string& TouchRollOut;

///Событие щипкового жеста
extern const std::string& GesturePinch;

class TouchEvent: public FlashEvent{
public:
	TouchEvent(const std::string& type, int touchIndex, float globalX, float globalY);

	/**
	* ID касания - для поддержки multitouch интерфейсов
	*/
	int getTouchIndex() const {
		return touchIndex;
	}

	float getGlobalX() const {
		return globalX;
	}

	float getGlobalY() const {
		return globalY;
	}

	float getLocalX() const {
		return localX;
	}

	float getLocalY() const {
		return localY;
	}

	void visitNode(IFlashDisplayObject* node);
private:
	int touchIndex;
	float globalX, globalY;
	float localX, localY;
};

class PinchEvent: public FlashEvent{
public:
	PinchEvent(const std::string& type, float globalX, float globalY, float scale);

	float getGlobalX() const {
		return globalX;
	}

	float getGlobalY() const {
		return globalY;
	}

	float getLocalX() const {
		return localX;
	}

	float getLocalY() const {
		return localY;
	}

	float getScale() const {
		return scale;
	}

	void visitNode(IFlashDisplayObject* node);

private:
	int touchIndex;
	float globalX, globalY;
	float localX, localY;
	float scale;
};