#pragma once

#include "Flash/events/FlashEvent.h"

/// Событие изменения размера сцены
extern const std::string& Resize;
/// Событие, отправляемое объекту, когда он добавляется напрямую на сцену
extern const std::string& AddedToStage;
/// Событие, отправляемое объекту, когда он удаляется со сцены
extern const std::string& RemovedFromStage;

extern const std::string& Added;
extern const std::string& Removed;

/// Событие, происходящее при каждой смене кадра в анимации
extern const std::string& PlaybackFrame;
/// Событие, происходящее когда анимация завершилась и готова перезапуститься
extern const std::string& PlaybackEnd;
/// Событие, просиходящее когда анимация начинается с начала
extern const std::string& PlaybackRestarted;

class ResizeEvent: public FlashEvent{
public:
	ResizeEvent(const std::string& type, float oldWidth, float oldHeight, float newWidth, float newHeight);

	float getOldWidth() const {
		return oldWidth;
	}

	float getOldHeight() const {
		return oldHeight;
	}

	float getNewWidth() const {
		return newWidth;
	}

	float getNewHeight() const {
		return newHeight;
	}
private:
	float oldWidth, oldHeight;
	float newWidth, newHeight;
};

class PlaybackEvent: public FlashEvent{
public:
	PlaybackEvent(const std::string& type, int frame);

	int getFrame() const {
		return frame;
	}

private:
	int frame = 0;
};

template<class T>
class QueryEvent: public FlashEvent{
public:
	explicit QueryEvent(const std::string& type):FlashEvent(type), responded(false), value(0){
	}

	~QueryEvent(){
		if ( responded ){
			delete value;
		}
	}

	void respond(const T& value){
		if ( !responded ){
			responded = true;
			this->value = new T(value);
			stopImmediatePropagation();
		}
	}

	bool isResponded() const {
		return responded;
	}

	T get(const T& def) const {
		if ( !responded ){
			return def;
		}
		return *value;
	}

	T get() const {
		if ( !responded ){
			Halt("'get' on unresponded query");
			return T();
		}
		return *value;
	}

private:
	bool responded;
	T* value = nullptr;
};

template<class T>
class ValueEvent: public FlashEvent{
public:
	ValueEvent(const std::string& type, const T& value):FlashEvent(type), value(value){
	}

	const T& get() const{
		return value;
	}
private:
	T value;
};