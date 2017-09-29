#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "MM/AudioSample.h"
#include "MM/AudioResource.h"

#include <boost/function.hpp>

#ifdef _DEBUG
#define AL_ERRORS_CHECK AlErrorsCheck(__FILE__, __LINE__)
#else
#define AL_ERRORS_CHECK
#endif
#define AL_ERRORS_CLEAR AlErrorsClear();

void AlErrorsCheck(const char *filename, int line);
void AlErrorsClear();

namespace MM {

class AudioManager {
public:
	enum CallbackAction {
		OnStop
	};

	AudioManager();	
	~AudioManager();
	
	static AudioManager& Self();
	
	void Initialize();
	void Shutdown();

	bool IsDevicePresent() const;
	
	/// Запустить заданный семпл.
	/// Возвращает уникальный идентификатор буфера воспроизведения или -1 при отказе.
	int PlaySample(const std::string& id, bool looping = false, float volume = 1.0f, float pitch = 1.0f);
	/// Запустить заданный трек. Аналогичен PlaySample.
	int PlayTrack(const std::string& id, bool looping = false, float volume = 1.0f, float pitch = 1.0f);
	/// Ставит звук в очередь на воспроизведение.
	int QueueSample(const std::string& id, int sample, bool looping = false, float volume = 1.0f, float pitch = 1.0f);
	/// Ставит трек в очередь на воспроизведение. Аналогичен QueueSample.
	int QueueTrack(const std::string& id, int sample, bool looping = false, float volume = 1.0f, float pitch = 1.0f);
	/// Проверить идентификатор буфера на валидность
	bool IsValid(int id) const;
	/// Возвращает идентификатор буфера для текущего музыкального трека (или -1 в случае отсутствия такового)
	int GetTrackId() const;
	/// Запаузить / отпаузить все играющие сейчас звуки [кроме трека, если butTrack = true]
	void PauseAll(bool pause, bool butTrack = false);
	/// Запаузить/отпаузить заданный звуковой буфер
	void PauseSample(int id, bool pause);
	/// Проверить запаузен ли буфер
	bool IsPaused(int id) const;
	/// Остановить воспроизведение
	void StopAll();
	/// Остановить воспроизведение заданного сэмпла
	void StopSample(int id);
	/// Остановить воспроизведение трека
	void StopTrack();
	/// Установить позицию воспроизведения (0..1)
	void SetPos(int id, float position);
	/// Вернуть позицию воспроизведения (0..1)
	float GetPos(int id) const;
	/// Установить громкость (для вновь создаваемых буферов громкость равна 1.0f)
	/// Реальная громкость будет умножена на SoundVolume.
	void SetVolume(int id, float volume);
	/// Установить громкость (для вновь создаваемых буферов громкость равна 1.0f)
	/// Реальная громкость будет умножена на MusicVolume.
	void SetTrackVolume(float volume);
	/// Получить громкость буфера
	float GetVolume(int id) const;
	/// Установить панораму (-1.0f - только левый канал, 0 - оба канала, 1.0f - только правый канал)
	void SetPan(int id, float pan);
	/// Устанавливает функцию обратного вызова, которая вызывается при останове звука
	void SetCallback(int id, boost::function<void(CallbackAction action)> callback);
	/// Погасить все буферы за заданное время. Если задать targetVolume отличный от нуля, то все буферы будут приведены к заданной громкости
	void FadeAll(float time, float targetVolume = 0.0f);
	/// Погасить все буферы, кроме трека за заданное время.
	void FadeOutAllButTrack(float time);
	/// Погасить буфер за заданное время. Если задать targetVolume отличный от нуля, то буфер будет приведён к заданной громкости
	void FadeSample(int id, float time, float targetVolume = 0.0f);
	/// Погасить текущий трек за заданное время
	void FadeOutTrack(float time);
	/// Включить текущий трек за заданное время
	void FadeInTrack(const std::string& id, float time, bool looping = false);
	/// Сменить текущий трек указанным за заданное время
	void ChangeTrack(const std::string& id, float time, float start_position = 0.0f);
	/// Получить громкость музыки. Этот множитель передаётся в каждый трек.
	float GetMusicVolume() const;
	/// Установить громкость музыки. Этот множитель передаётся в каждый трек.
	void SetMusicVolume(float value);
	/// Получить громкость звуков. Этот множитель передаётся в каждый сэмпл.
	float GetSoundVolume() const;
	/// Установить громкость звуков. Этот множитель передаётся в каждый сэмпл.
	void SetSoundVolume(float value);
	/// Получить громкость звуков окружения. Этот множитель передается в каждый семпл с установленным параметром volumeMode = "ambience"
	float GetAmbienceVolume() const;
	/// Установить громкость звуков окружения. Этот множитель передается в каждый семпл с установленным параметром volumeMode = "ambience"
	void SetAmbienceVolume(float value);
	/// Получить идентификаторы проигрываемых в настоящее время сэмплов
	void QueryPlayingSamples(std::vector<std::string>& ids) const;

	// Обнулить текущий контекст
	void SuspendContext();	
	// Восстановить текущий контекст
	void ResumeContext();

	bool IsContextSuspended() const;
	
	/// Обновить состояние
	void Update(float dt);

	/// список звуков
	void DumpSamples() const;

private:
	/// Запускает фоновый поток обновления
	void StartUpdateThread();
	/// Останавливает фоновый поток обновления
	void StopUpdateThread();

	/// Добавляет буфер к списку не удалённых из-за ошибки буферов
	void AddFailedBuffer(ALuint buffer);
	/// Пытается удалить буферы, находящиеся в списке
	void DeleteFailedBuffers();

	/// Возвращает свободный источник для трека
	ALuint GetTrackSource();
	/// Освобождает источник трека
	void FreeTrackSource(ALuint source);

	int DoPlaySample(int sampleId, const std::string& id, bool looping, float volume, float pitch, bool asTrack);
	int DoQueueSample(const std::string& id, int sampleId, bool looping, float volume, float pitch, bool asTrack);

	// Обнулить текущий контекст
	void DoSuspendContext();
	// Восстановить текущий контекст
	void DoResumeContext();

private:
	mutable MUTEX_TYPE _mutex;

	ALCdevice* _device;
	ALCcontext* _context;
	
	typedef std::deque<ALuint> Buffers;
	Buffers _failedBuffers;

	typedef std::deque<ALuint> TrackSources;
	TrackSources _trackSources;
	
	float _musicVolume;
	float _soundVolume;
	float _ambienceVolume;
	
	int _trackId;
	
	std::string _nextTrackId;
	
	float _pauseBeforeNextTrack;
	float _timeFadeInNextTrack;
	float _startPositionNextTrack;
	
	bool _paused;
	int _suspension_count;

	static int _nextId;
	
	struct SampleInfo {
		std::string resource;
		int id;
		AudioSamplePtr sample;
		VolumeMode volumeMode;
		float position;
		float volumeFactor;
		float currentVolume;
		float targetVolume;
		float fadeTime;
		float targetFade;
		float pan;
		float pitch;
		bool looping;
		bool pausable;
		bool deferred;
		bool track;
		int next;

		typedef boost::function<void(CallbackAction action)> Callback;
		Callback callback;
		
		SampleInfo()
			: id(-1), volumeMode(VolumeMode::Sound), position(-1.0f), volumeFactor(1.0f), currentVolume(1.0f), targetVolume(1.0f), fadeTime(0.0f), targetFade(0.0f)
			, pan(0.0f), pitch(1.0f), looping(false), pausable(true), deferred(false), track(false), next(0)
		{}
		
		explicit SampleInfo(int id)
			: id(id), volumeMode(VolumeMode::Sound), position(-1.0f), volumeFactor(1.0f), currentVolume(1.0f), targetVolume(1.0f), fadeTime(0.0f), targetFade(0.0f)
			, pan(0.0f), pitch(1.0f), looping(false), pausable(true), deferred(false), track(false), next(0)
		{}
	};
	
	typedef std::map<int, SampleInfo> Samples;
	Samples _samples;
	std::vector<SampleInfo> _lostSamples;
	
	typedef std::map<std::string, int> Mixes;
	Mixes _mixes;
	
#if !defined(ENGINE_TARGET_WINDOWS_METRO)
	std::unique_ptr<boost::thread> _thread;
#endif // ENGINE_TARGET_WINDOWS_METRO

private:
	friend class AudioSample;

	AudioManager(const AudioManager&);
	AudioManager& operator=(const AudioManager&);
	
	void SetupSampleVolume(SampleInfo& sample) const;
};

extern AudioManager& manager;

} // namespace MM

#endif // __AUDIOMANAGER_H__
