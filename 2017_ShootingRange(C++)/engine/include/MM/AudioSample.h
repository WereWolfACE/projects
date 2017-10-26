#ifndef __AUDIOSAMPLE_H__
#define __AUDIOSAMPLE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "MM/AudioSource.h"
#include "RefCounter.h"

#include <queue>

#include <boost/atomic/atomic.hpp>

#if defined(ENGINE_TARGET_IPHONE) || defined(ENGINE_TARGET_MACOS)
#	include <OpenAL/al.h>
#	include <OpenAL/alc.h>
#else
#	include <al.h>
#	include <alc.h>
#endif

template<typename T>
class ObservableValue {
public:
	ObservableValue()
		: _value(), _changed(false)
	{ }

	explicit ObservableValue(const T& v)
		: _value(v), _changed(true)
	{ }

	ObservableValue(const ObservableValue& rhs)
		: _value(rhs._value), _changed(true)
	{ }

	ObservableValue& operator=(const T& value) {
		_value = value;
		_changed = true;
		return *this;
	}

	ObservableValue& operator=(const ObservableValue& rhs) {
		if (this != &rhs) {
			_value = rhs._value;
			_changed = true;
		}
		return *this;
	}

	operator T() const { return _value; }

	T GetValue() const { return _value; }

	bool IsChanged() const { return _changed; }

	void ClearChanged() { _changed = false; }

private:
	// Чтение и установка значений может происходить из разных потоков
	boost::atomic<T> _value;
	boost::atomic<bool> _changed;
};

namespace MM {

class AudioManager;
class AudioResource;

///
/// Звуковой фрагмент, проигрывающий источник звуковых данных
///
class AudioSample : public RefCounter {
public:
	AudioSample(AudioManager* owner, AudioSource* audio, AudioResource* resource, bool streamable, bool is_track);

	~AudioSample();

	void Play();

	void Pause();

	void Stop();

	bool IsPlaying() const;

	bool IsPaused() const;

	bool IsFinished() const;

	void SetVolume(float value);

	// Высота тона (скорость воспроизведения): 0.5 .. 2.0
	void SetPitch(float value);

	// -1 - левый канал, 0 - оба канала, 1 - правый канал
	void SetPan(float value);

	void SetLooping(bool value);

	bool IsLooping() const;

	// Позиция воспроизведения: 0.0 .. 1.0
	void SetPosition(float value);

	// Позиция воспроизведения: 0.0 .. 1.0
	float GetPosition() const;

	void Update();

	bool IsPlayingSync() const { return _currentState == PLAYING; }

	bool IsPausedSync() const { return _currentState == PAUSED; }

protected:
	bool CreateSourceIfNeeded();

	void Destroy();

	void PlayNonStreamable();

	void DoPlay();

	void DoPause();

	void DoStop();

	// Читает порцию данных из потока и помещает в очередь воспроизведения.
	// Если buffer <= 0, то будет создан новый буфер.
	bool EnqueueStreamable(ALuint buffer = 0);

	// Продолжает воспроизведение, если источник был остановлен
	bool GoOnStreamable();

	// Удаляет буфер из _buffers и AL
	void DeleteBuffer(ALuint buffer);

protected:
	AudioManager* _owner;

	AudioSourcePtr _audio;

	AudioResource* _resource;

	ALuint _source;

	typedef std::deque<ALuint> Buffers;

	Buffers _buffers; // Список всех созданных буферов. Они удаляются при уничтожении AudioSample.

	bool _streamable;

	bool _is_track;

	ObservableValue<bool> _looping;

	ObservableValue<float> _volume;

	ObservableValue<float> _pitch;

	ObservableValue<float> _pan;

	ObservableValue<float> _audio_pos;

	enum State { NONE, PLAYING, PAUSED, STOPPED } _currentState;

	std::queue<State> _deferredState;
};

typedef boost::intrusive_ptr<AudioSample> AudioSamplePtr;

} // namespace MM

#endif // __AUDIOSAMPLE_H__
